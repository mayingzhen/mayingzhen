#pragma once


#include <vector>
#include <stdint.h>

namespace ma 
{

	// combines two hashes together, faster but less good
	template<class T>
	inline void combine_fast(size_t& seed, const T& v) noexcept {
		std::hash<T> hasher;
		seed ^= hasher(v) << 1u;
	}

class ResourceAllocator 
{
public:
    explicit ResourceAllocator() noexcept;
    ~ResourceAllocator() noexcept ;
    
    void terminate() noexcept;

    RefPtr<RenderPass> createRenderTarget(const char* name,
        const std::vector<RenderSurface>& mrtColor, RenderSurface depthStencil) noexcept;

    void destroyRenderTarget(RefPtr<RenderPass> h) noexcept;

	RefPtr<Texture> createTexture(const char* name, TEXTURE_TYPE target, uint8_t levels,
		PixelFormat format, uint8_t samples, uint32_t width, uint32_t height,
		uint32_t depth, TEXTURE_USAGE usage) noexcept;

    void destroyTexture(RefPtr<Texture> h) noexcept;

    void gc() noexcept;

private:
    // TODO: these should be settings of the engine
    static constexpr size_t CACHE_CAPACITY = 64u << 20u;   // 64 MiB
    static constexpr size_t CACHE_MAX_AGE  = 30u;

    struct TextureKey 
	{
        const char* name; // doesn't participate in the hash
		TEXTURE_TYPE target;
        uint8_t levels;
		PixelFormat format;
        uint8_t samples;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
		TEXTURE_USAGE usage;

        size_t getSize() const noexcept;

        bool operator==(const TextureKey& other) const noexcept 
		{
            return target == other.target &&
                   levels == other.levels &&
                   format == other.format &&
                   samples == other.samples &&
                   width == other.width &&
                   height == other.height &&
                   depth == other.depth &&
                   usage == other.usage;
        }



        friend size_t hash_value(TextureKey const& k) 
		{
            size_t seed = 0;
            combine_fast(seed, k.target);
            combine_fast(seed, k.levels);
            combine_fast(seed, k.format);
            combine_fast(seed, k.samples);
            combine_fast(seed, k.width);
            combine_fast(seed, k.height);
            combine_fast(seed, k.depth);
            combine_fast(seed, k.usage);
            return seed;
        }
    };

    struct TextureCachePayload 
	{
        RefPtr<Texture> handle;
        size_t age = 0;
        uint32_t size = 0;
    };

    template<typename T>
    struct Hasher 
	{
        std::size_t operator()(T const& s) const noexcept
		{
            return hash_value(s);
        }
    };

    template<typename T>
    struct Hasher<RefPtr<T>> 
	{
        std::size_t operator()(RefPtr<T> const& s) const noexcept {
            std::hash<typename RefPtr<T>::HandleId> hash{};
            return hash(s.get());
        }
    };

    inline void dump() const noexcept;

    template<typename Key, typename Value, typename Hasher = Hasher<Key>>
    class AssociativeContainer 
	{
        // We use a std::vector instead of a std::multimap because we don't expect many items
        // in the cache and std::multimap generates tons of code. Even with more items, we
        // could improve this trivially by using a sorted std::vector.
        using Container = std::vector<std::pair<Key, Value>>;
        Container mContainer;
    public:
        using iterator = typename Container::iterator;
        using const_iterator = typename Container::const_iterator;
        using key_type = typename Container::value_type::first_type;
        using value_type = typename Container::value_type::second_type;

        size_t size() const { return mContainer.size(); }
        iterator begin() { return mContainer.begin(); }
        const_iterator begin() const { return mContainer.begin(); }
        iterator end() { return mContainer.end(); }
        const_iterator end() const  { return mContainer.end(); }
        iterator erase(iterator it);
        const_iterator find(key_type const& key) const;
        iterator find(key_type const& key);
        template<typename ... ARGS>
        void emplace(ARGS&&... args);
    };

    AssociativeContainer<TextureKey, TextureCachePayload> mTextureCache;
    AssociativeContainer<RefPtr<Texture>, TextureKey> mInUseTextures;
    size_t mAge = 0;
    uint32_t mCacheSize = 0;
    const bool mEnabled = true;
};

} // namespace ma

