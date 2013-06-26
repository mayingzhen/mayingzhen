#include "Stream.h"
#include "FileSystem.h"


#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    #include <direct.h>
    #define gp_stat _stat
    #define gp_stat_struct struct stat
#else
    #define __EXT_POSIX2
    #include <libgen.h>
    #include <dirent.h>
    #define gp_stat stat
    #define gp_stat_struct struct stat
#endif

#ifdef __ANDROID__
#include <android/asset_manager.h>
extern AAssetManager* __assetManager;
#endif

namespace ma
{

	class FileStream : public Stream
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


#ifdef __ANDROID__
	class FileStreamAndroid : public Stream
	{
	public:
		friend class FileSystem;
	    
		~FileStreamAndroid();
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

		static FileStreamAndroid* create(const char* filePath, const char* mode);

	private:
		FileStreamAndroid(AAsset* asset);

	private:
		AAsset* _asset;
	};
#endif


	FileStream::FileStream(FILE* file)
		: _file(file), _canRead(false), _canWrite(false)
	{
	    
	}

	FileStream::~FileStream()
	{
		if (_file)
		{
			close();
		}
	}

	FileStream* FileStream::create(const char* filePath, const char* mode)
	{
		FILE* file = fopen(filePath, mode);
		if (file)
		{
			FileStream* stream = new FileStream(file);
			const char* s = mode;
			while (s != NULL && *s != '\0')
			{
				if (*s == 'r')
					stream->_canRead = true;
				else if (*s == 'w')
					stream->_canWrite = true;
				++s;
			}

			return stream;
		}
		return NULL;
	}

	bool FileStream::canRead()
	{
		return _file && _canRead;
	}

	bool FileStream::canWrite()
	{
		return _file && _canWrite;
	}

	bool FileStream::canSeek()
	{
		return _file != NULL;
	}

	void FileStream::close()
	{
		if (_file)
			fclose(_file);
		_file = NULL;
	}

	size_t FileStream::read(void* ptr, size_t size, size_t count)
	{
		if (!_file)
			return 0;
		return fread(ptr, size, count, _file);
	}

	char* FileStream::readLine(char* str, int num)
	{
		if (!_file)
			return 0;
		return fgets(str, num, _file);
	}

	size_t FileStream::write(const void* ptr, size_t size, size_t count)
	{
		if (!_file)
			return 0;
		return fwrite(ptr, size, count, _file);
	}

	bool FileStream::eof()
	{
		if (!_file || feof(_file))
			return true;
		return ((size_t)position()) >= length();
	}

	size_t FileStream::length()
	{
		size_t len = 0;
		if (canSeek())
		{
			long int pos = position();
			if (seek(0, SEEK_END))
			{
				len = position();
			}
			seek(pos, SEEK_SET);
		}
		return len;
	}

	long int FileStream::position()
	{
		if (!_file)
			return -1;
		return ftell(_file);
	}

	bool FileStream::seek(long int offset, int origin)
	{
		if (!_file)
			return false;
		return fseek(_file, offset, origin) == 0;
	}

	bool FileStream::rewind()
	{
		if (canSeek())
		{
			::rewind(_file);
			return true;
		}
		return false;
	}

	////////////////////////////////
#ifdef __ANDROID__

	FileStreamAndroid::FileStreamAndroid(AAsset* asset)
		: _asset(asset)
	{
	}

	FileStreamAndroid::~FileStreamAndroid()
	{
		if (_asset)
			close();
	}

	FileStreamAndroid* FileStreamAndroid::create(const char* filePath, const char* mode)
	{
		AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
		if (asset)
		{
			FileStreamAndroid* stream = new FileStreamAndroid(asset);
			return stream;
		}
		return NULL;
	}

	bool FileStreamAndroid::canRead()
	{
		return true;
	}

	bool FileStreamAndroid::canWrite()
	{
		return false;
	}

	bool FileStreamAndroid::canSeek()
	{
		return true;
	}

	void FileStreamAndroid::close()
	{
		if (_asset)
			AAsset_close(_asset);
		_asset = NULL;
	}

	size_t FileStreamAndroid::read(void* ptr, size_t size, size_t count)
	{
		int result = AAsset_read(_asset, ptr, size * count);
		return result > 0 ? ((size_t)result) / size : 0;
	}

	char* FileStreamAndroid::readLine(char* str, int num)
	{
		if (num <= 0)
			return NULL;
		char c = 0;
		size_t maxCharsToRead = num - 1;
		for (size_t i = 0; i < maxCharsToRead; ++i)
		{
			size_t result = read(&c, 1, 1);
			if (result != 1)
			{
				str[i] = '\0';
				break;
			}
			if (c == '\n')
			{
				str[i] = c;
				str[i + 1] = '\0';
				break;
			}
			else if(c == '\r')
			{
				str[i] = c;
				// next may be '\n'
				size_t pos = position();

				char nextChar = 0;
				if (read(&nextChar, 1, 1) != 1)
				{
					// no more characters
					str[i + 1] = '\0';
					break;
				}
				if (nextChar == '\n')
				{
					if (i == maxCharsToRead - 1)
					{
						str[i + 1] = '\0';
						break;
					}
					else
					{
						str[i + 1] = nextChar;
						str[i + 2] = '\0';
						break;
					}
				}
				else
				{
					seek(pos, SEEK_SET);
					str[i + 1] = '\0';
					break;
				}
			}
			str[i] = c;
		}
		return str; // what if first read failed?
	}

	size_t FileStreamAndroid::write(const void* ptr, size_t size, size_t count)
	{
		return 0;
	}

	bool FileStreamAndroid::eof()
	{
		return position() >= length();
	}

	size_t FileStreamAndroid::length()
	{
		return (size_t)AAsset_getLength(_asset);
	}

	long int FileStreamAndroid::position()
	{
		return AAsset_getLength(_asset) - AAsset_getRemainingLength(_asset);
	}

	bool FileStreamAndroid::seek(long int offset, int origin)
	{
		return AAsset_seek(_asset, offset, origin) != -1;
	}

	bool FileStreamAndroid::rewind()
	{
		if (canSeek())
		{
			return AAsset_seek(_asset, 0, SEEK_SET) != -1;
		}
		return false;
	}

#endif

}
