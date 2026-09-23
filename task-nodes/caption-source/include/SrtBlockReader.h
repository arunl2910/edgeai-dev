#pragma once

#include "CueBlock.h"

#include <istream>
#include <vector>

namespace edgeai
{

class SrtBlockReader
{
public:

    static std::vector<CueBlock>
    readBlocks(std::istream& input);
};

} // namespace edgeai
