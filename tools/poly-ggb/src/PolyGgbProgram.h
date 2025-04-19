#pragma once

#include <string>
#include <argparse/argparse.hpp>
#include <fstream>

class PolyGgbProgram
{
public:
    PolyGgbProgram(const int argc, char* argv[], const std::string_view version = "0.0.0")
        : m_polyGgbProgram { "poly-ggb", std::string { version } }
        {
            buildPolyGgbProgram();
            parseArgs(argc, argv);
        }

    [[nodiscard]] const std::string& polyhedraSpecification() const { return m_polyhedraSpecification; }
    [[nodiscard]] const std::optional<std::string>& outputFileName() const { return m_outputFileName; }
    [[nodiscard]] bool outputOnlyGeogebraXml() const { return m_onlyGeogebraXml; }
    [[nodiscard]] const std::optional<std::string>& xAxisVariable() const { return m_xAxisVariable; }
    [[nodiscard]] const std::optional<std::string>& yAxisVariable() const { return m_yAxisVariable; }
    [[nodiscard]] const std::optional<std::string>& fixedVariableValues() const { return m_fixedVariableValuesString; }
    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }
    [[nodiscard]] double xZero() const { return m_xZero; }
    [[nodiscard]] double yZero() const { return m_yZero; }
    [[nodiscard]] double xScale() const { return m_xScale; }
    [[nodiscard]] double yScale() const { return m_yScale; }
private:
    argparse::ArgumentParser m_polyGgbProgram {};
    std::istream* m_polyhedraSpecificationInputStream{&std::cin};
    std::string m_polyhedraSpecification {};
    std::optional<std::string> m_xAxisVariable {};
    std::optional<std::string> m_yAxisVariable {};
    std::optional<std::string> m_outputFileName {};
    bool m_onlyGeogebraXml { false };
    int m_width {};
    int m_height {};
    std::optional<std::string> m_fixedVariableValuesString {};

    int m_xZero {};
    int m_yZero {};
    int m_xScale {};
    int m_yScale {};

    void addFixedVariableValuesArgument()
    {
        m_polyGgbProgram.add_group("Variables options");
        m_polyGgbProgram.add_argument("--fixed")
                .help(
                    "Specifies fixed rational values for variables. \n"
                    "Use square brackets and assign values to variables \n"
                    "Example format: [x=1/3, y=-2/3, z=1]")
                .action([&](const std::string& mcPoint)
                {
                    m_fixedVariableValuesString.emplace(mcPoint);
                    assert(m_modelCheckingPointString.has_value());
                });
    }

    void buildPolyGgbProgram()
    {
        addDescription();
        addInputArguments();
        addOutputFileNameArgument();
        addOnlyXmlArgument();
        addAxisArguments();
        addFixedVariableValuesArgument();
        addEuclidianViewArguments();
    }

    void addDescription()
    {
        m_polyGgbProgram
            .add_description("A tool to generate 2D Geogebra files from a polyhedra specification with any space dimension.")
            .add_epilog("Polyhedra specification example:\n"
                "p { a >= b + 2 } 0x008000\n"
                "q { b >= a + 2 } 0xff0000\n"
                "r ( { t = 0 & a >= 0 & -a + b >= -29 & b >= 0 & 3*a - 3*b >= -25 } ) 0xff0000"
            )
            .set_usage_max_line_width(80);
    }

    void addInputArguments()
    {
        m_polyGgbProgram.add_group("Input options");

        auto& inputPolyhedralSystemGroup{m_polyGgbProgram.add_mutually_exclusive_group()};

        inputPolyhedralSystemGroup.add_argument("--polyhedra-file", "-pf")
                                  .help("Provide the polyhedra specification from a file.")
                                  .metavar("<FILE>")
                                  .action([&](const std::string& filename)
                                      {
                                          m_polyhedraSpecificationInputStream = {new std::ifstream{filename}};
                                      }
                                  ).nargs(1);

        inputPolyhedralSystemGroup.add_argument("--polyhedra-string", "-ps")
                                  .help("Provide the polyhedra specification directly as a string.")
                                  .metavar("<STRING>")
                                  .action([&](const std::string& systemString)
                                      {
                                          m_polyhedraSpecificationInputStream = {new std::istringstream{systemString}};
                                      }
                                  ).nargs(1);

        inputPolyhedralSystemGroup.add_argument("--polyhedra-stdin", "-pi")
                                  .help("Provide the polyhedra specification via stdin (default).")
                                  .flag()
                                  .default_value(true);
    }

    void addOutputFileNameArgument()
    {
        m_polyGgbProgram.add_group("Output options");

        m_polyGgbProgram.add_argument("-O", "--output")
                        .help("Specifies the output filename for the .ggb file.\n"
                              "If used in combination with the --only-xml flag,\na Geogebra XML file will be created instead, "
                              "using the provided filename.")
                        .action([&](const std::string& outputFileName)
                        {
                            m_outputFileName.emplace(outputFileName);
                        });
    }

    void addOnlyXmlArgument()
    {
        m_polyGgbProgram.add_argument("-X", "--only-xml")
              .help("Output only the XML Geogebra file without creating a .ggb file.")
              .flag()
              .store_into(m_onlyGeogebraXml);
    }

    void addEuclidianViewArguments()
    {
        m_polyGgbProgram.add_group("Geogebra View configuration");

        m_polyGgbProgram.add_argument("-w", "--width")
              .help("Set the width of the Geogebra euclidianView XML element.")
              .default_value(1980)
              .nargs(1)
              .scan<'i', int>();

        m_polyGgbProgram.add_argument("-h", "--height")
              .help("Set the height of the Geogebra euclidianView XML element.")
              .default_value(1080)
              .nargs(1)
              .scan<'i', int>();

        m_polyGgbProgram.add_argument("-e", "--euclidean-view")
            .help("Set the coordSystem of the Geogebra euclidianView XML element\n(xZero, yZero, scale, yscale).")
            .nargs(4)
            .default_value(std::vector<double>{77.58692286282914, 828.5776895278092, 5.186657626474695, 5.091916108126125})
            .scan<'g', double>();
    }

    void addAxisArguments()
    {
        m_polyGgbProgram.add_group("Axes Configuration");

        m_polyGgbProgram.add_argument("-x", "--x-axis")
                      .help(
                          "Specifies the variable to use on the X-axis (horizontal axis).\n"
                          "By default, the first variable will be used."
                      )
                      .action([&](const std::string& xAxisVariable)
                      {
                          m_xAxisVariable.emplace(xAxisVariable);
                      });

        m_polyGgbProgram.add_argument("-y", "--y-axis")
                      .help(
                          "Specifies the variable to use on the Y-axis (vertical axis).\n"
                          "By default, the second variable will be used (if any)."
                      )
                      .action([&](const std::string& yAxisVariable)
                      {
                          m_yAxisVariable.emplace(yAxisVariable);
                      });
    }

    void parseArgs(const int argc, char* argv[])
    {
        try
        {
            m_polyGgbProgram.parse_args(argc, argv);
            std::istream& is = *m_polyhedraSpecificationInputStream;
            m_polyhedraSpecification = std::string(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
            m_width = m_polyGgbProgram.get<int>("-w");
            m_height = m_polyGgbProgram.get<int>("-h");
            auto euclidianSettings { m_polyGgbProgram.get<std::vector<double>>("-e") };
            m_xZero = euclidianSettings[0];
            m_yZero = euclidianSettings[1];
            m_xScale = euclidianSettings[2];
            m_yScale = euclidianSettings[3];
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << m_polyGgbProgram;
            exit(1);
        }

        if (m_polyhedraSpecificationInputStream != &std::cin) delete m_polyhedraSpecificationInputStream;
    }
};
