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
		ZipDataStream(zzip_file* zzipFile, UINT uncompressedSize);
		/// Constructor for creating named streams
		ZipDataStream(const char* name, zzip_file* zzipFile, UINT uncompressedSize);
		~ZipDataStream(void);

		// ---------------------------------------------------------------------
		// virtual function
		// ---------------------------------------------------------------------
		// Read the requisite number of bytes from the stream, stopping at the end of the file.
		virtual UINT Read(IN OUT void* pBuffer, UINT nCount);

		// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
		virtual UINT Write(const void* pBuffer, UINT nCount);

		/** Skip a defined number of bytes. This can also be a negative value, in which case
		the file pointer rewinds a defined number of bytes*/
		virtual void Skip(int nCount);

		// Repositions the read point to a specified byte.
		virtual void Seek(UINT nPos);

		// Returns the current byte offset from beginning
		virtual UINT Tell() const;

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
