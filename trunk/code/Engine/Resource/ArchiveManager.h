#ifndef _ArchiveManager_H_
#define _ArchiveManager_H_

#include "Archive.h"

namespace ma
{
	class ENGINE_API ArchiveManager
	{
	public:
		ArchiveManager();

		~ArchiveManager();

		StreamPtr			Open(const char* pszFile, bool readOnly = true) const;

		StreamPtr			Create(const char* pszFile) const;

		MemoryStreamPtr		ReadAll(const char* pszFile,UINT nExtenSiz = 0) const;

		void				AddArchive(Archive* pArchive);

		bool				Exists(const char* pszFile) const;

		std::string			GetFullPath(const char* pszFile) const;

	private:
		typedef std::vector< RefPtr<Archive> > VEC_ARCHIVE;

		VEC_ARCHIVE		m_vecArchive;
	};

	ENGINE_API void SetArchiveManager(ArchiveManager* pArchiveManager);

	ENGINE_API ArchiveManager* GetArchiveMananger();

}

#endif
