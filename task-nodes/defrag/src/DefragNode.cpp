#include "DefragNode.h"

namespace edgeai {

std::vector<Sentence> DefragNode::defrag(const std::vector<CaptionCue>& /*cues*/) const
{
    // TODO: accumulate cues per speaker until terminal punctuation, then flush;
    // force-flush is not needed since the whole file is available upfront (sidecar model).
    return {};
}

} // namespace edgeai
