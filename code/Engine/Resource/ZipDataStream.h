#ifndef _ZipDataStream_H_
#define _ZipDataStream_H_

#include "Stream.h"
#include "StaticCache.h"

struct zzip_file;

namespace ma
{

	class ZipDataStream : public Stream
	{
	public:
		/// Unnamed constructor
		ZipDataStream(zzip_file* zzipFile, uint32_t uncompressedSize);
		/// Constructor for creating named streams
		ZipDataStream(const char* name, zzip_file* zzipFile, uint32_t uncompressedSize);
		~ZipDataStream(void);

		// ---------------------------------------------------------------------
		// virtual function
		// ---------------------------------------------------------------------
		// Read the requisite number of bytes from the stream, stopping at the end of the file.
		virtual uint32_t Read(IN OUT void* pBuffer, uint32_t nCount);

		// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
		virtual uint32_t Write(const void* pBuffer, uint32_t nCount);

		/** Skip a defined number of bytes. This can also be a negative value, in which case
		the file pointer rewinds a defined number of bytes*/
		virtual void Skip(int nCount);

		// Repositions the read point to a specified byte.
		virtual void Seek(uint32_t nPos);

		// Returns the current byte offset from beginning
		virtual uint32_t Tell() const;

		// Returns true if the stream has reached the end.
		virtual bool Eof() const;

		// Close the stream; this makes further operations invalid.
		virtual void Close();

		// ---------------------------------------------------------------------
		// Self
		// ---------------------------------------------------------------------
	protected:
		zzip_file* mZzipFile;

		/// We need caching because sometimes serializers step back in data stream and zziplib behaves slow
		StaticCache<2 * OGRE_STREAM_TEMP_SIZE> mCache;
	};

}

#endif
