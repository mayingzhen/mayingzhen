#ifndef _Archive_h_
#define _Archive_h_


namespace ma
{
	struct FileInfo;

	typedef std::vector<FileInfo> VEC_FileInfo;
	typedef std::vector<std::string> VEC_STR;

	class Archive : public Referenced
	{
	public:
		Archive(const char* pszName)
		{
			mName = pszName;
			mReadOnly = true;
		}

		virtual ~Archive() {}

		/// Get the name of this archive
		const char* GetName() const{return mName.c_str();}

		/** Reports whether this Archive is read-only, or whether the contents
				can be updated. 
		*/
		virtual bool isReadOnly() const { return mReadOnly; }

		// ---------------------------------------------------------------------
		// Virtual Function
		// ---------------------------------------------------------------------
		/// Returns whether this archive is case sensitive in the way it matches files
		virtual bool isCaseSensitive(void) const = 0;

		// Open a stream on a given file. 
		virtual MemoryStream* open(const char* pszFile, bool readOnly = true) const = 0;

		// Create a new file (or overwrite one already there).
		virtual Stream* create(const char* pszFile) const = 0;
		
		// Delete a named file.
		virtual void remove(const char* pszFile) const = 0;

		// List all file names in the archive.
		virtual void list(OUT VEC_STR& ret, bool recursive = true, bool dirs = false) = 0;

		// List all files in the archive with accompanying information.
		virtual void listFileInfo(OUT VEC_FileInfo& vecFileInfo, bool recursive = true, bool dirs = false) = 0;

		// Find all file or directory names matching a given pattern in this archive.
		virtual void find(OUT VEC_STR& ret, const std::string& pattern, bool recursive = true, bool dirs = false) = 0;

		/** Find out if the named file exists (note: fully qualified filename required) */
		virtual bool exists(const std::string& filename) = 0; 

		/** Retrieve the modification time of a given file */
		virtual time_t getModifiedTime(const std::string& filename) = 0; 

		//Find all files or directories matching a given pattern in this
		//	archive and get some detailed information about them.
		virtual void findFileInfo(OUT VEC_FileInfo& vecFileInfo, const std::string& pattern, 
			bool recursive = true, bool dirs = false) = 0;

	protected:
		std::string		mName;

		bool			mReadOnly;
	};

}

#endif