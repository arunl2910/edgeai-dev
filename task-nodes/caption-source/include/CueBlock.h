// CueBlock.h

#pragma once

#include <string>
#include <vector>

namespace edgeai
{

struct CueBlock
{
    std::size_t first_line_number{0};

    std::vector<std::string> lines;
};

} // namespace edgeai
