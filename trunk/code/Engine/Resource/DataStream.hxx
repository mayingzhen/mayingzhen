#include "DataStream.h"


namespace ma
{

	DataStream::DataStream(Byte* pMem, size_t size,const char* pFilePath)
	{
		m_pData = pMem;
		m_size = size;
		m_pPos = m_pData;
		m_pEnd = m_pPos + size;
		_canRead = true;
		_canWrite = true;
		m_sFilePath = pFilePath ? pFilePath : "";
	}

	DataStream::DataStream(size_t size)
	{
		m_pData = (Byte*)malloc(size);
		m_size = size;
		m_pPos = m_pData;
		m_pEnd = m_pPos + size;
		_canRead = true;
		_canWrite = true;
	}


	DataStream::~DataStream()
	{
		if (m_pData)
		{
			close();
		}
	}


	bool DataStream::canRead()
	{
		return m_pData && _canRead;
	}

	bool DataStream::canWrite()
	{
		return m_pData && _canWrite;
	}

	bool DataStream::canSeek()
	{
		return m_pData != NULL;
	}

	void DataStream::close()
	{
		if (m_pData)
		{
			free(m_pData);
			m_pData == NULL;
		}
	}

	size_t DataStream::read(void* ptr, size_t size, size_t count)
	{
		if (!canRead())
			return 0;

		size_t cnt = size * count;
		if (cnt <= 0)
			return 0;

		if (m_pPos + cnt > m_pEnd)
		{
			ASSERT(false);
			return 0;
		}

		memcpy(ptr, m_pPos, cnt);
		m_pPos += cnt;

		return count;
	}

	char* DataStream::readLine(char* str, int num)
	{
		// Deal with both Unix & Windows LFs
		bool trimCR = true;

		size_t pos = 0;

		// Make sure pos can never go past the end of the data 
		while (pos < num && m_pPos < m_pEnd)
		{
			if ( *(char*)m_pPos == '\n' )  
			{
				// Trim off trailing CR if this was a CR/LF entry
				if (trimCR && pos && str[pos-1] == '\r')
				{
					// terminate 1 character early
					--pos;
				}

				// Found terminator, skip and break out
				++m_pPos;
				break;
			}

			str[pos++] = *(char*)m_pPos++;
		}

		// terminate
		str[pos] = '\0';

		return str;
	}

	size_t DataStream::write(const void* ptr, size_t size, size_t count)
	{
		if (!canWrite())
			return 0;

		size_t cnt = size * count;
		if (cnt <= 0)
			return 0;

		if (m_pPos + cnt > m_pEnd)
		{
			ASSERT(false);
			return 0;
		}

		memcpy(m_pPos, ptr, cnt);
		m_pPos += cnt;

		return count;
	}

	bool DataStream::eof()
	{
		return m_pPos >= m_pEnd;
	}

	size_t DataStream::length()
	{
		return m_size;
	}

	long int DataStream::position()
	{
		return m_pPos - m_pData;
	}

	bool DataStream::seek(long int offset, int origin)
	{
		if (!canSeek())
			return false;

		if (origin == SEEK_SET)
		{
			m_pPos = m_pData + offset;
		}
		else if (origin == SEEK_CUR)
		{
			m_pPos = m_pPos + offset;
		}
		else if (origin == SEEK_END)
		{
			m_pPos = m_pEnd - offset;
		}
	}

	bool DataStream::rewind()
	{
		return seek(0,SEEK_SET);
	}

}
