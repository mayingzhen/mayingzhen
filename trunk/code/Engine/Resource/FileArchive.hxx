#include "FileArchive.h"
#include "FileStream.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <io.h>
#endif

namespace ma
{

	bool FileArchive::ms_IgnoreHidden = true;

	//-----------------------------------------------------------------------
	static bool is_reserved_dir (const char *fn)
	{
		return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
	}

	static bool is_absolute_path(const char* path)
	{
		return path[0] == '/' || path[0] == '\\';
	}

	static std::string concatenate_path(const std::string& base, const std::string& name)
	{
		std::string ret;
		if (base.empty() || is_absolute_path(name.c_str()))
			ret = name;
		else
			ret = base + '/' + name;

		ret = StringUtil::replaceAll(ret, "\\", "/");

		return ret;
	}
	//-----------------------------------------------------------------------

	FileArchive::FileArchive(const char* pszName)
	: Archive(pszName)
	{
		// test to see if this folder is writeable
		std::string testPath = concatenate_path(mName, "__testwrite.ma");
		std::ofstream writeStream;
		writeStream.open(testPath.c_str());
		if (writeStream.fail())
			mReadOnly = true;
		else
		{
			mReadOnly = false;
			writeStream.close();
			::remove(testPath.c_str());
		}
	}

	FileArchive::~FileArchive(void)
	{
	}

	// ---------------------------------------------------------------------
	// Function
	// ---------------------------------------------------------------------
	/// Returns whether this archive is case sensitive in the way it matches files
	bool FileArchive::isCaseSensitive(void) const
	{
		return false;
	}

	// Open a stream on a given file. 
	Stream* FileArchive::open(const std::string& filename, bool readOnly) const
	{
		std::string full_path = concatenate_path(mName, filename);
		StringUtil::toLowerCase(full_path);

		// Use filesystem to determine size 
		// (quicker than streaming to the end and back)
		struct stat tagStat;
		int ret = stat(full_path.c_str(), &tagStat);
		if (ret != 0)
		{
			return NULL;
		}

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::in | std::ios::binary;
		std::istream* baseStream = 0;
		std::ifstream* roStream = 0;
		std::fstream* rwStream = 0;

		if (!readOnly && isReadOnly())
		{
			mode |= std::ios::out;
			rwStream = new std::fstream;
			rwStream->open(full_path.c_str(), mode);
			baseStream = rwStream;
		}
		else
		{
			roStream = new std::ifstream;
			roStream->open(full_path.c_str(), mode);
			baseStream = roStream;
		}


		// Should check ensure open succeeded, in case fail for some reason.
		if (baseStream->fail())
		{
			SAFE_DELETE(rwStream);
			SAFE_DELETE(roStream);
			return NULL;
		}

		/// Construct return stream, tell it to delete on destroy
		FileStream* stream = 0;
		if (rwStream)
		{
			// use the writeable stream 
			stream = new FileStream(filename.c_str(),
				rwStream, (size_t)tagStat.st_size, true);
		}
		else
		{
			// read-only stream
			stream = new FileStream(filename.c_str(),
				roStream, (size_t)tagStat.st_size, true);
		}
		return (Stream*)(stream);
	}

	// Create a new file (or overwrite one already there).
	Stream* FileArchive::create(const std::string& filename) const
	{
		if (isReadOnly())
		{
			ASSERT(false);
			return NULL;
		}

		std::string full_path = concatenate_path(mName, filename);

		// Always open in binary mode
		// Also, always include reading
		std::ios::openmode mode = std::ios::out | std::ios::binary;
		std::fstream* rwStream = new std::fstream;
		rwStream->open(full_path.c_str(), mode);

		// Should check ensure open succeeded, in case fail for some reason.
		if (rwStream->fail())
		{
			SAFE_DELETE(rwStream);
			return NULL;
		}

		/// Construct return stream, tell it to delete on destroy
		FileStream* stream = new FileStream(filename.c_str(),
			rwStream, 0, true);

		return (Stream*)(stream);
	}

	// Delete a named file.
	void FileArchive::remove(const std::string& filename) const
	{
		if (isReadOnly())
		{
			ASSERT(false);
			return;
		}

		std::string full_path = concatenate_path(mName, filename);
		::remove(full_path.c_str());
	}

	// List all file names in the archive.
	void FileArchive::list(OUT VEC_STR& ret, bool recursive, bool dirs)
	{
		// directory change requires locking due to saved returns
		findFiles("*", recursive, dirs, &ret, 0);
	}

	// List all files in the archive with accompanying information.
	void FileArchive::listFileInfo(OUT VEC_FileInfo& vecFileInfo, bool recursive, bool dirs)
	{
		findFiles("*", recursive, dirs, 0, &vecFileInfo);
	}

	// Find all file or directory names matching a given pattern in this archive.
	void FileArchive::find(OUT VEC_STR& ret, const std::string& pattern, bool recursive , bool dirs)
	{
		findFiles(pattern, recursive, dirs, &ret, 0);
	}

	/** Find out if the named file exists (note: fully qualified filename required) */
	bool FileArchive::exists(const std::string& filename)
	{
		std::string full_path = concatenate_path(mName, filename);

		struct stat tagStat;
		bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		// stat will return true if the filename is absolute, but we need to check
		// the file is actually in this archive
		if (ret && is_absolute_path(filename.c_str()))
		{
			// only valid if full path starts with our base
			if(!this->isCaseSensitive())
			{
				// case insensitive on windows
				std::string lowerCaseName = mName;
				StringUtil::toLowerCase(lowerCaseName);
				ret = StringUtil::startsWith(full_path, lowerCaseName, true);
			}
			else
			{
				// case sensitive
				ret = StringUtil::startsWith(full_path, mName, false);
			}
		}

		return ret;
	}

	/** Retrieve the modification time of a given file */
	time_t FileArchive::getModifiedTime(const std::string& filename)
	{
		std::string full_path = concatenate_path(mName, filename);

		struct stat tagStat;
		bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		if (ret)
		{
			return tagStat.st_mtime;
		}
		else
		{
			return 0;
		}
	}

	//Find all files or directories matching a given pattern in this
	//	archive and get some detailed information about them.
	void FileArchive::findFileInfo(OUT VEC_FileInfo& vecFileInfo, const std::string& pattern, 
							  bool recursive, bool dirs)
	{
		findFiles(pattern, recursive, dirs, 0, &vecFileInfo);
	}

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	void FileArchive::findFiles( const std::string& pattern, bool recursive, bool dirs,
									   VEC_STR* simpleList, VEC_FileInfo* detailList )
	{
		ASSERT(false);
	}


	ref_ptr<Archive> CreateFileArchive( const char* pszFile )
	{
		return new FileArchive(pszFile);
	}

}