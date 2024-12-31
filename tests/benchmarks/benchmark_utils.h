#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

inline std::string readBenchmarkFileAsString(std::string_view filename)
{
    std::string testCaseFilePath { filename };
    std::ifstream input { testCaseFilePath };
    assert(input);
    std::ostringstream ss {};
    ss << input.rdbuf();
    assert(input.eof() || !input.fail());
    return ss.str();
}