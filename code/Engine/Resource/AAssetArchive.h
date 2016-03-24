#ifndef _AAssetArchive_h_
#define _AAssetArchive_h_

#include "Archive.h"

namespace ma
{
	class AAssetArchive : public Archive
	{
	public:
		AAssetArchive(const char* pszName);

		~AAssetArchive();

		virtual bool isCaseSensitive(void) const;

		// Open a stream on a given file. 
		virtual MemoryStream* open(const char* pszFile, bool readOnly = true) const;

		// Create a new file (or overwrite one already there).
		virtual Stream* create(const char* pszFile) const;

		// Delete a named file.
		virtual void remove(const char* pszFile) const;

		// List all file names in the archive.
		virtual void list(OUT VEC_STR& ret, bool recursive = true, bool dirs = false);

		// List all files in the archive with accompanying information.
		virtual void listFileInfo(OUT VEC_FileInfo& vecFileInfo, bool recursive = true, bool dirs = false);

		// Find all file or directory names matching a given pattern in this archive.
		virtual void find(OUT VEC_STR& ret, const std::string& pattern, bool recursive = true, bool dirs = false);

		/** Find out if the named file exists (note: fully qualified filename required) */
		virtual bool exists(const std::string& filename); 

		/** Retrieve the modification time of a given file */
		virtual time_t getModifiedTime(const std::string& filename); 

		//Find all files or directories matching a given pattern in this
		//	archive and get some detailed information about them.
		virtual void findFileInfo(OUT VEC_FileInfo& vecFileInfo, const std::string& pattern, 
			bool recursive = true, bool dirs = false);
	};

}

#endif