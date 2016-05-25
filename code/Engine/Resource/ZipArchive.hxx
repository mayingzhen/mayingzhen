#include "ZipArchive.h"
#include "ZipDataStream.h"
#include "FileInfo.h"

#include "zzip/zzip.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#endif
//#include "ResourceBackgroundQueue.h"

namespace ma
{

	/// Utility method to format out zzip errors
	std::string getZzipErrorDescription(zzip_error_t zzipError) 
	{
		std::string errorMsg;
		switch (zzipError)
		{
		case ZZIP_NO_ERROR:
			break;
		case ZZIP_OUTOFMEM:
			errorMsg = "Out of memory.";
			break;            
		case ZZIP_DIR_OPEN:
		case ZZIP_DIR_STAT: 
		case ZZIP_DIR_SEEK:
		case ZZIP_DIR_READ:
			errorMsg = "Unable to read zip file.";
			break;            
		case ZZIP_UNSUPP_COMPR:
			errorMsg = "Unsupported compression format.";
			break;            
		case ZZIP_CORRUPTED:
			errorMsg = "Corrupted archive.";
			break;            
		default:
			errorMsg = "Unknown error.";
			break;            
		};

		return errorMsg;
	}

	ZipArchive::ZipArchive(const char* pszName)
	: Archive(pszName)
	, mZzipDir(NULL)
	{
		this->load();
	}

	ZipArchive::~ZipArchive(void)
	{
		this->unload();
	}

	// ---------------------------------------------------------------------
	// Virtual Function
	// ---------------------------------------------------------------------
	/// Returns whether this archive is case sensitive in the way it matches files
	bool ZipArchive::isCaseSensitive(void) const
	{
		return false;
	}

	// Open a stream on a given file. 
	Stream* ZipArchive::open(const char* pszFile, bool readOnly) const
	{
		std::string filename = StringUtil::replaceAll(pszFile, "\\", "/");

		// Format not used here (always binary)
		ZZIP_FILE* zzipFile = zzip_file_open(mZzipDir, filename.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
		if (!zzipFile)
		{
			int zerr = zzip_error(mZzipDir);
			std::string zzDesc = getZzipErrorDescription((zzip_error_t)zerr);
			return NULL;
		}

		// Get uncompressed size too
		ZZIP_STAT zstat;
		zzip_dir_stat(mZzipDir, filename.c_str(), &zstat, ZZIP_CASEINSENSITIVE);

		// Construct & return stream
		ZipDataStream* pZipDataSteam = new ZipDataStream(filename.c_str(), zzipFile, static_cast<size_t>(zstat.st_size));
		return pZipDataSteam;
	}

	// Create a new file (or overwrite one already there).
	Stream* ZipArchive::create(const char* pszFile) const
	{
		ASSERT(false);
		return NULL;
	}

	// Delete a named file.
	void ZipArchive::remove(const char* pszFile) const
	{
	}

	// List all file names in the archive.
	void ZipArchive::list(OUT VEC_STR& ret, bool recursive, bool dirs)
	{
		VEC_FileInfo::iterator i, iend;
		iend = mFileList.end();
		for (i = mFileList.begin(); i != iend; ++i)
			if ((dirs == (i->compressedSize == size_t (-1))) &&
				(recursive || i->path.empty()))
				ret.push_back(i->filename);
	}

	// List all files in the archive with accompanying information.
	void ZipArchive::listFileInfo(OUT VEC_FileInfo& vecFileInfo, bool recursive, bool dirs)
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

		VEC_FileInfo::const_iterator i, iend;
		iend = mFileList.end();
		for (i = mFileList.begin(); i != iend; ++i)
			if ((dirs == (i->compressedSize == size_t (-1))) &&
				(recursive || i->path.empty()))
				vecFileInfo.push_back(*i);
	}

	// Find all file or directory names matching a given pattern in this archive.
	void ZipArchive::find(OUT VEC_STR& ret, const std::string& pattern, bool recursive, bool dirs)
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

		// If pattern contains a directory name, do a full match
		bool full_match = (pattern.find ('/') != std::string::npos) ||
			(pattern.find ('\\') != std::string::npos);

		VEC_FileInfo::iterator i, iend;
		iend = mFileList.end();
		for (i = mFileList.begin(); i != iend; ++i)
			if ((dirs == (i->compressedSize == size_t (-1))) &&
				(recursive || full_match || i->path.empty()))
				// Check basename matches pattern (zip is case insensitive)
				if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
					ret.push_back(i->filename);
	}

	/** Find out if the named file exists (note: fully qualified filename required) */
	bool ZipArchive::exists(const std::string& filename)
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

		// zziplib is not threadsafe
		ZZIP_STAT zstat;
		int res = zzip_dir_stat(mZzipDir, filename.c_str(), &zstat, ZZIP_CASEINSENSITIVE);

		return (res == ZZIP_NO_ERROR);
	}


	/** Retrieve the modification time of a given file */
	time_t ZipArchive::getModifiedTime(const std::string& filename)
	{
		// Zziplib doesn't yet support getting the modification time of individual files
		// so just check the mod time of the zip itself
		struct stat tagStat;
		bool ret = (stat(mName.c_str(), &tagStat) == 0);

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
	void ZipArchive::findFileInfo(OUT VEC_FileInfo& ret, const std::string& pattern, 
							  bool recursive, bool dirs)
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

		// If pattern contains a directory name, do a full match
		bool full_match = (pattern.find ('/') != std::string::npos) ||
			(pattern.find ('\\') != std::string::npos);

		VEC_FileInfo::iterator i, iend;
		iend = mFileList.end();
		for (i = mFileList.begin(); i != iend; ++i)
			if ((dirs == (i->compressedSize == size_t (-1))) &&
				(recursive || full_match || i->path.empty()))
				// Check name matches pattern (zip is case insensitive)
				if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
					ret.push_back(*i);

	}

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	void ZipArchive::load()
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

			if (!mZzipDir)
			{
				zzip_error_t zzipError;
	#ifdef WIN32
				mZzipDir = zzip_dir_open(mName.c_str(), &zzipError);
	#else
				mZzipDir = zzip_dir_open_ext_io(mName.c_str(), &zzipError, 0, NULL);
	#endif
				checkZzipError(zzipError, "opening archive");
				if (mZzipDir == NULL)
				{
					//LogError("Failed to open zip:%s", mName.c_str());
				}

				// Cache names
				ZZIP_DIRENT zzipEntry;
				while (zzip_dir_read(mZzipDir, &zzipEntry))
				{
					FileInfo info;
					info.archive = this;
					// Get basename / path
					StringUtil::splitFilename(zzipEntry.d_name, info.basename, info.path);
					info.filename = zzipEntry.d_name;
					// Get sizes
					info.compressedSize = static_cast<size_t>(zzipEntry.d_csize);
					info.uncompressedSize = static_cast<size_t>(zzipEntry.st_size);
					// folder entries
					if (info.basename.empty())
					{
						info.filename = info.filename.substr (0, info.filename.length () - 1);
						StringUtil::splitFilename(info.filename, info.basename, info.path);
						// Set compressed size to -1 for folders; anyway nobody will check
						// the compressed size of a folder, and if he does, its useless anyway
						info.compressedSize = size_t (-1);
					}

					mFileList.push_back(info);

				}

			}
	}
	//-----------------------------------------------------------------------
	void ZipArchive::unload()
	{
		//MutexScope scope(CResourceBackgroundQueue::m_sIOMutex);

			if (mZzipDir)
			{
				zzip_dir_close(mZzipDir);
				mZzipDir = 0;
				mFileList.clear();
			}

	}



	void ZipArchive::checkZzipError(int zzipError, const std::string& operation) const
	{
		if (zzipError != ZZIP_NO_ERROR)
		{
			std::string errorMsg = getZzipErrorDescription(static_cast<zzip_error_t>(zzipError));

			ASSERT(false);
	// 		OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
	// 			mName + " - error whilst " + operation + ": " + errorMsg,
	// 			"ZipArchive::checkZzipError");
		}
	}

	RefPtr<Archive> CreateZipArchive( const char* pszFile )
	{
		return new ZipArchive(pszFile);
	}

}
