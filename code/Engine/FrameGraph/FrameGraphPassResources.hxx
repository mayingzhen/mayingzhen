
#include "FrameGraphPassResources.h"
#include "FrameGraph.h"
#include "FrameGraphHandle.h"
#include "ResourceNode.h"
#include "PassNode.h"


namespace ma
{

FrameGraphPassResources::FrameGraphPassResources(FrameGraph& fg, PassNode const& pass) noexcept
        : mFrameGraph(fg), mPass(pass) {
}

const char* FrameGraphPassResources::getPassName() const noexcept {
    return mPass.name;
}

ResourceEntryBase const& FrameGraphPassResources::getResourceEntryBase(FrameGraphHandle r) const noexcept {
    ResourceNode& node = mFrameGraph.getResourceNodeUnchecked(r);

    ResourceEntryBase const* const pResource = node.resource;
    assert(pResource);

    // TODO: we should check for write to
    //    // check that this FrameGraphHandle is indeed used by this pass
    //    ASSERT_POSTCONDITION_NON_FATAL(mPass.isReadingFrom(r),
    //            "Pass \"%s\" doesn't declare reads to resource \"%s\" -- expect graphic corruptions",
    //            mPass.name, pResource->name);

    return *pResource;
}

} // namespace ma
