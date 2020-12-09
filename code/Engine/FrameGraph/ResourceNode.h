#pragma once

#include <limits>
#include <stdint.h>

namespace ma 
{


struct PassNode;
class ResourceEntryBase;

struct ResourceNode 
{ 
    ResourceNode(ResourceEntryBase* resource, uint8_t version) noexcept
            : resource(resource), version(version) 
	{
	}

    ResourceNode(ResourceNode const&) = delete;
    ResourceNode(ResourceNode&&) noexcept = default;
    ResourceNode& operator=(ResourceNode const&) = delete;

    // we use a FrameGraphHandle to store an index, because conveniently is 'invalid' when
    // default initialized (and doesn't use 0 as the invalid state).
    FrameGraphHandle writerIndex{}; // only needed by moveResource

    // updated during compile()
    ResourceEntryBase* resource;    // actual (aliased) resource data
    PassNode* writer = nullptr;     // writer to this node
    uint32_t readerCount = 0;       // # of passes reading from this resource

    // constants
    const uint8_t version;          // version of the resource when the node was created
};


} // namespace ma

