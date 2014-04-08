#ifndef _StaticCache_H_
#define _StaticCache_H_


namespace ma
{
	/** Template version of cache based on static array.
	'cacheSize' defines size of cache in bytes. */
	template <UINT cacheSize>
	class StaticCache
	{
	protected:
		/// Static buffer
		char mBuffer[cacheSize];

		/// Number of bytes valid in cache (written from the beginning of static buffer)
		UINT mValidBytes;
		/// Current read position
		UINT mPos;

	public:
		/// Constructor
		StaticCache()
		{
			mValidBytes = 0;
			mPos = 0;
		}

		/** Cache data pointed by 'buf'. If 'count' is greater than cache size, we cache only last bytes.
		Returns number of bytes written to cache. */
		UINT cacheData(const void* buf, UINT count)
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
					UINT begOff = count - (cacheSize - mValidBytes);
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
		UINT read(void* buf, UINT count)
		{
			UINT rb = avail();
			rb = (rb < count) ? rb : count;
			memcpy(buf, mBuffer + mPos, rb);
			mPos += rb;
			return rb;
		}

		/** Step back in cached stream by 'count' bytes. Returns 'true' if cache contains resulting position. */
		bool rewind(UINT count)
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
		bool ff(UINT count)
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
		UINT avail() const
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
