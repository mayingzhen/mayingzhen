#pragma once

namespace ma
{

class FrameGraph;

struct PassNode;

struct VirtualResource 
{
    VirtualResource() noexcept = default;
    VirtualResource(VirtualResource const&) = default;
    virtual void resolve(FrameGraph& fg) noexcept = 0;
    virtual void preExecuteDevirtualize(FrameGraph& fg) noexcept = 0;
    virtual void preExecuteDestroy(FrameGraph& fg) noexcept = 0;
    virtual void postExecuteDestroy(FrameGraph& fg) noexcept = 0;
    virtual void postExecuteDevirtualize(FrameGraph& fg) noexcept = 0;
    virtual ~VirtualResource();

    // computed during compile()
    PassNode* first = nullptr;              // pass that needs to instantiate the resource
    PassNode* last = nullptr;               // pass that can destroy the resource
};


} // namespace ma


