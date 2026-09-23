#pragma once

#include "CaptionCue.h"
#include "ParseIssue.h"

#include <vector>

namespace edgeai
{

struct ParseResult
{
    std::vector<CaptionCue> cues;

    std::vector<ParseIssue> issues;

    std::size_t blocks_read{0};

    std::size_t cues_accepted{0};

    std::size_t cues_rejected{0};

    bool hasErrors() const
    {
        for (const auto& issue : issues)
        {
            if (issue.severity ==
                IssueSeverity::Error)
            {
                return true;
            }
        }

        return false;
    }
};

} // namespace edgeai
