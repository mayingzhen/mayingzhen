#ifndef _StaticCache_H_
#define _StaticCache_H_


namespace ma
{
	/** Template version of cache based on static array.
	'cacheSize' defines size of cache in bytes. */
	template <uint32_t cacheSize>
	class StaticCache
	{
	protected:
		/// Static buffer
		char mBuffer[cacheSize];

		/// Number of bytes valid in cache (written from the beginning of static buffer)
		uint32_t mValidBytes;
		/// Current read position
		uint32_t mPos;

	public:
		/// Constructor
		StaticCache()
		{
			mValidBytes = 0;
			mPos = 0;
		}

		/** Cache data pointed by 'buf'. If 'count' is greater than cache size, we cache only last bytes.
		Returns number of bytes written to cache. */
		uint32_t cacheData(const void* buf, uint32_t count)
		{
			ASSERT(avail() == 0 && "It is assumed that you cache data only after you have read everything.");

			if (count < cacheSize)
			{
				// number of bytes written is less than total size of cache
				if (count + mValidBytes <= cacheSize)
				{
					// just append
					memcpy(mBuffer + mValidBytes, buf, count);
					mValidBytes += count;
				}
				else
				{
					uint32_t begOff = count - (cacheSize - mValidBytes);
					// override old cache content in the beginning
					memmove(mBuffer, mBuffer + begOff, mValidBytes - begOff);
					// append new data
					memcpy(mBuffer + cacheSize - count, buf, count);
					mValidBytes = cacheSize;
				}
				mPos = mValidBytes;
				return count;
			}
			else
			{
				// discard all
				memcpy(mBuffer, (const char*)buf + count - cacheSize, cacheSize);
				mValidBytes = mPos = cacheSize;
				return cacheSize;
			}
		}
		/** Read data from cache to 'buf' (maximum 'count' bytes). Returns number of bytes read from cache. */
		uint32_t read(void* buf, uint32_t count)
		{
			uint32_t rb = avail();
			rb = (rb < count) ? rb : count;
			memcpy(buf, mBuffer + mPos, rb);
			mPos += rb;
			return rb;
		}

		/** Step back in cached stream by 'count' bytes. Returns 'true' if cache contains resulting position. */
		bool rewind(uint32_t count)
		{
			if (mPos < count)
			{
				clear();
				return false;
			}
			else
			{
				mPos -= count;
				return true;
			}
		}
		/** Step forward in cached stream by 'count' bytes. Returns 'true' if cache contains resulting position. */
		bool ff(uint32_t count)
		{
			if (avail() < count)
			{
				clear();
				return false;
			}
			else
			{
				mPos += count;
				return true;
			}
		}

		/** Returns number of bytes available for reading in cache after rewinding. */
		uint32_t avail() const
		{
			return mValidBytes - mPos;
		}

		/** Clear the cache */
		void clear()
		{
			mValidBytes = 0;
			mPos = 0;
		}
	};

}

#endif
