#ifndef _MemoryStream_H_
#define _MemoryStream_H_

#include "Stream.h"

namespace ma
{

	class MemoryStream : public Stream
	{
	public:
		MemoryStream(void* pMem, UINT nSize, bool bReadOnly = false);
		MemoryStream(const char* pszName, void* pMem, UINT nSize, bool bReadOnly = false);
		MemoryStream(Stream* srcStream, bool bReadOnly = false);
		MemoryStream(const char* pszName, Stream* srcStream, bool bReadOnly = false);
		MemoryStream(const char* pszName, Stream* srcStream, UINT nSize, bool bReadOnly = false);
		MemoryStream(size_t size, bool readOnly = false);
		~MemoryStream(void);

		// ---------------------------------------------------------------------
		// virtual function
		// ---------------------------------------------------------------------
		// Read the requisite number of bytes from the stream, stopping at the end of the file.
		virtual UINT Read(IN OUT void* pBuffer, UINT nCount);

		// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
		virtual UINT Write(const void* pBuffer, UINT nCount);

		virtual UINT ReadLine(char* buf, UINT maxCount, const std::string& delim = "\n");

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
	public:
		uint8* GetPtr() const{return m_pData;}
		uint8* GetCurrentPtr() const{return m_pPos;}

	private:
		// Pointer to the start of the data area
		uint8* m_pData;

		// Pointer to the current position in the memory
		uint8* m_pPos;

		// Pointer to the end of the memory
		uint8* m_pEnd;

		// Do we delete the memory on close
		bool m_bFreeOnClose;
	};


	DeclareRefPtr(MemoryStream);

	 RefPtr<MemoryStream> CreateMemoryStream(const char* pszName, Stream* srcStream, uint32 nSize, bool bReadOnly);
	 RefPtr<MemoryStream> CreateMemoryStream(const char* pszName, void* pMem, uint32 nSize, bool bReadOnly);
	 RefPtr<MemoryStream> CreateMemoryStream(uint32 size, bool readOnly);

}

#endif
