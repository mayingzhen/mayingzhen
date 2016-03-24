#ifndef _FileArchive_h_
#define _FileArchive_h_

#include "Archive.h"

namespace ma
{
	class FileArchive : public Archive
	{
	public:
		FileArchive(const char* pszName);

		~FileArchive(void);

		// ---------------------------------------------------------------------
		// Virtual Function
		// ---------------------------------------------------------------------
		/// Returns whether this archive is case sensitive in the way it matches files
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

		// ---------------------------------------------------------------------
		// Self
		// ---------------------------------------------------------------------
	public:
		/// Set whether filesystem enumeration will include hidden files or not.
		/// This should be called prior to declaring and/or initializing filesystem
		/// resource locations. The default is true (ignore hidden files).
		static void setIgnoreHidden(bool ignore)
		{
			ms_IgnoreHidden = ignore;
		}

		/// Get whether hidden files are ignored during filesystem enumeration.
		static bool getIgnoreHidden()
		{
			return ms_IgnoreHidden;
		}

	private:
		/** Utility method to retrieve all files in a directory matching pattern.
			@param pattern File pattern
			@param recursive Whether to cascade down directories
			@param dirs Set to true if you want the directories to be listed
				instead of files
			@param simpleList Populated if retrieving a simple list
			@param detailList Populated if retrieving a detailed list
			@param currentDir The current directory relative to the base of the 
				archive, for file naming
			*/
			void findFiles(const std::string& pattern, bool recursive, bool dirs,
				VEC_STR* simpleList, VEC_FileInfo* detailList);

	private:
		static bool ms_IgnoreHidden;
	};

	 RefPtr<Archive> CreateFileArchive(const char* pszFile);
}

#endif