

#include "RenderTargetResourceEntry.h"
#include "PassNode.h"
#include "ResourceNode.h"
#include "FrameGraph.h"
#include "FrameGraphHandle.h"
#include "ResourceAllocator.h"

#include <vector>


namespace ma 
{

void RenderTargetResourceEntry::resolve(FrameGraph& fg) noexcept
{
    attachments = {};
    width = 0;
    height = 0;
    auto& resource = getResource();

    static constexpr TargetBufferFlags flags[] = {
            TargetBufferFlags::COLOR0,
            TargetBufferFlags::COLOR1,
            TargetBufferFlags::COLOR2,
            TargetBufferFlags::COLOR3,
            TargetBufferFlags::DEPTH,
            TargetBufferFlags::STENCIL };

    static_assert(sizeof(flags)/sizeof(*flags) == FrameGraphRenderTarget::Attachments::COUNT,
            "array sizes don't match");

    static constexpr TEXTURE_USAGE usages[] = {
            TEXTURE_USAGE::USAGE_RENDERTARGET,
            TEXTURE_USAGE::USAGE_RENDERTARGET,
            TEXTURE_USAGE::USAGE_RENDERTARGET,
            TEXTURE_USAGE::USAGE_RENDERTARGET,
            TEXTURE_USAGE::USAGE_DEPTHSTENCIL,
            TEXTURE_USAGE::USAGE_DEPTHSTENCIL };

    static_assert(sizeof(flags)/sizeof(*flags) == sizeof(usages)/sizeof(*usages),
            "array sizes don't match");

    uint32_t minWidth = 0;
    uint32_t minHeight = 0;
    uint32_t maxWidth = 0;
    uint32_t maxHeight = 0;

    for (size_t i = 0; i < descriptor.attachments.textures.size(); i++)
	{
        auto attachment = descriptor.attachments.textures[i];
        if (attachment.isValid())
		{
            ResourceEntry<FrameGraphTexture>& entry =
                    fg.getResourceEntryUnchecked(attachment.getHandle());
            // update usage flags for referenced attachments
            entry.descriptor.usage |= usages[i];

            // update attachment sample count if not specified and usage permits it
            if (!entry.descriptor.samples &&
                none(entry.descriptor.usage & TEXTURE_USAGE::USAGE_SAMPLEABLE))
			{
                entry.descriptor.samples = descriptor.samples;
            }

            attachments |= flags[i];

            // figure out the min/max dimensions across all attachments
            const size_t level = attachment.getLevel();
            const uint32_t w = entry.descriptor.width >> level;//FTexture::valueForLevel(level, entry.descriptor.width);
            const uint32_t h = entry.descriptor.height >> level;//FTexture::valueForLevel(level, entry.descriptor.height);
            minWidth = std::min<uint32_t>(minWidth, w);
            maxWidth = std::max<uint32_t>(maxWidth, w);
            minHeight = std::min<uint32_t>(minHeight, h);
            maxHeight = std::max<uint32_t>(maxHeight, h);
        }
    }

    if (any(attachments)) 
	{
        if (minWidth == maxWidth && minHeight == maxHeight) 
		{
            // All attachments' size match, we're good to go.
            width = minWidth;
            height = minHeight;
        } 
		else 
		{
            // TODO: what should we do here? Is it a user-error?
            width = maxWidth;
            height = maxHeight;
        }

        if (resource.params.viewport.width() == 0 && resource.params.viewport.height() == 0) {
            resource.params.viewport.left = width;
            resource.params.viewport.top = height;
        }
    } 
	else 
	{
        // this can happen with imported targets
        resource.params.viewport = descriptor.viewport;
    }
    resource.params.clearColor = descriptor.clearColor;
    resource.params.flags.clear = descriptor.clearFlags;
}

void RenderTargetResourceEntry::update(FrameGraph& fg, PassNode const& pass) noexcept
{
    auto& resource = getResource();

    // this update at every pass
    if (any(attachments)) 
	{
        // overwrite discard flags with the per-rendertarget (per-pass) computed value
        resource.params.flags.discardStart = TargetBufferFlags::NONE;
        resource.params.flags.discardEnd   = TargetBufferFlags::NONE;

        static constexpr TargetBufferFlags flags[] = {
                TargetBufferFlags::COLOR0,
                TargetBufferFlags::COLOR1,
                TargetBufferFlags::COLOR2,
                TargetBufferFlags::COLOR3,
                TargetBufferFlags::DEPTH,
                TargetBufferFlags::STENCIL };

        static_assert(sizeof(flags)/sizeof(*flags) == FrameGraphRenderTarget::Attachments::COUNT,
                "array sizes don't match");

        auto& resourceNodes = fg.mResourceNodes;
        for (size_t i = 0; i < descriptor.attachments.textures.size(); i++) 
		{
            FrameGraphHandle attachment = descriptor.attachments.textures[i];
            if (attachment.isValid()) {
                if (resourceNodes[attachment.index]->resource->discardStart)
				{
                    resource.params.flags.discardStart |= flags[i];
                }
                if (resourceNodes[attachment.index]->resource->discardEnd)
				{
                    resource.params.flags.discardEnd |= flags[i];
                }
            }
        }

        // check that this FrameGraphRenderTarget is indeed declared by this pass
        ASSERTMSG(resource.target,
                "Pass \"%s\" doesn't declare rendertarget \"%s\" -- expect graphic corruptions",
                pass.name, name);
    }

    // clear implies discarding the content of the buffer
    resource.params.flags.discardStart |= resource.params.flags.clear;
}

void RenderTargetResourceEntry::preExecuteDevirtualize(FrameGraph& fg) noexcept 
{
    if (!imported)
	{
        assert(any(attachments));

        // TODO: we could cache the result of this loop
        std::vector<RenderSurface> info(FrameGraphRenderTarget::Attachments::COUNT);
        for (size_t i = 0, c = descriptor.attachments.textures.size(); i < c; i++) 
		{
            auto const& attachmentInfo = descriptor.attachments.textures[i];
#ifndef NDEBUG
            static constexpr TargetBufferFlags flags[] = {
                    TargetBufferFlags::COLOR0,
                    TargetBufferFlags::COLOR1,
                    TargetBufferFlags::COLOR2,
                    TargetBufferFlags::COLOR3,
                    TargetBufferFlags::DEPTH,
                    TargetBufferFlags::STENCIL };
            static_assert(sizeof(flags)/sizeof(*flags) == FrameGraphRenderTarget::Attachments::COUNT,
                    "array sizes don't match");
            assert(bool(attachments & flags[i]) == attachmentInfo.isValid());
#endif
            if (attachmentInfo.isValid()) 
			{
                ResourceEntry<FrameGraphTexture> const& entry =
                        fg.getResourceEntryUnchecked(attachmentInfo.getHandle());
                info[i].m_pTexture = entry.getResource().texture;
                info[i].m_nMip = attachmentInfo.getLevel();
                info[i].m_nFace = attachmentInfo.getLayer();
                // the attachment buffer (texture or renderbuffer) must be valid
                assert(info[i].m_pTexture);
                // the attachment level must be within range
                assert(info[i].m_nMip < entry.descriptor.levels);
                // if the attachment is multisampled, then the rendertarget must be too
                assert(entry.descriptor.samples <= 1 || entry.descriptor.samples == descriptor.samples);
            }
        }

        std::vector<RenderSurface> mrtColor;
        mrtColor.push_back(info[0]);
        mrtColor.push_back(info[1]);
        mrtColor.push_back(info[2]);
        RenderSurface depthStencil = info[4];

        auto& resource = getResource();
        resource.target = fg.getResourceAllocator().createRenderTarget(name,
            mrtColor,depthStencil);
    }
}

void RenderTargetResourceEntry::preExecuteDestroy(FrameGraph& fg) noexcept
{
    // FIXME: (workaround: see postExecuteDevirtualize) technically we need
    //        to clear discardEnd here (in case the RT is reused), but save
    //        its original value, to be restored in postExecuteDevirtualize()
    //        we don't do that right now, because it's not needed in our current
    //        use case.
}

void RenderTargetResourceEntry::postExecuteDestroy(FrameGraph& fg) noexcept 
{
    if (!imported) 
	{
        auto& resource = getResource();
        if (resource.target)
		{
            fg.getResourceAllocator().destroyRenderTarget(resource.target);
            //resource.target.clear();
            resource.target = nullptr;
        }
    }
}

void RenderTargetResourceEntry::postExecuteDevirtualize(FrameGraph& fg) noexcept 
{
    // after a rendertarget has been used once, it's never cleared anymore
    // (otherwise it wouldn't be possible to meaningfully reuse it)
    auto& resource = getResource();
    resource.params.flags.clear = TargetBufferFlags::NONE;

    if (imported) 
	{
        // FIXME: (workaround) imported targets currently don't have attachments associated to
        //        them, so the discard flags can't be calculated. So if the target is used
        //        several times, discardStart must be cleared after the first time.
        //        (technically, discardEnd should be reset to it's initial value -- we don't do
        //        this here, because we never modified it in the first place)
        resource.params.flags.discardStart = TargetBufferFlags::NONE;
    }
}

} // namespace ma
