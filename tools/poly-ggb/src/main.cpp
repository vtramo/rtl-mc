#include <iostream>
#include <ostream>
#include <spdlog/spdlog.h>

#include "geogebra_polyhedra_parser.h"
#include "geogebra_xml.h"
#include "geogebra_zip.h"
#include "logger.h"
#include "pointparser.h"
#include "PolyGgbProgram.h"

#ifndef RTL_MC_VERSION
#define RTL_MC_VERSION "0.0.0"
#endif

using PPL::IO_Operators::operator<<;

std::pair<PPL::dimension_type, PPL::dimension_type> getXYVariables(
    const PolyGgbProgram& polyGgbProgram,
    const SimpleSymbolTable& symbolTable
);

std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>
parseFixedValues(
    PPL::dimension_type x,
    PPL::dimension_type y,
    const SymbolTable& symbolTable,
    std::optional<std::string> fixedValuesString
);

int main(const int argc, char* argv[])
{
    PolyGgbProgram polyGgbProgram{argc, argv, RTL_MC_VERSION};
    GeogebraPolyhedraParsingResult geogebraPolyhedraParsingResult{
        parseGeogebraPolyhedra(polyGgbProgram.polyhedraSpecification())
    };

    if (std::holds_alternative<GeogebraParsingSuccess>(geogebraPolyhedraParsingResult))
    {
        auto [geogebraPatches, symbolTable]{std::get<GeogebraParsingSuccess>(geogebraPolyhedraParsingResult)};
        auto [x, y] { getXYVariables(polyGgbProgram, symbolTable) };
        auto fixedValues { parseFixedValues(x, y, symbolTable, polyGgbProgram.fixedVariableValues()) };

        auto euclidianViewSize { std::make_pair<int, int>(polyGgbProgram.width(), polyGgbProgram.height()) };
        tinyxml2::XMLDocument* geogebraXmlDocument{
            geogebraXml(
                geogebraPatches,
                symbolTable,
                fixedValues,
                polyGgbProgram.xZero(),
                polyGgbProgram.yZero(),
                polyGgbProgram.xScale(),
                polyGgbProgram.yScale(),
                euclidianViewSize,
                x, y
            )};

        if (polyGgbProgram.outputOnlyGeogebraXml())
        {
            if (!polyGgbProgram.outputFileName().has_value())
            {
                geogebraXmlDocument->Print();
            }
            else
            {
                std::string outputFileName { *polyGgbProgram.outputFileName() };
                std::string geogebraXmlFileName { endsWith(outputFileName, ".xml") ? outputFileName : (outputFileName + ".xml") };
                geogebraXmlDocument->SaveFile(geogebraXmlFileName.c_str());
            }
        }
        else
        {
            std::string ggbOutputFile{
                polyGgbProgram.outputFileName().has_value()
                    ? *polyGgbProgram.outputFileName()
                    : "geogebra.ggb"
            };

            createGeogebraGgbFile(geogebraXmlDocument, ggbOutputFile);
        }
        delete geogebraXmlDocument;
    }
    else
    {
        std::vector<ParserError> errors{std::get<std::vector<ParserError>>(geogebraPolyhedraParsingResult)};
        std::cerr << errors << std::endl;
    }

    return 0;
}

std::pair<PPL::dimension_type, PPL::dimension_type> getXYVariables(
    const PolyGgbProgram& polyGgbProgram,
    const SimpleSymbolTable& symbolTable
)
{
    std::string xVariableName {
        polyGgbProgram.xAxisVariable().has_value()
            ? *polyGgbProgram.xAxisVariable()
            : *symbolTable.getVariableName(PPL::Variable { 0 })
    };

    if (!symbolTable.containsVariable(xVariableName))
    {
        std::cerr << "Variable " << xVariableName << " not found!" << std::endl;
        exit(1);
    }

    std::optional<std::string> yVariableName {};

    if (polyGgbProgram.yAxisVariable().has_value())
    {
        yVariableName = *polyGgbProgram.yAxisVariable();
    }
    else if (symbolTable.spaceDimension() >= 2)
    {
        yVariableName = *symbolTable.getVariableName(PPL::Variable { 1 });
    }

    if (yVariableName.has_value() && !symbolTable.containsVariable(*yVariableName))
    {
        std::cerr << "Variable " << *yVariableName << " not found!" << std::endl;
        exit(1);
    }

    return std::make_pair<PPL::dimension_type, PPL::dimension_type>(
        symbolTable.getVariable(xVariableName)->space_dimension() - 1,
        yVariableName.has_value()
            ? symbolTable.getVariable(*yVariableName)->space_dimension() - 1
            : 1
    );
}

std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>
parseFixedValues(
    const PPL::dimension_type x,
    const PPL::dimension_type y,
    const SymbolTable& symbolTable,
    std::optional<std::string> fixedValuesString
)
{
    std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo> fixedValues {};

    if (fixedValuesString.has_value())
    {
        RationalPointParsingResult rationalPointParsingResult { parseRationalPoint(*fixedValuesString, symbolTable) };

        if (std::holds_alternative<std::vector<ParserError>>(rationalPointParsingResult))
        {
            spdlog::error("Errors while parsing model-checking point:");
            std::vector errors{std::get<std::vector<ParserError>>(rationalPointParsingResult)};
            for (auto& error : errors)
            {
                spdlog::error("- {}", error.errorMessage());
            }
            exit(1);
        }

        RationalPoint rationalPoint { std::get<RationalPoint>(rationalPointParsingResult) };
        for (auto& [dim, coefficient]: rationalPoint.coefficients())
        {
            if (dim == x || dim == y)
            {
                Log::configureLogger();
                spdlog::error("Cannot set a fixed value for variable '{}' since it has been assigned to {} axis",
                              *symbolTable.getVariableName(PPL::Variable{dim}), dim == x ? "X" : "Y");
                exit(1);
            }

            fixedValues.insert({PPL::Variable{dim}, PPL::Coefficient { coefficient } });
        }
    }
    else
    {
        for (PPL::dimension_type dim { 0 }; dim < symbolTable.spaceDimension(); ++dim)
        {
            PPL::Variable variable { dim };
            if (dim != x && dim != y)
            {
                fixedValues.insert({ variable, 0 });
            }
        }
    }

    return fixedValues;
}
