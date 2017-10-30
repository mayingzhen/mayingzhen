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

	Stream* ArchiveManager::Open(const char* pszFile, bool readOnly)
	{
		std::lock_guard<std::mutex> autoLock(m_csOpen);

		ASSERT(pszFile);
		if (pszFile == NULL)
			return NULL;

		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			Stream* data = (*iter)->open(pszFile,readOnly);
			if (data != NULL)
			{
				return data;
			}
		}

		//ASSERT(false);
		//LogError("Open file :%s", pszFile);
		return NULL;
	}

	void ArchiveManager::Close(Stream* pStream)
	{
		pStream->Close();
		SAFE_DELETE(pStream);
	}

	RefPtr<MemoryStream> ArchiveManager::ReadAll(const char* pszFile)
	{
		RefPtr<Stream> pStream = this->Open(pszFile,true);
		if (pStream == NULL)
			return NULL;

		return new MemoryStream(pszFile, pStream.get(), pStream->GetSize(), false);
	}

	void ArchiveManager::AddArchive(Archive* pArchive)
	{
		std::lock_guard<std::mutex> autoLock(m_csOpen);

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

}
