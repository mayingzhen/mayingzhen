#ifndef _AAssetStream_H_
#define _AAssetStream_H_

#include "Stream.h"

class AAsset;

namespace ma
{
	class AAssetStream : public Stream 
	{
	public:
		AAssetStream(AAsset* pAsset);

		~AAssetStream();
	    
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

	private:
		AAsset*		m_pAAsset;
	};
}

#endif //_AAssetStream_H_



