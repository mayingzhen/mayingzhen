#ifndef _AAssetFileStream_H_
#define _AAssetFileStream_H_

#include "Stream.h"

class AAsset;

namespace ma
{
	// Android AAsset

	class AAssetFileStream : public Stream 
	{
	public:
		friend class FileSystem;
	    
		~AAssetFileStream();

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

		static AAssetFileStream*	create(const char* filePath, const char* mode);

		static void					setAAssetManager(AAssetManager* pAAssetManager);

		static AAssetManager*		getAAssetManager();

	private:
		AAssetFileStream(AAsset* asset);

	private:
		AAsset* _asset;
	};
}

#endif //_FileStreamAndroid_H_



