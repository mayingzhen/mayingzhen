#include "ResourceAllocator.h"


namespace ma 
{

// ------------------------------------------------------------------------------------------------

template<typename K, typename V, typename H>
typename ResourceAllocator::AssociativeContainer<K, V, H>::iterator
ResourceAllocator::AssociativeContainer<K, V, H>::erase(iterator it) 
{
    return mContainer.erase(it);
}

template<typename K, typename V, typename H>
typename ResourceAllocator::AssociativeContainer<K, V, H>::const_iterator
ResourceAllocator::AssociativeContainer<K, V, H>::find(key_type const& key) const 
{
    return const_cast<AssociativeContainer*>(this)->find(key);
}

template<typename K, typename V, typename H>
typename ResourceAllocator::AssociativeContainer<K, V, H>::iterator
ResourceAllocator::AssociativeContainer<K, V, H>::find(key_type const& key)
{
    return std::find_if(mContainer.begin(), mContainer.end(), [&key](auto const& v)
	{
        return v.first == key;
    });
}

template<typename K, typename V, typename H>
template<typename... ARGS>
void ResourceAllocator::AssociativeContainer<K, V, H>::emplace(ARGS&& ... args) 
{
    mContainer.emplace_back(std::forward<ARGS>(args)...);
}

// ------------------------------------------------------------------------------------------------

size_t ResourceAllocator::TextureKey::getSize() const noexcept 
{
    size_t pixelCount = width * height * depth;
    size_t size = pixelCount; //* FTexture::getFormatSize(format);
    size_t s = std::max<uint8_t>(uint8_t(1), samples);
    if (s > 1) {
        // if we have MSAA, we assume N times the storage
        size *= s;
    }
    if (levels > 1) {
        // if we have mip-maps we assume the full pyramid
        size += size / 3;
    }
    return size;
}

ResourceAllocator::ResourceAllocator() noexcept
{
}

ResourceAllocator::~ResourceAllocator() noexcept 
{
    ASSERT(!mTextureCache.size());
    ASSERT(!mInUseTextures.size());
}

void ResourceAllocator::terminate() noexcept 
{
    ASSERT(!mInUseTextures.size());
    auto& textureCache = mTextureCache;
    for (auto it = textureCache.begin(); it != textureCache.end();) 
	{
        //mBackend.destroyTexture(it->second.handle);
        it->second.handle = nullptr;
        it = textureCache.erase(it);
    }
}

RefPtr<RenderPass> ResourceAllocator::createRenderTarget(const char* name,
	const std::vector<RenderSurface>& mrtColor, RenderSurface depthStencil) noexcept
{
    RefPtr<RenderPass> pass = GetRenderDevice()->CreateRenderPass();
    for (size_t i = 0; i < mrtColor.size(); ++i)
    {
        pass->AttachColor(i, mrtColor[i]);
    }
    pass->AttachDepthStencil(depthStencil);
    GetRenderSystem()->RenderPassStreamComplete(pass.get());
    return pass;
}

void ResourceAllocator::destroyRenderTarget(RefPtr<RenderPass> h) noexcept
{
    h = nullptr;
}

RefPtr<Texture> ResourceAllocator::createTexture(const char* name,
	TEXTURE_TYPE target, uint8_t levels,
	PixelFormat format, uint8_t samples, uint32_t width, uint32_t height,
        uint32_t depth, TEXTURE_USAGE usage) noexcept
{

    // The frame graph descriptor uses "0" to mean "auto" but the sample count that is passed to the
    // backend should always be 1 or greater.
    samples = samples ? samples : uint8_t(1);

    // do we have a suitable texture in the cache?
    RefPtr<Texture> handle;
    if (mEnabled)
	{
        auto& textureCache = mTextureCache;
        const TextureKey key{ name, target, levels, format, samples, width, height, depth, usage };
        auto it = textureCache.find(key);
        if (it != textureCache.end()) 
		{
            // we do, move the entry to the in-use list, and remove from the cache
            handle = it->second.handle;
            mCacheSize -= it->second.size;
            textureCache.erase(it);
        }
		else 
        {
            if (usage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
            {
                handle = GetRenderSystem()->CreateDepthStencil(width, height, format);
            }
            else
            {
                handle = GetRenderSystem()->CreateRenderTarget(width, height, levels, format, true, target);
            }
            
            // we don't, allocate a new texture and populate the in-use list
            //handle = mBackend.createTexture(
            //        target, levels, format, samples, width, height, depth, usage);
        }
        mInUseTextures.emplace(handle, key);
    }
	else 
	{
		if (usage == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
		{
			handle = GetRenderSystem()->CreateDepthStencil(width, height, format);
		}
		else
		{
			handle = GetRenderSystem()->CreateRenderTarget(width, height, levels, format, true, target);
		}

        //handle = mBackend.createTexture(
         //       target, levels, format, samples, width, height, depth, usage);
    }
    return handle;
}

void ResourceAllocator::destroyTexture(RefPtr<Texture> h) noexcept
{
    if (mEnabled) 
	{
        // find the texture in the in-use list (it must be there!)
        auto it = mInUseTextures.find(h);
        ASSERT(it != mInUseTextures.end());

        // move it to the cache
        const TextureKey key = it->second;
        uint32_t size = key.getSize();

        mTextureCache.emplace(key, TextureCachePayload{ h, mAge, size });
        mCacheSize += size;

        // remove it from the in-use list
        mInUseTextures.erase(it);
    } 
	else 
	{
        h = nullptr;
        //mBackend.destroyTexture(h);
    }
}

void ResourceAllocator::gc() noexcept 
{
    // this is called regularly -- usually once per frame of each Renderer

    // increase our age
    const size_t age = mAge++;

    // Purging strategy:
    // + remove entries that are older than a certain age
    // - remove only one entry per gc(), unless we're at capacity

    auto& textureCache = mTextureCache;
    for (auto it = textureCache.begin(); it != textureCache.end();) 
	{
        const size_t ageDiff = age - it->second.age;
        if (ageDiff >= CACHE_MAX_AGE) 
		{
            //mBackend.destroyTexture(it->second.handle);
            it->second.handle = nullptr;
            mCacheSize -= it->second.size;
            //slog.d << "purging " << it->second.handle.getId() << io::endl;
            it = textureCache.erase(it);
            if (mCacheSize < CACHE_CAPACITY) {
                // if we're not at capacity, only purge a single entry per gc, trying to
                // avoid a burst of work.
                break;
            }
        } 
		else 
		{
            ++it;
        }
    }

    //if (mAge % 60 == 0) dump();
    // TODO: maybe purge LRU entries if we have more than a certain number
    // TODO: maybe purge LRU entries if the size of the cache is too large
}


void ResourceAllocator::dump() const noexcept
{
//     slog.d << "# entries=" << mTextureCache.size() << ", sz=" << mCacheSize / float(1u << 20u)
//            << " MiB" << io::endl;
//     for (auto const & it : mTextureCache) {
//         auto w = it.first.width;
//         auto h = it.first.height;
//         auto f = FTexture::getFormatSize(it.first.format);
//         slog.d << it.first.name << ": w=" << w << ", h=" << h << ", f=" << f << ", sz="
//                << it.second.size / float(1u << 20u) << io::endl;
//     }
}

} // namespace ma
