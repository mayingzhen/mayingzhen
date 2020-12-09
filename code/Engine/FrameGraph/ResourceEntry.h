#pragma once

#include "VirtualResource.h"

#include <stdint.h>

namespace ma 
{

class FrameGraph;
class ResourceAllocator;

struct PassNode;
class RenderTargetResourceEntry;

class ResourceEntryBase : public VirtualResource 
{
public:
    explicit ResourceEntryBase(const char* name, uint16_t id, bool imported, uint8_t priority) noexcept;
    ResourceEntryBase(ResourceEntryBase const&) = default;
    ~ResourceEntryBase() override;

    virtual RenderTargetResourceEntry* asRenderTargetResourceEntry() noexcept {
        return nullptr;
    }

    void preExecuteDestroy(FrameGraph& fg) noexcept override {
        discardEnd = true;
    }

    void postExecuteDevirtualize(FrameGraph& fg) noexcept override {
        discardStart = false;
    }

    static ResourceAllocator& getResourceAllocator(FrameGraph& fg) noexcept;

    // constants
    const char* const name;
    const uint16_t id;                      // for debugging and graphing
    mutable bool imported;
    const uint8_t priority;

    // updated by builder
    uint8_t version = 0;

    // computed during compile()
    uint32_t refs = 0;                      // final reference count

    // updated during execute()
    bool discardStart = true;
    bool discardEnd = false;
};


template<typename T>
class ResourceEntry : public ResourceEntryBase 
{
    T resource{};

public:
    using Descriptor = typename T::Descriptor;
    Descriptor descriptor;

    ResourceEntry(const char* name, Descriptor const& desc, uint16_t id, uint8_t priority) noexcept
            : ResourceEntryBase(name, id, false, priority), descriptor(desc) 
	{
    }

    ResourceEntry(const char* name, Descriptor const& desc, const T& r, uint16_t id,
            uint8_t priority) noexcept
            : ResourceEntryBase(name, id, true, priority), resource(r), descriptor(desc)
	{
    }

    T const& getResource() const noexcept { return resource; }

    T& getResource() noexcept { return resource; }

    void resolve(FrameGraph& fg) noexcept override { }

    void preExecuteDevirtualize(FrameGraph& fg) noexcept override
	{
        if (!imported) 
		{
            resource.create(getResourceAllocator(fg), name, descriptor);
        }
    }

    void postExecuteDestroy(FrameGraph& fg) noexcept override 
	{
        if (!imported) 
		{
            resource.destroy(getResourceAllocator(fg));
            // make sure to clear the resource as some code might rely on e.g. handles to know
            // if they need to be set or not
            resource = {};
        }
    }
};


} // namespace ma

