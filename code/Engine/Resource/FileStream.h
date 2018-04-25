#ifndef _FileStream_H_
#define _FileStream_H_

#include "Stream.h"

namespace ma
{
	class FileStream : public Stream
	{
	public:
		FileStream(std::ifstream* s, bool freeOnClose = true);
		FileStream(const char* pszName, std::ifstream* s, bool freeOnClose = true);
		FileStream(std::ifstream* s, uint32_t nSize, bool freeOnClose = true);
		FileStream(const char* pszName, std::ifstream* s, uint32_t nSize, bool freeOnClose = true);
		FileStream(std::fstream* s, bool freeOnClose = true);
		FileStream(const char* pszName, std::fstream* s, bool freeOnClose = true);
		FileStream(std::fstream* s, uint32_t nSize, bool freeOnClose = true);
		FileStream(const char* pszName, std::fstream* s, uint32_t nSize, bool freeOnClose = true);

		~FileStream(void);

		// ---------------------------------------------------------------------
		// virtual function
		// ---------------------------------------------------------------------
		// Read the requisite number of bytes from the stream, stopping at the end of the file.
		virtual uint32_t Read(IN OUT void* pBuffer, uint32_t nCount);

		// Write the requisite number of bytes from the stream (only applicable to streams that are not read-only)
		virtual uint32_t Write(const void* pBuffer, uint32_t nCount);

		virtual uint32_t ReadLine(char* buf, uint32_t maxCount, const std::string& delim = "\n");

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
	private:
		void DetermineAccess();

	private:
		/// Reference to source stream (read)
		std::istream* mpInStream;

		/// Reference to source file stream (read-only)
		std::ifstream* mpFStreamRO;

		/// Reference to source file stream (read-write)
		std::fstream* mpFStream;

		bool m_bFreeOnClose;
	};
	

	RefPtr<FileStream> CreateFileStream(const char* pszFile);
}

#endif
