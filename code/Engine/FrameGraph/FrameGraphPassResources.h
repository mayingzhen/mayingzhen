#pragma once

#include "FrameGraphHandle.h"
#include "ResourceEntry.h"


namespace ma 
{

struct PassNode;
class FrameGraph;

class FrameGraphPassResources 
{
public:

    // Return the name of the pass being executed
    const char* getPassName() const noexcept;

    // get the resource for this handle
    template<typename T>
    T const& get(FrameGraphId<T> handle) const noexcept {
        return getResourceEntry(handle).getResource();
    }

    // get the descriptor of this resource
    template<typename T>
    typename T::Descriptor const& getDescriptor(FrameGraphId<T> handle) const {
        // TODO: we should check that this FrameGraphHandle is indeed used by this pass
        return getResourceEntry(handle).descriptor;
    }

    // this is just a helper for FrameGraphTexture
    RefPtr<Texture> getTexture(FrameGraphId<FrameGraphTexture> handle) const noexcept {
        return get(handle).texture;
    }

    // detach() is used to detach (export) a resource from the framegraph, at which point its
    // lifetime is no longer managed by the FrameGraph. This resource can later be used by
    // the FrameGraph again using FrameGraph::import() -- but note that this will not transfer
    // lifetime management back to the FrameGraph.
    template<typename T>
    void detach(FrameGraphId<T> r, T* resource, typename T::Descriptor* desc) const noexcept {
        ResourceEntry<T> const& entry = getResourceEntry(r);
        *resource = entry.getResource();
        *desc = entry.descriptor;
        entry.imported = true; // avoid destruction
    }

private:
    friend class FrameGraph;
    explicit FrameGraphPassResources(FrameGraph& fg, PassNode const& pass) noexcept;

    ResourceEntryBase const& getResourceEntryBase(FrameGraphHandle r) const noexcept;

    template<typename T>
    ResourceEntry<T> const& getResourceEntry(FrameGraphId<T> handle) const noexcept {
        return static_cast<ResourceEntry<T> const&>(getResourceEntryBase(handle));
    }

    FrameGraph& mFrameGraph;
    PassNode const& mPass;
};

} // namespace ma
