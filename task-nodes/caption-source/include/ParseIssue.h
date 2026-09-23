#pragma once

#include <string>

namespace edgeai
{

enum class IssueSeverity
{
    Info,
    Warning,
    Error
};

enum class IssueCode
{
    FileOpenFailed,

    EmptyCue,
    InvalidTiming,
    InvalidIndex,
    MissingIndex,
    MalformedCue,

    OverlappingCue,
    NonMonotonicTiming,

    EndBeforeStart,
    ZeroDuration,

    DuplicateIndex
};

struct ParseIssue
{
    IssueSeverity severity;

    IssueCode code;

    std::size_t line_number{0};

    std::string message;
};

} // namespace edgeai
