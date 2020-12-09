

#include "ResourceEntry.h"
#include "FrameGraph.h"

namespace ma 
{

VirtualResource::~VirtualResource() = default;

ResourceEntryBase::ResourceEntryBase(const char* name, uint16_t id, bool imported, uint8_t priority) noexcept
        : name(name), id(id), imported(imported), priority(priority) 
{
}

ResourceAllocator& ResourceEntryBase::getResourceAllocator(FrameGraph& fg) noexcept 
{
    return fg.getResourceAllocator();
}

ResourceEntryBase::~ResourceEntryBase() = default;


} // namespace ma
