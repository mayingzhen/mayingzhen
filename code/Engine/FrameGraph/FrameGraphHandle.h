#pragma once


#include <stdint.h>
#include <array>
#include <limits>

namespace ma 
{

struct PassNode;
struct ResourceNode;
class RenderTargetResourceEntry;

class Blackboard;
class FrameGraph;
class FrameGraphPassResources;
class ResourceAllocator;

//! Texture sampler type
enum class SamplerType : uint8_t
{
	SAMPLER_2D,         //!< 2D texture
	SAMPLER_2D_ARRAY,   //!< 2D array texture
	SAMPLER_CUBEMAP,    //!< Cube map texture
	SAMPLER_EXTERNAL,   //!< External texture
	SAMPLER_3D,         //!< 3D texture
};

// ------------------------------------------------------------------------------------------------

struct FrameGraphTexture 
{
    struct Descriptor 
	{
        uint32_t width = 1;     // width of resource in pixel
        uint32_t height = 1;    // height of resource in pixel
        uint32_t depth = 1;     // # of images for 3D textures
        uint8_t levels = 1;     // # of levels for textures
        uint8_t samples = 0;    // 0=auto, 1=request not multisample, >1 only for NOT SAMPLEABLE
		TEXTURE_TYPE type = TEXTYPE_2D;     // texture target type
		PixelFormat format = PF_A8R8G8B8;    // resource internal format
		TEXTURE_USAGE usage = TEXTURE_USAGE::USAGE_RENDERTARGET;
    };

    void create(ResourceAllocator& allocator, const char* name, Descriptor const& desc) noexcept;
    void destroy(ResourceAllocator& allocator) noexcept;

    RefPtr<Texture> texture;
};

// ------------------------------------------------------------------------------------------------

template<typename T>
class FrameGraphId;

class FrameGraphHandle 
{
    template<typename T>
    friend class FrameGraphId;
    friend class Blackboard;
    friend class FrameGraph;
    friend class FrameGraphPassResources;
    friend struct PassNode;
    friend struct ResourceNode;
    friend class RenderTargetResourceEntry;

    // private ctor -- this cannot be constructed by users
    FrameGraphHandle() noexcept = default;
    explicit FrameGraphHandle(uint16_t index) noexcept : index(index) {}

	static constexpr uint16_t UNINITIALIZED = -1;// std::numeric_limits<uint16_t>::max();
    // index to the resource handle
    uint16_t index = UNINITIALIZED;

public:
    FrameGraphHandle(FrameGraphHandle const& rhs) noexcept = default;
    FrameGraphHandle(FrameGraphHandle&& rhs) noexcept : index(rhs.index) { rhs.index = UNINITIALIZED; }
    FrameGraphHandle& operator=(FrameGraphHandle const& rhs) noexcept = default;
    FrameGraphHandle& operator=(FrameGraphHandle&& rhs) noexcept  
	{
        std::swap(rhs.index, index);
        return *this;
    }

    bool isValid() const noexcept { return index != UNINITIALIZED; }

    void clear() noexcept { index = UNINITIALIZED; }

    bool operator < (const FrameGraphHandle& rhs) const noexcept 
	{
        return index < rhs.index;
    }

    bool operator == (const FrameGraphHandle& rhs) const noexcept 
	{
        return (index == rhs.index);
    }

    bool operator != (const FrameGraphHandle& rhs) const noexcept 
	{
        return !operator==(rhs);
    }
};

/*
 * A FrameGraph resource ID.
 *
 * This is used to represent a virtual resource.
 */

template<typename T>
class FrameGraphId : public FrameGraphHandle 
{
public:
    using FrameGraphHandle::FrameGraphHandle;
    FrameGraphId() noexcept = default;
    explicit FrameGraphId(FrameGraphHandle r) : FrameGraphHandle(r) { }
};

/**
 * Bitmask for selecting render buffers
 */
enum class TargetBufferFlags : uint8_t 
{
    NONE = 0x0u,                            //!< No buffer selected.
    COLOR0 = 0x1u,                          //!< Color buffer selected.
    COLOR1 = 0x2u,                          //!< Color buffer selected.
    COLOR2 = 0x4u,                          //!< Color buffer selected.
    COLOR3 = 0x8u,                          //!< Color buffer selected.
    COLOR = COLOR0,                         //!< \deprecated
    COLOR_ALL = COLOR0 | COLOR1 | COLOR2 | COLOR3,
    DEPTH = 0x10u,                          //!< Depth buffer selected.
    STENCIL = 0x20u,                        //!< Stencil buffer selected.
    DEPTH_AND_STENCIL = DEPTH | STENCIL,    //!< depth and stencil buffer selected.
    ALL = COLOR_ALL | DEPTH | STENCIL       //!< Color, depth and stencil buffer selected.
};

template<> struct EnableBitMaskOperators<TargetBufferFlags> : public std::true_type {};

inline TargetBufferFlags getMRTColorFlag(size_t index) noexcept 
{
    assert(index < 4);
    return TargetBufferFlags(1u << index);
}


/**
 * Selects which buffers to clear at the beginning of the render pass, as well as which buffers
 * can be discarded at the beginning and end of the render pass.
 *
 */
struct RenderPassFlags
{
    /**
     * bitmask indicating which buffers to clear at the beginning of a render pass.
     * This implies discard.
     */
    TargetBufferFlags clear;

    /**
     * bitmask indicating which buffers to discard at the beginning of a render pass.
     * Discarded buffers have uninitialized content, they must be entirely drawn over or cleared.
     */
    TargetBufferFlags discardStart;

    /**
     * bitmask indicating which buffers to discard at the end of a render pass.
     * Discarded buffers' content becomes invalid, they must not be read from again.
     */
    TargetBufferFlags discardEnd;
};

/**
 * Specifies the mapping of the near and far clipping plane to window coordinates.
 */
// struct DepthRange 
// {
//     float near;    //!< mapping of the near plane to window coordinates.
//     float far;     //!< mapping of the far plane to window coordinates.
// };

struct RenderPassParams 
{
    RenderPassFlags flags{};    //!< operations performed on the buffers for this pass

    Rectangle viewport{};        //!< viewport for this pass
   // DepthRange depthRange{};    //!< depth range for this pass

    //! Color to use to clear the COLOR buffer. RenderPassFlags::clear must be set.
    ColourValue clearColor;

    //! Depth value to clear the depth buffer with
    double clearDepth = 0.0;

    //! Stencil value to clear the stencil buffer with
    uint32_t clearStencil = 0;

    /**
     * The subpass mask specifies which color attachments are designated for read-back in the second
     * subpass. If this is zero, the render pass has only one subpass. The least significant bit
     * specifies that the first color attachment in the render target is a subpass input.
     *
     * For now only 2 subpasses are supported, so only the lower 4 bits are used, one for each color
     * attachment (see MRT::TARGET_COUNT).
     */
    uint32_t subpassMask = 0;
};

// ------------------------------------------------------------------------------------------------

struct FrameGraphRenderTarget
{
    struct Attachments 
	{
        struct AttachmentInfo 
		{
            // auto convert to FrameGraphHandle (allows: handle = desc.attachments.color;)
            operator FrameGraphId<FrameGraphTexture>() const noexcept { return mHandle; } // NOLINT

            AttachmentInfo() noexcept = default;

            // auto convert from FrameGraphHandle (allows: desc.attachments.color = handle;)
            AttachmentInfo(FrameGraphId<FrameGraphTexture> handle) noexcept : mHandle(handle) {} // NOLINT

            // allows: desc.attachments.color = { handle, level };
            AttachmentInfo(FrameGraphId<FrameGraphTexture> handle, uint8_t level) noexcept
                    : mHandle(handle), mLevel(level) {}

            AttachmentInfo(FrameGraphId<FrameGraphTexture> handle, uint8_t level, uint8_t layer) noexcept
                    : mHandle(handle), mLevel(level), mLayer(layer) {}

            bool isValid() const noexcept { return mHandle.isValid(); }

            FrameGraphId<FrameGraphTexture> getHandle() const noexcept { return mHandle; }
            uint8_t getLevel() const noexcept { return mLevel; }
            uint8_t getLayer() const noexcept { return mLayer; }

        private:
            FrameGraphId<FrameGraphTexture> mHandle{};
            uint8_t mLevel = 0;
            uint8_t mLayer = 0;
        };

        Attachments() noexcept
                : textures{} 
		{
        }

        Attachments(AttachmentInfo c) noexcept  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
                : textures{ c } 
		{
        }

        Attachments(AttachmentInfo c, AttachmentInfo d) noexcept
                : textures{ c, {}, {}, {}, d }
		{
        }

        Attachments(AttachmentInfo c, AttachmentInfo d, AttachmentInfo s) noexcept
                : textures{ c, {}, {}, {}, d, s } 
		{
        }

        Attachments(std::array<AttachmentInfo, 4> mrt,
                AttachmentInfo d, AttachmentInfo s) noexcept
                : textures{ mrt[0], mrt[1], mrt[2], mrt[3], d, s } 
		{
        }

        static constexpr size_t COUNT = 6;
        std::array<AttachmentInfo, COUNT> textures = {};
    };

    struct Descriptor 
	{
        Attachments attachments;
		Rectangle viewport;
		ColourValue clearColor{};
        uint8_t samples = 0; // # of samples (0 = unset, default)
        TargetBufferFlags clearFlags{};
    };

    RefPtr<RenderPass> target;
    RenderPassParams params;

    // these are empty because we have custom overrides for rendertargets
    void create(ResourceAllocator& allocator, const char* name, Descriptor const& desc) noexcept {}
    void destroy(ResourceAllocator& allocator) noexcept {}
};

using FrameGraphRenderTargetHandle = FrameGraphId<FrameGraphRenderTarget>;

} // namespace ma

