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
		
		Stream*					Open(const char* pszFile, bool readOnly = true);
		void					Close(Stream* pStream);

		RefPtr<MemoryStream>	ReadAll(const char* pszFile);

		void					AddArchive(Archive* pArchive);
		uint32_t				GetArchiveNumber();
		Archive*				GetArchiveByIndex(uint32_t index);

		bool					Exists(const char* pszFile) const;
		
		void					SetSaveDir(const char* pSaveDir);
		const char*				GetSaveDir() const;

	private:
		typedef std::vector< RefPtr<Archive> > VEC_ARCHIVE;

		VEC_ARCHIVE		m_vecArchive;

		std::string		m_saveDir;

		std::mutex		m_csOpen;
	};


	extern ArchiveManager* g_pArchiveManager;
	ArchiveManager* GetArchiveMananger();

}

#endif
