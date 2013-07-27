#ifndef _DataStream_H_
#define _DataStream_H_

#include "Stream.h"

namespace ma
{

	class ENGINE_API DataStream : public Stream
	{
	public:
		friend class FileSystem;

		DataStream(Byte* pMem, size_t size, const char* pFilePath = NULL);

		DataStream(size_t size);
	    
		~DataStream();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool canSeek();
		virtual void close();
		virtual size_t read(void* ptr, size_t size, size_t count);
		virtual char* readLine(char* str, int num);
		virtual size_t write(const void* ptr, size_t size, size_t count);
		virtual bool eof();
		virtual size_t length();
		virtual long int position();
		virtual bool seek(long int offset, int origin);
		virtual bool rewind();

		Byte*			GetData() {return m_pData;}
		size_t			GetSize() {return m_size;}
		const char*		GetFilePath() {return m_sFilePath.c_str();}

		//static DataStream* create(const char* filePath, const char* mode);	

	private:
		size_t	m_size;
		Byte*	m_pData;
		Byte*	m_pPos;
		Byte*	m_pEnd;

		bool _canRead;
		bool _canWrite;

		std::string	m_sFilePath;
	};

}

#endif
