
#include "FrameGraphHandle.h"
#include "FrameGraph.h"
#include "ResourceAllocator.h"

#include <assert.h>

namespace ma 
{

void FrameGraphTexture::create(ResourceAllocator& allocator, const char* name,
        FrameGraphTexture::Descriptor const& desc) noexcept
{
// 	if (desc.usage == USAGE_NONE)
// 	{
// 		return;
// 	}

    // FIXME (workaround): a texture could end up with no usage if it was used as an attachment
    //  of a RenderTarget that itself was replaced by a moveResource(). In this case, the texture
    //  is simply unused.  A better fix would be to let the framegraph culling eliminate
    //  this resource, but this is currently not working or set-up this way.
    //  Instead, we simply do nothing here.
//     if (none(desc.usage)) {
//         return;
//     }

 //   assert(any(desc.usage));

    // texture that can't be sampled can't have LOD -- they obviously can't be accessed
    // note: this could happen if a texture was created with LODs, but a later pass didn't
    // actually sample from it.
//     uint8_t levels = desc.levels;
//     if (!(desc.usage & TextureUsage::SAMPLEABLE)) {
//         levels = 1;
//     }
//     assert(levels <= FTexture::maxLevelCount(desc.width, desc.height));
// 
//     uint8_t samples = desc.samples;
//     assert(samples <= 1 || none(desc.usage & TextureUsage::SAMPLEABLE));
//     if (samples > 1 && any(desc.usage & TextureUsage::SAMPLEABLE)) {
//         // Sampleable textures can't be multi-sampled
//         // This should never happen (and will be caught by the assert above), but just to be safe,
//         // we reset the sample count to 1 in that case.
//         samples = 1;
//     }

    texture = allocator.createTexture(name, desc.type, desc.levels,
            desc.format, desc.samples, desc.width, desc.height, desc.depth, desc.usage);

    assert(texture);
}

void FrameGraphTexture::destroy(ResourceAllocator& allocator) noexcept 
{
    if (texture) {
        allocator.destroyTexture(texture);
    }
}

} // namespace ma
