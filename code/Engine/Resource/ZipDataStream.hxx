#include "ZipDataStream.h"

#include <zzip/zzip.h>

namespace ma
{

	ZipDataStream::ZipDataStream(zzip_file* zzipFile, uint32_t uncompressedSize)
	:mZzipFile(zzipFile)
	{
		m_nSize = uncompressedSize;
	}

	ZipDataStream::ZipDataStream(const char* name, zzip_file* zzipFile, uint32_t uncompressedSize)
	:Stream(name), mZzipFile(zzipFile)
	{
		m_nSize = uncompressedSize;
	}

	ZipDataStream::~ZipDataStream(void)
	{
		this->Close();
	}

	// ---------------------------------------------------------------------
	// virtual function
	// ---------------------------------------------------------------------
	// Read the requisite number of bytes from the stream, stopping at the end of the file.
	uint32_t ZipDataStream::Read(IN OUT void* buf, uint32_t count)
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

		uint32_t was_avail = mCache.read(buf, count);
		zzip_ssize_t r = 0;
		if (was_avail < count)
		{
			r = zzip_file_read(mZzipFile, (char*)buf + was_avail, count - was_avail);
			if (r<0) {
				ZZIP_DIR *dir = zzip_dirhandle(mZzipFile);
				std::string msg = zzip_strerror_of(dir);
	// 			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
	// 				mName+" - error from zziplib: "+msg,
	// 				"ZipDataStream::read");
				ASSERT(false);
				return 0;
			}
			mCache.cacheData((char*)buf + was_avail, (uint32_t)r);
		}
		return was_avail + (uint32_t)r;
	}

	// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
	uint32_t ZipDataStream::Write(const void* pBuffer, uint32_t nCount)
	{
		// not supported
		return 0;
	}

	/** Skip a defined number of bytes. This can also be a negative value, in which case
	the file pointer rewinds a defined number of bytes*/
	void ZipDataStream::Skip(int nCount)
	{
		long was_avail = static_cast<long>(mCache.avail());
		if (nCount > 0)
		{
			if (!mCache.ff(nCount))
				zzip_seek(mZzipFile, static_cast<zzip_off_t>(nCount - was_avail), SEEK_CUR);
		}
		else if (nCount < 0)
		{
			if (!mCache.rewind((uint32_t)(-(int)nCount)))
				zzip_seek(mZzipFile, static_cast<zzip_off_t>(nCount + was_avail), SEEK_CUR);
		}
	}

	// Repositions the read point to a specified byte.
	void ZipDataStream::Seek(uint32_t nPos)
	{
		zzip_off_t newPos = static_cast<zzip_off_t>(nPos);
		zzip_off_t prevPos = static_cast<zzip_off_t>(Tell());
		if (prevPos < 0)
		{
			// seek set after invalid pos
			mCache.clear();
			zzip_seek(mZzipFile, newPos, SEEK_SET);
		}
		else
		{
			// everything is going all right, relative seek
			Skip(newPos - prevPos);
		}
	}

	// Returns the current byte offset from beginning
	uint32_t ZipDataStream::Tell() const
	{
		zzip_off_t pos = zzip_tell(mZzipFile);
		if (pos<0)
			return (uint32_t)(-1);
		return static_cast<uint32_t>(pos) - mCache.avail();
	}

	// Returns true if the stream has reached the end.
	bool ZipDataStream::Eof() const
	{
		return (Tell() >= m_nSize);
	}

	// Close the stream; this makes further operations invalid.
	void ZipDataStream::Close()
	{
		if (mZzipFile != 0)
		{
			zzip_file_close(mZzipFile);
			mZzipFile = 0;
		}
		mCache.clear();
	}

}