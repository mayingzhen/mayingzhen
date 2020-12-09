#pragma once

#include "FrameGraph.h"

#include <vector>
#include <stdint.h>

namespace ma 
{


struct ResourceNode;
struct VirtualResource;

struct PassNode 
{

    PassNode(FrameGraph& fg, const char* name, uint32_t id, FrameGraphPassExecutor* base) noexcept;
    PassNode(PassNode&& rhs) noexcept;
    ~PassNode();

    PassNode(PassNode const&) = delete;
    PassNode& operator=(PassNode const&) = delete;
    PassNode& operator=(PassNode&&) = delete;

    // for Builder
    FrameGraphHandle read(FrameGraph& fg, FrameGraphHandle handle);
    FrameGraphId<FrameGraphTexture> sample(FrameGraph& fg, FrameGraphId<FrameGraphTexture> handle);
    FrameGraphId<FrameGraphRenderTarget> use(FrameGraph& fg, FrameGraphId<FrameGraphRenderTarget> handle);
    FrameGraphHandle write(FrameGraph& fg, const FrameGraphHandle& handle);

    // constants
    const char* const name = nullptr;                       // our name
    const uint32_t id = 0;                                  // index in the mPassNodes (for debugging and moveResource)
    std::unique_ptr<FrameGraphPassExecutor> base;     // type eraser for calling execute()

    // set by the builder
    std::vector<FrameGraphHandle> reads;                     // resources we're reading from
	std::vector<FrameGraphHandle> writes;                    // resources we're writing to
	std::vector<FrameGraphId<FrameGraphTexture>> samples;    // resources we're sampling from
	std::vector<FrameGraphId<FrameGraphRenderTarget>> renderTargets;

    // computed during compile()
	std::vector<VirtualResource*> devirtualize;         // resources we need to create before executing
	std::vector<VirtualResource*> destroy;              // resources we need to destroy after executing
    uint32_t refCount = 0;                  // count resources that have a reference to us

    // set by the builder
    bool hasSideEffect = false;             // whether this pass has side effects
};

} // namespace ma

