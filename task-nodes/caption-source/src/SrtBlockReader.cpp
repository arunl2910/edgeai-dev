#include "SrtBlockReader.h"

#include <string>
#include <utility>

namespace edgeai
{
namespace
{

// Removes a UTF-8 BOM if present at the very start of the first line.
void stripUtf8Bom(std::string& line)
{
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF)
    {
        line.erase(0, 3);
    }
}

// std::getline splits on '\n'. This strips a trailing '\r' (CRLF).
void stripTrailingCarriageReturn(std::string& line)
{
    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }
}

bool isBlank(const std::string& line)
{
    for (const char character : line)
    {
        if (!std::isspace(static_cast<unsigned char>(character)))
        {
            return false;
        }
    }

    return true;
}

} // namespace

std::vector<CueBlock> SrtBlockReader::readBlocks(
    std::istream& stream)
{
    std::vector<CueBlock> blocks;

    CueBlock current;
    std::string line;

    std::size_t lineNumber = 0;
    bool firstLineOfFile = true;

    while (std::getline(stream, line))
    {
        ++lineNumber;

        stripTrailingCarriageReturn(line);

        if (firstLineOfFile)
        {
            stripUtf8Bom(line);
            firstLineOfFile = false;
        }

        // A blank line terminates the current block.
        if (isBlank(line))
        {
            if (!current.lines.empty())
            {
                blocks.push_back(std::move(current));
                current = CueBlock{};
            }

            continue;
        }

        if (current.lines.empty())
        {
            current.first_line_number = lineNumber;
        }

        current.lines.push_back(line);
    }

    // Flush the final block if the file did not end with a blank line.
    if (!current.lines.empty())
    {
        blocks.push_back(std::move(current));
    }

    return blocks;
}

} // namespace edgeai
