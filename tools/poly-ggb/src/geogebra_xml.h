#pragma once

#include <tinyxml2.h>
#include "GeogebraPatch.h"
#include "SimpleSymbolTable.h"

static void addExpressionElement(
    tinyxml2::XMLElement* element,
    const SimpleSymbolTable& symbolTable,
    const std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>& fixedValues,
    const GeogebraPatch& geogebraPatch,
    const PPL::dimension_type x = 0,
    const PPL::dimension_type y = 1,
    const unsigned layer = 0
)
{
    tinyxml2::XMLElement* expressionElement { element->InsertNewChildElement("expression") };
    expressionElement->SetAttribute("label", geogebraPatch.label().c_str());
    expressionElement->SetAttribute("exp", geogebraPatch.toGeogebraExpression(symbolTable, fixedValues, x, y).c_str());


    tinyxml2::XMLElement* geogebraElement { element->InsertNewChildElement("element") };
    geogebraElement->SetAttribute("type", "functionNVar");
    geogebraElement->SetAttribute("label", geogebraPatch.label().c_str());
    auto show { geogebraElement->InsertNewChildElement("show") };
    show->SetAttribute("object", true);
    show->SetAttribute("label", false);

    auto objColor { geogebraElement->InsertNewChildElement("objColor") };
    RGBColor patchColor { geogebraPatch.color() };
    objColor->SetAttribute("r", patchColor.red());
    objColor->SetAttribute("g", patchColor.green());
    objColor->SetAttribute("b", patchColor.blue());
    objColor->SetAttribute("alpha", "0.75");

    auto lineStyle { geogebraElement->InsertNewChildElement("lineStyle") };
    lineStyle->SetAttribute("thickness", 0);
    lineStyle->SetAttribute("type", 0);
    lineStyle->SetAttribute("typeHidden", 1);

    auto layerElement { geogebraElement->InsertNewChildElement("layer") };
    layerElement->SetAttribute("val", layer);
}

inline tinyxml2::XMLDocument* geogebraXml(
    const std::vector<GeogebraPatch>& geogebraPatches,
    const SimpleSymbolTable& symbolTable,
    const std::unordered_map<PPL::Variable, PPL::Coefficient, std::hash<PPL::Variable>, PPLVariableEqualTo>& fixedValues,
    const double xZero, const double yZero, const double scale, const double yscale,
    const std::pair<int, int> euclidianViewSize = { 1980, 1080 },
    const PPL::dimension_type x = 0,
    const PPL::dimension_type y = 1,
    const std::string_view title = "",
    const std::string_view author = "",
    const std::string_view date = ""
)
{
    if (euclidianViewSize.first <= 0 || euclidianViewSize.second <= 0)
    {
        throw std::runtime_error("geogebraXml: Euclidean view width and height must be positive.");
    }

    tinyxml2::XMLDocument* document { new tinyxml2::XMLDocument };
    tinyxml2::XMLDeclaration* decl { document->NewDeclaration() };
    document->InsertFirstChild(decl);

    tinyxml2::XMLElement* geogebraElement { document->NewElement("geogebra") };
    geogebraElement->SetAttribute("format", "5.0");
    document->InsertEndChild(geogebraElement);

    auto euclidianView { geogebraElement->InsertNewChildElement("euclidianView") };
    auto size { euclidianView->InsertNewChildElement("size") };
    size->SetAttribute("width", euclidianViewSize.first);
    size->SetAttribute("height", euclidianViewSize.second);

    auto coordSystem { euclidianView->InsertNewChildElement("coordSystem") };
    coordSystem->SetAttribute("xZero", xZero );
    coordSystem->SetAttribute("yZero", yZero);
    coordSystem->SetAttribute("scale", scale);
    coordSystem->SetAttribute("yscale", yscale);

    tinyxml2::XMLElement* constructionElement { geogebraElement->InsertNewChildElement("construction") };
    constructionElement->SetAttribute("title", std::string { title }.c_str());
    constructionElement->SetAttribute("author", std::string { author }.c_str());
    constructionElement->SetAttribute("date", std::string { date }.c_str());

    unsigned layer {static_cast<unsigned>(geogebraPatches.size() - 1)};
    for (const GeogebraPatch& geogebraPatch: geogebraPatches)
    {
        addExpressionElement(constructionElement, symbolTable, fixedValues, geogebraPatch, x, y, layer--);
    }

    return document;
}