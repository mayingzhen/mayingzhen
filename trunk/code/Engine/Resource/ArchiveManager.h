#ifndef _ArchiveManager_H_
#define _ArchiveManager_H_

#include "Archive.h"

namespace ma
{
	class ArchiveManager
	{
	public:
		ArchiveManager();

		~ArchiveManager();

		StreamPtr			Open(const char* pszFile, bool readOnly = true) const;

		StreamPtr			Create(const char* pszFile) const;

		MemoryStreamPtr		ReadAll(const char* pszFile,UINT nExtenSiz = 0) const;

		void				AddArchive(Archive* pArchive);
		
		UINT				GetArchiveNumber();

		Archive*			GetArchiveByIndex(UINT index);

		bool				Exists(const char* pszFile) const;
		
		void				SetSaveDir(const char* pSaveDir);

		const char*			GetSaveDir() const;

	private:
		typedef std::vector< RefPtr<Archive> > VEC_ARCHIVE;

		VEC_ARCHIVE		m_vecArchive;

		std::string		m_saveDir;
	};

	 void SetArchiveManager(ArchiveManager* pArchiveManager);

	 ArchiveManager* GetArchiveMananger();

}

#endif
