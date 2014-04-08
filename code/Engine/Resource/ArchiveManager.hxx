#include "ArchiveManager.h"

namespace ma
{
	static ArchiveManager* gpArchiveManager = NULL;

	void SetArchiveManager(ArchiveManager* pArchiveManager)
	{
		gpArchiveManager = pArchiveManager;
	}

	ArchiveManager* GetArchiveMananger()
	{
		return gpArchiveManager;
	}


	ArchiveManager::ArchiveManager()
	{
	}

	ArchiveManager::~ArchiveManager()
	{
	}

	StreamPtr ArchiveManager::Open(const char* pszFile, bool readOnly /*= true*/ ) const
	{
		ASSERT(pszFile);
		if (pszFile == NULL)
			return NULL;

		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			StreamPtr data = (*iter)->open(pszFile, readOnly);
			if (data != NULL)
			{
				return data;
			}
		}

		ASSERT(false);
		Log("Cannot open file :%s", pszFile);
		return NULL;
	}

	MemoryStreamPtr	ArchiveManager::ReadAll(const char* pszFile,UINT nExtenSiz/* = 0*/) const
	{
		StreamPtr pStream = Open(pszFile,true);
		if (pStream == NULL)
			return NULL;

		MemoryStreamPtr pMemStream = new MemoryStream(pStream->GetName(),pStream.get(),pStream->GetSize() + nExtenSiz,true);
		
		memset(pMemStream->GetPtr() + pStream->GetSize(),0,nExtenSiz);

		return pMemStream;
	}

	void ArchiveManager::AddArchive(Archive* pArchive)
	{
		m_vecArchive.insert(m_vecArchive.begin(), pArchive);
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

	std::string	ArchiveManager::GetFullPath(const char* pszFile) const
	{
		for (VEC_ARCHIVE::const_iterator iter = m_vecArchive.begin();iter != m_vecArchive.end();++iter)
		{
			if( (*iter)->exists(pszFile) )
			{
				return std::string( (*iter)->GetName() ) + "/" + pszFile; 
			}
		}

		return "";
	}


}
