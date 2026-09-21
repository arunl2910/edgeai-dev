#pragma once
#include <cstdio>
#include <string>

namespace edgeai {

inline void logInfo(const std::string& component, const std::string& msg)
{
    std::fprintf(stderr, "[EdgeAI][%s] %s\n", component.c_str(), msg.c_str());
}

} // namespace edgeai
