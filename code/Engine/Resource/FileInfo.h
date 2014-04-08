#ifndef _FileInfo_h_
#define _FileInfo_h_

namespace ma
{
	class Archive;

	struct FileInfo 
	{
		Archive*		archive;    /// The archive in which the file has been found 

		std::string		filename;	/// The file's fully qualified name

		std::string		path;		/// Path name; separated by '/' and ending with '/'

		std::string		basename;	/// Base filename

		UINT			compressedSize;

		UINT			uncompressedSize;
	};

}

#endif
