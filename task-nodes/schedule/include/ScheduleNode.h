#pragma once

#include <cstdint>
#include <vector>

namespace edgeai {

struct TtsClip
{
    std::vector<int16_t> pcm;
    int64_t target_pts_ms{0};
    int64_t end_pts_ms{0};
};

// Holds finished TtsClips keyed by target_pts and releases them into the audio-mix
// appsrc once the pipeline clock reaches (target_pts - lead_time). Not implemented yet (stub).
class ScheduleNode
{
public:
    void hold(const TtsClip& clip);

    // Called periodically with the current pipeline position; pushes due clips into
    // appsrc, drops any clip whose target_pts has already passed.
    void tick(int64_t currentPositionMs);
};

} // namespace edgeai
