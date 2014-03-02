#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_


class AAssetManager;

namespace ma
{


	class ENGINE_API FileSystem
	{
	public:

		enum StreamMode
		{
			READ = 1,
			WRITE = 2
		};

		~FileSystem();

#ifdef __ANDROID__
		static void				setAAssetManager(AAssetManager* pAAssetManager);	
		static AAssetManager*	getAAssetManager();
#endif

		static void			setResourcePath(const char* path);

		static const char*	getResourcePath();

		static std::string	getFullPath(const char* path);

		static bool			listFiles(const char* dirPath, std::vector<std::string>& files);

		static bool			fileExists(const char* filePath);

		static Stream*		open(const char* path, size_t mode = READ);

		static FILE*		openFile(const char* filePath, const char* mode);

		//static char*		readAll(const char* filePath, int* fileSize = NULL);

		static DataStream*	readAll(const char* filePath);

		static bool			isAbsolutePath(const char* filePath);

		static void			createFileFromAsset(const char* path);

		/**
		 * Returns the directory name up to and including the trailing '/'.
		 * 
		 * This is a lexical method so it does not verify that the directory exists.
		 * Back slashes will be converted to forward slashes.
		 * 
		 * - "res/image.png" will return "res/"
		 * - "image.png" will return ""
		 * - "c:\foo\bar\image.png" will return "c:/foo/bar/"
		 * 
		 * @param The file path. May be relative or absolute, forward or back slashes. May be NULL.
		 * 
		 * @return The directory name with the trailing '/'. Returns "" if path is NULL or the path does not contain a directory.
		 */
		static std::string	getDirectoryName(const char* path);

		/**
		 * Returns the extension of the given file path.
		 *
		 * The extension returned includes all character after and including the last '.'
		 * in the file path. The extension is returned as all uppercase.
		 *
		 * If the path does not contain an extension, an empty string is returned.
		 * 
		 * @param path File path.
		 *
		 * @return The file extension, all lowercase, including the '.'.
		 */
		static std::string	getExtension(const char* path);

		static std::string  ReplaceFileExt(const std::string& strFile,const std::string& strNewExt);

	private:

		FileSystem();
	};

}

#endif
