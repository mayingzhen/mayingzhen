#include "MemoryStream.h"


namespace ma
{

	MemoryStream::MemoryStream(void* pMem, UINT nSize, bool bReadOnly)
		: Stream(bReadOnly ? AM_READ : (AM_READ | AM_WRITE))
	{
		m_pData = m_pPos = static_cast<uint8*>(pMem);
		m_nSize = nSize;
		m_pEnd = m_pData + m_nSize;
		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = false;
	}

	MemoryStream::MemoryStream(const char* pszName, void* pMem, UINT nSize, bool bReadOnly)
		: Stream(pszName, bReadOnly?AM_READ:(AM_READ|AM_WRITE))
	{
		m_pData = m_pPos = static_cast<uint8*>(pMem);
		m_nSize = nSize;
		m_pEnd = m_pData + m_nSize;
		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = false;
	}

	MemoryStream::MemoryStream(Stream* srcStream, bool bReadOnly)
		: Stream(bReadOnly?AM_READ:(AM_READ|AM_WRITE))
	{
		m_nSize = srcStream->GetSize();
		m_pData = new uint8[m_nSize + 1];
		m_pData[m_nSize] = '\0';
		m_pPos = m_pData;
		m_pEnd = m_pData + srcStream->Read(m_pData, m_nSize);
		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = true;
	}

	MemoryStream::MemoryStream(const char* pszName, Stream* srcStream, bool bReadOnly)
		: Stream(pszName, bReadOnly?AM_READ:(AM_READ|AM_WRITE))
	{
		m_nSize = srcStream->GetSize();
		m_pData = new uint8[m_nSize + 1];
		m_pData[m_nSize] = '\0';
		m_pPos = m_pData;
		m_pEnd = m_pData + srcStream->Read(m_pData, m_nSize);
		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = true;
	}

	MemoryStream::MemoryStream(const char* pszName, Stream* srcStream, UINT nSize, bool bReadOnly)
		: Stream(pszName, bReadOnly?AM_READ:(AM_READ|AM_WRITE))
	{
		m_nSize = nSize;
		m_pData = new uint8[m_nSize + 1];
		m_pData[m_nSize] = '\0';
		m_pPos = m_pData;

		if (m_nSize > srcStream->GetSize())
		{
			UINT uReadSize = srcStream->Read(m_pData, srcStream->GetSize());
			memset(m_pData + uReadSize, 0, m_nSize-uReadSize);
			m_pEnd = m_pData + m_nSize;
		}
		else
			m_pEnd = m_pData + srcStream->Read(m_pData, m_nSize);

		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = true;
	}

	MemoryStream::MemoryStream(size_t size, bool readOnly)
		:Stream(static_cast<uint16>(readOnly ? AM_READ : (AM_READ | AM_WRITE)))
	{
		m_nSize = size;
		m_pData = new uint8[m_nSize+1];
    	m_pData[m_nSize] = '\0';
		m_pPos = m_pData;
		m_pEnd = m_pData + m_nSize;
		ASSERT(m_pEnd >= m_pPos);
		m_bFreeOnClose = true;
	}

	MemoryStream::~MemoryStream()
	{
		this->Close();
	}

	// ---------------------------------------------------------------------
	// virtual function
	// ---------------------------------------------------------------------
	// Read the requisite number of bytes from the stream, stopping at the end of the file.
	UINT MemoryStream::Read(IN OUT void* pBuffer, UINT nCount)
	{
		size_t cnt = nCount;
		// Read over end of memory?
		if (m_pPos + cnt > m_pEnd)
			cnt = m_pEnd - m_pPos;
		if (cnt == 0)
			return 0;

		ASSERT (cnt<= nCount);

		memcpy(pBuffer, m_pPos, cnt);
		m_pPos += cnt;
		return cnt;
	}

	// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
	UINT MemoryStream::Write(const void* pBuffer, UINT nCount)
	{
		size_t written = 0;
		if (IsWritable())
		{
			written = nCount;
			// we only allow writing within the extents of allocated memory
			// check for buffer overrun & disallow
			if (m_pPos + written > m_pEnd)
				written = m_pEnd - m_pPos;
			if (written == 0)
				return 0;

			memcpy(m_pPos, pBuffer, written);
			m_pPos += written;
		}
		return written;
	}


	UINT MemoryStream::ReadLine( char* buf, UINT maxCount, const std::string& delim /*= "\n"*/ )
	{
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.find_first_of('\n') != std::string::npos)
		{
			trimCR = true;
		}

		size_t pos = 0;

		// Make sure pos can never go past the end of the data 
		while (pos < maxCount && m_pPos < m_pEnd)
		{
			if (delim.find(*m_pPos) != std::string::npos)
			{
				// Trim off trailing CR if this was a CR/LF entry
				if (trimCR && pos && buf[pos-1] == '\r')
				{
					// terminate 1 character early
					--pos;
				}

				// Found terminator, skip and break out
				++m_pPos;
				break;
			}

			buf[pos++] = *m_pPos++;
		}

		// terminate
		buf[pos] = '\0';

		return pos;
	}

	/** Skip a defined number of bytes. This can also be a negative value, in which case
	the file pointer rewinds a defined number of bytes*/
	void MemoryStream::Skip(int nCount)
	{
		size_t newpos = (size_t)( ( m_pPos - m_pData ) + nCount );
		ASSERT( m_pData + newpos <= m_pEnd );        

		m_pPos = m_pData + newpos;
	}

	// Repositions the read point to a specified byte.
	void MemoryStream::Seek(UINT nPos)
	{
		ASSERT( m_pData + nPos <= m_pEnd );
		m_pPos = m_pData + nPos;
	}

	// Returns the current byte offset from beginning
	UINT MemoryStream::Tell() const
	{
		//mData is start, mPos is current location
		return m_pPos - m_pData;
	}

	// Returns true if the stream has reached the end.
	bool MemoryStream::Eof() const
	{
		return m_pPos >= m_pEnd;
	}

	// Close the stream; this makes further operations invalid.
	void MemoryStream::Close()
	{
		if (m_bFreeOnClose && m_pData)
		{
			SAFE_DELETE_ARRAY(m_pData);
		}
	}

	RefPtr<MemoryStream> CreateMemoryStream(const char* pszName, Stream* srcStream, uint32 nSize, bool bReadOnly)
	{
		return new MemoryStream(pszName,srcStream,nSize,bReadOnly);
	}

	RefPtr<MemoryStream> CreateMemoryStream(const char* pszName, void* pMem, uint32 nSize, bool bReadOnly)
	{
		return new MemoryStream(pszName,pMem,nSize,bReadOnly);
	}

	RefPtr<MemoryStream> CreateMemoryStream(uint32 size, bool readOnly)
	{
		return new MemoryStream(size,readOnly);
	}

}
