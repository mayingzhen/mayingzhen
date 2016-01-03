#include "FileStream.h"

namespace ma
{
	FileStream::FileStream(std::ifstream* s, bool freeOnClose)
		: Stream(AM_READ), mpInStream(s), mpFStream(NULL), mpFStreamRO(s), m_bFreeOnClose(freeOnClose)
	{
		// calculate the size
		mpInStream->seekg(0, std::ios_base::end);
		m_nSize = (UINT)mpInStream->tellg();
		mpInStream->seekg(0, std::ios_base::beg);
		this->DetermineAccess();
	}

	FileStream::FileStream(const char* pszName, std::ifstream* s, bool freeOnClose)
		: Stream(pszName, AM_READ), mpInStream(s), mpFStream(NULL), mpFStreamRO(s), m_bFreeOnClose(freeOnClose)
	{
		// calculate the size
		mpInStream->seekg(0, std::ios_base::end);
		m_nSize = (UINT)mpInStream->tellg();
		mpInStream->seekg(0, std::ios_base::beg);
		this->DetermineAccess();
	}

	FileStream::FileStream(std::ifstream* s, UINT nSize, bool freeOnClose)
		: Stream(AM_READ), mpInStream(s), mpFStream(NULL), mpFStreamRO(s), m_bFreeOnClose(freeOnClose)
	{
		m_nSize = nSize;
		this->DetermineAccess();
	}

	FileStream::FileStream(const char* pszName, std::ifstream* s, UINT nSize, bool freeOnClose)
		: Stream(pszName, AM_READ), mpInStream(s), mpFStream(NULL), mpFStreamRO(s), m_bFreeOnClose(freeOnClose)
	{
		m_nSize = nSize;
		this->DetermineAccess();
	}

	// 
	FileStream::FileStream(std::fstream* s, bool freeOnClose)
		: Stream(AM_READ|AM_WRITE), mpInStream(s), mpFStream(s), mpFStreamRO(NULL), m_bFreeOnClose(freeOnClose)
	{
		// calculate the size
		mpInStream->seekg(0, std::ios_base::end);
		m_nSize = (size_t)mpInStream->tellg();
		mpInStream->seekg(0, std::ios_base::beg);
		this->DetermineAccess();
	}

	FileStream::FileStream(const char* pszName, std::fstream* s, bool freeOnClose)
		: Stream(pszName, AM_READ|AM_WRITE), mpInStream(s), mpFStream(s), mpFStreamRO(NULL), m_bFreeOnClose(freeOnClose)
	{
		// calculate the size
		mpInStream->seekg(0, std::ios_base::end);
		m_nSize = (size_t)mpInStream->tellg();
		mpInStream->seekg(0, std::ios_base::beg);
		this->DetermineAccess();
	}

	FileStream::FileStream(std::fstream* s, UINT nSize, bool freeOnClose)
		: Stream(AM_READ|AM_WRITE), mpInStream(s), mpFStream(s), mpFStreamRO(NULL), m_bFreeOnClose(freeOnClose)
	{
		m_nSize = nSize;
		this->DetermineAccess();
	}

	FileStream::FileStream(const char* pszName, std::fstream* s, UINT nSize, bool freeOnClose)
		: Stream(pszName, AM_READ|AM_WRITE), mpInStream(s), mpFStream(s), mpFStreamRO(NULL), m_bFreeOnClose(freeOnClose)
	{
		m_nSize = nSize;
		this->DetermineAccess();
	}

	FileStream::~FileStream(void)
	{
		this->Close();
	}

	// ---------------------------------------------------------------------
	// virtual function
	// ---------------------------------------------------------------------
	// Read the requisite number of bytes from the stream, stopping at the end of the file.
	UINT FileStream::Read(IN OUT void* pBuffer, UINT nCount)
	{
		mpInStream->read(static_cast<char*>(pBuffer), static_cast<std::streamsize>(nCount));
		return (UINT)mpInStream->gcount();
	}

	// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
	UINT FileStream::Write(const void* pBuffer, UINT nCount)
	{
		size_t written = 0;
		if (this->IsWritable() && mpFStream)
		{
			mpFStream->write(static_cast<const char*>(pBuffer), static_cast<std::streamsize>(nCount));
			written = nCount;
		}
		return written;
	}

	UINT FileStream::ReadLine( char* buf, UINT maxCount, const std::string& delim /*= "\n"*/ )
	{
		if (delim.empty())
		{
			ASSERT(false&&"No delimiter provided");
		}
		if (delim.size() > 1)
		{
			//LogManager::getSingleton().logMessage(
			//	"WARNING: FileStreamDataStream::readLine - using only first delimeter");
		}
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.at(0) == '\n') 
		{
			trimCR = true;
		}
		// maxCount + 1 since count excludes terminator in getline
		mpInStream->getline(buf, static_cast<std::streamsize>(maxCount+1), delim.at(0));
		size_t ret = (size_t)mpInStream->gcount();
		// three options
		// 1) we had an eof before we read a whole line
		// 2) we ran out of buffer space
		// 3) we read a whole line - in this case the delim character is taken from the stream but not written in the buffer so the read data is of length ret-1 and thus ends at index ret-2
		// in all cases the buffer will be null terminated for us

		if (mpInStream->eof()) 
		{
			// no problem
		}
		else if (mpInStream->fail())
		{
			// Did we fail because of maxCount hit? No - no terminating character
			// in included in the count in this case
			if (ret == maxCount)
			{
				// clear failbit for next time 
				mpInStream->clear();
			}
			else
			{
				ASSERT(false && "Streaming error occurred");
			}
		}
		else 
		{
			// we need to adjust ret because we want to use it as a
			// pointer to the terminating null character and it is
			// currently the length of the data read from the stream
			// i.e. 1 more than the length of the data in the buffer and
			// hence 1 more than the _index_ of the NULL character
			--ret;
		}

		// trim off CR if we found CR/LF
		if (trimCR && buf[ret-1] == '\r')
		{
			--ret;
			buf[ret] = '\0';
		}
		return ret;
	}

	/** Skip a defined number of bytes. This can also be a negative value, in which case
	the file pointer rewinds a defined number of bytes*/
	void FileStream::Skip(int nCount)
	{
		mpInStream->clear(); //Clear fail status in case eof was set
		mpInStream->seekg(static_cast<std::ifstream::pos_type>(nCount), std::ios::cur);
	}

	// Repositions the read point to a specified byte.
	void FileStream::Seek(UINT nPos)
	{
		mpInStream->clear(); //Clear fail status in case eof was set
		mpInStream->seekg(static_cast<std::streamoff>(nPos), std::ios::beg);
	}

	// Returns the current byte offset from beginning
	UINT FileStream::Tell() const
	{
		mpInStream->clear(); //Clear fail status in case eof was set
		return (size_t)mpInStream->tellg();
	}

	// Returns true if the stream has reached the end.
	bool FileStream::Eof() const
	{
		return mpInStream->eof();
	}

	// Close the stream; this makes further operations invalid.
	void FileStream::Close()
	{
		if (mpInStream)
		{
			if (mpFStream)
			{
				mpFStream->flush();
				mpFStream->close();
			}

			if (mpFStreamRO)
				mpFStreamRO->close();

			if (m_bFreeOnClose)
			{
				SAFE_DELETE(mpFStream);
				SAFE_DELETE(mpFStreamRO);
			}
		}
	}

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	void FileStream::DetermineAccess()
	{
		m_eAccessMode = 0;
		if (mpInStream)
			m_eAccessMode |= AM_READ;
		if (mpFStream)
			m_eAccessMode |= AM_WRITE;
	}

	RefPtr<FileStream> CreateFileStream(const char* pszFile)
	{
		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszFile;
		strFullName = StringUtil::standardiseDir(strFullName);

		std::fstream* pfstream = new std::fstream(strFullName.c_str(), ios::out|ios::binary);
		ASSERT(!pfstream->fail());
		if (pfstream == NULL)
			return NULL;

		FileStream* pFileStream = new FileStream(pfstream);
		return pFileStream;
	}

}