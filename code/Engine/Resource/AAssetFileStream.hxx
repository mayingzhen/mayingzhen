#include "AAssetFileStream.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <android/asset_manager.h>
#include <unistd.h>

// #ifdef WIN32
// #include <windows.h>
// #include <tchar.h>
// #include <stdio.h>
// #include <direct.h>
// #define gp_stat _stat
// #define gp_stat_struct struct stat
// #else
// #define __EXT_POSIX2
// #include <libgen.h>
// #include <dirent.h>
// #define gp_stat stat
// #define gp_stat_struct struct stat
// #endif


namespace ma
{

	AAssetManager* __assetManager;

	void AAssetFileStream::setAAssetManager(AAssetManager* pAAssetManager)
	{
		__assetManager = pAAssetManager;
	}

	AAssetManager*	AAssetFileStream::getAAssetManager()
	{
		return __assetManager;
	}

		/**
	 * Returns true if the file exists in the android read-only asset directory.
	 */
	static bool androidFileExists(const char* filePath)
	{
		AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
		if (asset)
		{
			int length = AAsset_getLength(asset);
			AAsset_close(asset);
			return length > 0;
		}
		return false;
	}


	////////////////////////////////

	AAssetFileStream::AAssetFileStream(AAsset* asset)
		: _asset(asset)
	{
	}

	AAssetFileStream::~AAssetFileStream()
	{
		if (_asset)
			close();
	}

	AAssetFileStream* AAssetFileStream::create(const char* filePath, const char* mode)
	{
		AAsset* asset = AAssetManager_open(AAssetFileStream::getAAssetManager(), filePath, AASSET_MODE_RANDOM);
		if (asset)
		{
			AAssetFileStream* stream = new AAssetFileStream(asset);
			return stream;
		}
		return NULL;
	}

	bool AAssetFileStream::canRead()
	{
		return true;
	}

	bool AAssetFileStream::canWrite()
	{
		return false;
	}

	bool AAssetFileStream::canSeek()
	{
		return true;
	}

	void AAssetFileStream::close()
	{
		if (_asset)
			AAsset_close(_asset);
		_asset = NULL;
	}

	size_t AAssetFileStream::read(void* ptr, size_t size, size_t count)
	{
		int result = AAsset_read(_asset, ptr, size * count);
		return result > 0 ? ((size_t)result) / size : 0;
	}

	char* AAssetFileStream::readLine(char* str, int num)
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

	size_t AAssetFileStream::write(const void* ptr, size_t size, size_t count)
	{
		return 0;
	}

	bool AAssetFileStream::eof()
	{
		return position() >= length();
	}

	size_t AAssetFileStream::length()
	{
		return (size_t)AAsset_getLength(_asset);
	}

	long int AAssetFileStream::position()
	{
		return AAsset_getLength(_asset) - AAsset_getRemainingLength(_asset);
	}

	bool AAssetFileStream::seek(long int offset, int origin)
	{
		return AAsset_seek(_asset, offset, origin) != -1;
	}

	bool AAssetFileStream::rewind()
	{
		if (canSeek())
		{
			return AAsset_seek(_asset, 0, SEEK_SET) != -1;
		}
		return false;
	}

}
