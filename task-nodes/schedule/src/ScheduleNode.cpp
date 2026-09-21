#include "ScheduleNode.h"

namespace edgeai {

void ScheduleNode::hold(const TtsClip& /*clip*/)
{
    // TODO: insert into a priority queue keyed by target_pts_ms.
}

void ScheduleNode::tick(int64_t /*currentPositionMs*/)
{
    // TODO: pop + gst_app_src_push_buffer() for due clips; drop late ones.
}

} // namespace edgeai
