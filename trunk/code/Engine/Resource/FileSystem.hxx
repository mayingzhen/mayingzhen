#include "FileSystem.h"
#include "Stream.h"

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
	static std::string __resourcePath("./");

	#ifdef __ANDROID__
	#include <unistd.h>

	static void makepath(std::string path, int mode)
	{
		std::vector<std::string> dirs;
		while (path.length() > 0)
		{
			int index = path.find('/');
			std::string dir = (index == -1 ) ? path : path.substr(0, index);
			if (dir.length() > 0)
				dirs.push_back(dir);
	        
			if (index + 1 >= path.length() || index == -1)
				break;
	            
			path = path.substr(index + 1);
		}
	    
		struct stat s;
		std::string dirPath;
		for (unsigned int i = 0; i < dirs.size(); i++)
		{
			dirPath += "/";
			dirPath += dirs[i];
			if (stat(dirPath.c_str(), &s) != 0)
			{
				// Directory does not exist.
				if (mkdir(dirPath.c_str(), 0777) != 0)
				{
					GP_ERROR("Failed to create directory: '%s'", dirPath.c_str());
					return;
				}
			}
		}
	    
		return;
	}

	/**
	 * Returns true if the file exists in the android read-only asset directory.
	 */
	static bool androidFileExists(const char* filePath)
	{
		AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
		if (asset)
		{
			int lenght = AAsset_getLength(asset);
			AAsset_close(asset);
			return length > 0;
		}
		return false;
	}

	#endif

	





	/////////////////////////////

	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}

	void FileSystem::setResourcePath(const char* path)
	{
		__resourcePath = path == NULL ? "" : path;
	}

	const char* FileSystem::getResourcePath()
	{
		return __resourcePath.c_str();
	}

	bool FileSystem::listFiles(const char* dirPath, std::vector<std::string>& files)
	{
	#ifdef WIN32
		std::string path(__resourcePath);
		if (dirPath && strlen(dirPath) > 0)
		{
			path.append(dirPath);
		}
		path.append("/*");
		// Convert char to wchar
		std::basic_string<TCHAR> wPath;
		wPath.assign(path.begin(), path.end());

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(wPath.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
		{
			return false;
		}
		do
		{
			// Add to the list if this is not a directory
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				// Convert wchar to char
				std::basic_string<TCHAR> wfilename(FindFileData.cFileName);
				std::string filename;
				filename.assign(wfilename.begin(), wfilename.end());
				files.push_back(filename);
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
		return true;
	#else
		std::string path(__resourcePath);
		if (dirPath && strlen(dirPath) > 0)
		{
			path.append(dirPath);
		}
		path.append("/.");
		bool result = false;

		struct dirent* dp;
		DIR* dir = opendir(path.c_str());
		if (dir != NULL)
		{
			while ((dp = readdir(dir)) != NULL)
			{
				std::string filepath(path);
				filepath.append("/");
				filepath.append(dp->d_name);

				struct stat buf;
				if (!stat(filepath.c_str(), &buf))
				{
					// Add to the list if this is not a directory
					if (!S_ISDIR(buf.st_mode))
					{
						files.push_back(dp->d_name);
					}
				}
			}
			closedir(dir);
			result = true;
		}

	#ifdef __ANDROID__
		// List the files that are in the android APK at this path
		AAssetDir* assetDir = AAssetManager_openDir(__assetManager, dirPath);
		if (assetDir != NULL)
		{
			AAssetDir_rewind(assetDir);
			const char* file = NULL;
			while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
			{
				std::string filename(file);
				// Check if this file was already added to the list because it was copied to the SD card.
				if (find(files.begin(), files.end(), filename) == files.end())
				{
					files.push_back(filename);
				}
			}
			AAssetDir_close(assetDir);
			result = true;
		}
	#endif

		return result;
	#endif
	}

	std::string FileSystem::getFullPath(const char* path)
	{
		std::string fullPath;
		if (FileSystem::isAbsolutePath(path))
		{
			fullPath.assign(path);
		}
		else
		{
			fullPath.assign(__resourcePath);
			fullPath += path;
		}

		return fullPath;
	}

	bool FileSystem::fileExists(const char* filePath)
	{
		ASSERT(filePath);

	#ifdef __ANDROID__
		if (androidFileExists(filePath))
		{
			return true;
		}
	#endif

		std::string fullPath = getFullPath(filePath);

		gp_stat_struct s;

	#ifdef WIN32
		if (!isAbsolutePath(filePath) && stat(fullPath.c_str(), &s) != 0)
		{
			fullPath = __resourcePath;
			fullPath += filePath;
	        
			int result = stat(fullPath.c_str(), &s);
			if (result != 0)
			{
				fullPath = __resourcePath;
				fullPath += filePath;
				return stat(fullPath.c_str(), &s) == 0;
			}
		}
		return true;
	#else
		return stat(fullPath.c_str(), &s) == 0;
	#endif
	}

	Stream* FileSystem::open(const char* path, size_t mode)
	{
		char modeStr[] = "rb";
		if ((mode & WRITE) != 0)
			modeStr[0] = 'w';

	#ifdef __ANDROID__
		if ((mode & WRITE) != 0)
		{
			// Open a file on the SD card
			std::string fullPath(__resourcePath);
			fullPath += path;

			size_t index = fullPath.rfind('/');
			if (index != std::string::npos)
			{
				std::string directoryPath = fullPath.substr(0, index);
				struct stat s;
				if (stat(directoryPath.c_str(), &s) != 0)
					makepath(directoryPath, 0777);
			}
			return FileStream::create(fullPath.c_str(), modeStr);
		}
		else
		{
			// Open a file in the read-only asset directory
			return FileStreamAndroid::create(path, modeStr);
		}
	#else
		std::string fullPath = getFullPath(path);
	     
		FileStream* stream = FileStream::create(fullPath.c_str(), modeStr);
		return stream;
	#endif
	}

	FILE* FileSystem::openFile(const char* filePath, const char* mode)
	{
		ASSERT(filePath);
		ASSERT(mode);

		std::string fullPath = getFullPath(filePath);

		createFileFromAsset(filePath);
	    
		FILE* fp = fopen(fullPath.c_str(), mode);
	    
		return fp;
	}

// 	char* FileSystem::readAll(const char* filePath, int* fileSize)
// 	{
// 		ASSERT(filePath);
// 
// 		// Open file for reading.
// 		std::auto_ptr<Stream> stream(open(filePath));
// 		if (stream.get() == NULL)
// 		{
// 			GP_ERROR("Failed to load file: %s", filePath);
// 			return NULL;
// 		}
// 		size_t size = stream->length();
// 
// 		// Read entire file contents.
// 		char* buffer = new char[size + 1];
// 		size_t read = stream->read(buffer, 1, size);
// 		if (read != size)
// 		{
// 			GP_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %u < %u).", filePath, read, size);
// 			SAFE_DELETE_ARRAY(buffer);
// 			return NULL;
// 		}
// 
// 		// Force the character buffer to be NULL-terminated.
// 		buffer[size] = '\0';
// 
// 		if (fileSize)
// 		{
// 			*fileSize = (int)size; 
// 		}
// 		return buffer;
// 	}

	DataStream*	FileSystem::readAll(const char* filePath)
	{
		ASSERT(filePath);

		// Open file for reading.
		std::auto_ptr<Stream> stream(open(filePath));
		if (stream.get() == NULL)
		{
			GP_ERROR("Failed to load file: %s", filePath);
			return NULL;
		}
		size_t size = stream->length();

		// Read entire file contents.
		Byte* buffer = new Byte[size + 1];
		size_t read = stream->read(buffer, 1, size);
		if (read != size)
		{
			//GP_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %u < %u).", filePath, read, size);
			SAFE_DELETE_ARRAY(buffer);
			return NULL;
		}

		// Force the character buffer to be NULL-terminated.
		buffer[size] = '\0';

		DataStream* pDataSteam = new DataStream(buffer,size);
		
		return pDataSteam;
			
	}

	bool FileSystem::isAbsolutePath(const char* filePath)
	{
		if (filePath == 0 || filePath[0] == '\0')
			return false;
	#ifdef WIN32
		if (filePath[1] != '\0')
		{
			char first = filePath[0];
			return (filePath[1] == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')));
		}
		return false;
	#else
		return filePath[0] == '/';
	#endif
	}

	void FileSystem::createFileFromAsset(const char* path)
	{
	#ifdef __ANDROID__
		static std::set<std::string> upToDateAssets;

		ASSERT(path);
		std::string fullPath(__resourcePath);
		std::string resolvedPath = path;
		fullPath += resolvedPath;

		std::string directoryPath = fullPath.substr(0, fullPath.rfind('/'));
		struct stat s;
		if (stat(directoryPath.c_str(), &s) != 0)
			makepath(directoryPath, 0777);

		// To ensure that the files on the file system corresponding to the assets in the APK bundle
		// are always up to date (and in sync), we copy them from the APK to the file system once
		// for each time the process (game) runs.
		if (upToDateAssets.find(fullPath) == upToDateAssets.end())
		{
			AAsset* asset = AAssetManager_open(__assetManager, resolvedPath.c_str(), AASSET_MODE_RANDOM);
			if (asset)
			{
				const void* data = AAsset_getBuffer(asset);
				int length = AAsset_getLength(asset);
				FILE* file = fopen(fullPath.c_str(), "wb");
				if (file != NULL)
				{
					int ret = fwrite(data, sizeof(unsigned char), length, file);
					if (fclose(file) != 0)
					{
						GP_ERROR("Failed to close file on file system created from APK asset '%s'.", path);
						return;
					}
					if (ret != length)
					{
						GP_ERROR("Failed to write all data from APK asset '%s' to file on file system.", path);
						return;
					}
				}
				else
				{
					GP_ERROR("Failed to create file on file system from APK asset '%s'.", path);
					return;
				}

				upToDateAssets.insert(fullPath);
			}
		}
	#endif
	}

	std::string FileSystem::getDirectoryName(const char* path)
	{
		if (path == NULL || strlen(path) == 0)
		{
			return "";
		}
	#ifdef WIN32
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath(path, drive, dir, NULL, NULL);
		std::string dirname;
		size_t driveLength = strlen(drive);
		if (driveLength > 0)
		{
			dirname.reserve(driveLength + strlen(dir));
			dirname.append(drive);
			dirname.append(dir);
		}
		else
		{
			dirname.assign(dir);
		}
		std::replace(dirname.begin(), dirname.end(), '\\', '/');
		return dirname;
	#else
		// dirname() modifies the input string so create a temp string
		std::string dirname;
		char* tempPath = new char[strlen(path) + 1];
		strcpy(tempPath, path);
		char* dir = ::dirname(tempPath);
		if (dir && strlen(dir) > 0)
		{
			dirname.assign(dir);
			// dirname() strips off the trailing '/' so add it back to be consistent with Windows
			dirname.append("/");
		}
		SAFE_DELETE_ARRAY(tempPath);
		return dirname;
	#endif
	}

	std::string FileSystem::getExtension(const char* path)
	{
		const char* str = strrchr(path, '.');
		if (str == NULL)
			return "";

		std::string ext;
		size_t len = strlen(str);
		for (size_t i = 1; i < len; ++i)
			ext += tolower(str[i]);

		return ext;
	}


}
