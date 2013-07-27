#ifndef _FileStream_H_
#define _FileStream_H_

#include "Stream.h"


namespace ma
{

	class ENGINE_API FileStream : public Stream
	{
	public:
		friend class FileSystem;
	    
		~FileStream();
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

		static FileStream* create(const char* filePath, const char* mode);

	private:
		FileStream(FILE* file);

	private:
		FILE* _file;
		bool _canRead;
		bool _canWrite;
	};

}

#endif
