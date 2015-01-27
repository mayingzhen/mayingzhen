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
		FileStream(std::ifstream* s, UINT nSize, bool freeOnClose = true);
		FileStream(const char* pszName, std::ifstream* s, UINT nSize, bool freeOnClose = true);
		FileStream(std::fstream* s, bool freeOnClose = true);
		FileStream(const char* pszName, std::fstream* s, bool freeOnClose = true);
		FileStream(std::fstream* s, UINT nSize, bool freeOnClose = true);
		FileStream(const char* pszName, std::fstream* s, UINT nSize, bool freeOnClose = true);

		~FileStream(void);

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

}

#endif
