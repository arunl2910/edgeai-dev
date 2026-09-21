#include "CaptionSourceNode.h"

namespace edgeai {

std::vector<CaptionCue> CaptionSourceNode::parseSrtFile(const std::string& /*path*/) const
{
    // TODO(AI-37-05): block-split on blank lines; parse index/timestamp/text; strip
    // speaker prefix + inline tags; sort + flag monotonic-order/overlap anomalies.
    return {};
}

} // namespace edgeai
