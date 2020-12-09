#include "PassNode.h"
#include "ResourceNode.h"

namespace ma 
{


PassNode::PassNode(FrameGraph& fg, const char* name, uint32_t id,
        FrameGraphPassExecutor* base) noexcept
        : name(name), id(id), base(base)
{
}

PassNode::PassNode(PassNode&& rhs) noexcept = default;

PassNode::~PassNode() = default;

// for Builder
FrameGraphHandle PassNode::read(FrameGraph& fg, FrameGraphHandle handle) {
    // don't allow multiple reads of the same resource -- it's just redundant.
    auto pos = std::find_if(reads.begin(), reads.end(),
            [&handle](FrameGraphHandle cur) { return handle.index == cur.index; });
    if (pos == reads.end()) {
        // just record that we're reading from this resource (at the given version)
        reads.push_back(handle);
    }
    return handle;
}

FrameGraphId<FrameGraphTexture> PassNode::sample(FrameGraph& fg,
        FrameGraphId<FrameGraphTexture> handle) {
    // sample() implies a read
    read(fg, handle);

    // don't allow multiple reads of the same resource -- it's just redundant.
    auto pos = std::find_if(samples.begin(), samples.end(),
            [&handle](FrameGraphHandle cur) { return handle.index == cur.index; });
    if (pos == samples.end()) {
        // just record that we're reading from this resource (at the given version)
        samples.push_back(handle);
    }
    return handle;
}

FrameGraphId<FrameGraphRenderTarget> PassNode::use(FrameGraph& fg,
        FrameGraphId<FrameGraphRenderTarget> handle) {
    // use() implies a read
    read(fg, handle);

    // don't allow multiple use() of the same FrameGraphRenderTarget -- it's just redundant.
    auto pos = std::find_if(renderTargets.begin(), renderTargets.end(),
            [&handle](FrameGraphHandle cur) { return handle.index == cur.index; });
    if (pos == renderTargets.end()) {
        // just record that we're reading from this resource (at the given version)
        renderTargets.push_back(handle);
    }
    return handle;
}

FrameGraphHandle PassNode::write(FrameGraph& fg, const FrameGraphHandle& handle) {
    ResourceNode const& node = fg.getResourceNode(handle);

    // don't allow multiple writes of the same resource -- it's just redundant.
    auto pos = std::find_if(writes.begin(), writes.end(),
            [&handle](FrameGraphHandle cur) { return handle.index == cur.index; });
    if (pos != writes.end()) {
        return *pos;
    }

    /*
     * We invalidate and rename handles that are written into, to avoid undefined order
     * access to the resources.
     *
     * e.g. forbidden graphs
     *
     *         +-> [R1] -+
     *        /           \
     *  (A) -+             +-> (A)
     *        \           /
     *         +-> [R2] -+        // failure when setting R2 from (A)
     *
     */

    ++node.resource->version;

    // writing to an imported resource should count as a side-effect
    if (node.resource->imported) {
        hasSideEffect = true;
    }

    FrameGraphHandle r = fg.createResourceNode(node.resource);

    // record the write
    auto& newNode = fg.getResourceNodeUnchecked(r);
    assert(!newNode.writerIndex.isValid());
    newNode.writerIndex = FrameGraphHandle(this->id); // needed by move resources

    writes.push_back(r);
    return r;
}

} // namespace ma
