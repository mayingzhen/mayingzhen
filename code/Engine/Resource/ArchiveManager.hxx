#include "ArchiveManager.h"

namespace ma
{
	ArchiveManager* g_pArchiveManager = NULL;

	ArchiveManager* GetArchiveMananger()
	{
		return g_pArchiveManager;
	}


	ArchiveManager::ArchiveManager()
	{
	}

	ArchiveManager::~ArchiveManager()
	{
	}

	void ArchiveManager::SetSaveDir(const char* pSaveDir)
	{
		m_saveDir = pSaveDir ? pSaveDir : "";
	}

	const char*	ArchiveManager::GetSaveDir() const
	{
		return m_saveDir.c_str();
	}

	RefPtr<Stream> ArchiveManager::Create(const char* pszFile) const
	{
		ASSERT(pszFile);
		if (pszFile == NULL)
			return NULL;

		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			RefPtr<Stream> data = (*iter)->create(pszFile);
			if (data != NULL)
			{
				return data;
			}
		}

		ASSERT(false);
		LogError("Ceate file :%s", pszFile);
		return NULL;
	}

	RefPtr<MemoryStream> ArchiveManager::Open(const char* pszFile, bool readOnly /*= true*/ ) const
	{
		ASSERT(pszFile);
		if (pszFile == NULL)
			return NULL;

		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			RefPtr<MemoryStream> data = (*iter)->open(pszFile, readOnly);
			if (data != NULL)
			{
				return data;
			}
		}

		ASSERTMSG1(false,"Cannot open file :%s", pszFile);
		return NULL;
	}

	void ArchiveManager::AddArchive(Archive* pArchive)
	{
		m_vecArchive.insert(m_vecArchive.begin(), pArchive);
	}

	UINT ArchiveManager::GetArchiveNumber()
	{
		return m_vecArchive.size();
	}

	Archive* ArchiveManager::GetArchiveByIndex(UINT index)
	{
		return m_vecArchive[index].get();
	}

	bool ArchiveManager::Exists( const char* pszFile ) const
	{
		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			if((*iter)->exists(pszFile))
			{
				return true;
			}
		}

		return false;
	}

// 	std::string	ArchiveManager::GetFullPath(const char* pszFile) const
// 	{
// 		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
// 		{
// 			if( (*iter)->exists(pszFile) )
// 			{
// 				return std::string( (*iter)->GetName() ) + "/" + pszFile; 
// 			}
// 		}
// 
// 		return "";
// 	}


}
