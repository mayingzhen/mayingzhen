#include "FileStream.h"

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


namespace ma
{


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


}
