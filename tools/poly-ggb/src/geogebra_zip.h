#pragma once

#include <iostream>
#include <ostream>
#include <minizip/zip.h>
#include <tinyxml2.h>
#include "utils.h"

inline void createGeogebraGgbFile(const tinyxml2::XMLDocument* geogebraXml, std::string ggbFileName = "geogebra.ggb" )
{
    tinyxml2::XMLPrinter printer {};
    geogebraXml->Print(&printer);

    const char* xmlData { printer.CStr() };
    size_t xmlSize { strlen(xmlData) };
    ggbFileName = endsWith(ggbFileName, ".ggb") ? ggbFileName : (ggbFileName + ".ggb");

    zipFile zip { zipOpen(ggbFileName.c_str(), APPEND_STATUS_CREATE) };
    if (!zip)
    {
        std::cerr << "Unable to create ggb file." << std::endl;
        exit(1);
    }

    if (zipOpenNewFileInZip(zip, "geogebra.xml", nullptr,
                        nullptr, 0, nullptr, 0, nullptr,
                        Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
        zipClose(zip, nullptr);
        std::cerr << "Unable to write geogebra.xml to ggb file." << std::endl;
        exit(1);
    }

    if (zipWriteInFileInZip(zip, xmlData, static_cast<unsigned int>(xmlSize)) != ZIP_OK)
    {
        std::cerr << "Unable to write data to ggb file." << std::endl;
        exit(1);
    }

    zipCloseFileInZip(zip);
    zipClose(zip, nullptr);
}