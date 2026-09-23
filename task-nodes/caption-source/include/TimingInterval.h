// TimingInterval.h

#pragma once

#include <cstdint>

namespace edgeai
{

struct TimingInterval
{
    std::int64_t start_pts{0};
    std::int64_t end_pts{0};
};

} // namespace edgeai
