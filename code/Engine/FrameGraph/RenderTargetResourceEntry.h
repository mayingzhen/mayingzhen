#pragma once

#include "ResourceEntry.h"
#include "FrameGraphHandle.h"

namespace ma 
{

class FrameGraph;


class RenderTargetResourceEntry : public ResourceEntry<FrameGraphRenderTarget> 
{
public:
    using ResourceEntry<FrameGraphRenderTarget>::ResourceEntry;

    void update(FrameGraph& fg, PassNode const& pass) noexcept;

private:
    void resolve(FrameGraph& fg) noexcept override;
    void preExecuteDevirtualize(FrameGraph& fg) noexcept override;
    void postExecuteDestroy(FrameGraph& fg) noexcept override;
    void preExecuteDestroy(FrameGraph& fg) noexcept override;
    void postExecuteDevirtualize(FrameGraph& fg) noexcept override;
    RenderTargetResourceEntry* asRenderTargetResourceEntry() noexcept override { return this; }

    // render target creation info
    uint32_t width;
    uint32_t height;
    TargetBufferFlags attachments;
};

} // namespace ma


