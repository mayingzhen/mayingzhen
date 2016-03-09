#include "Resource.h"
#include "ArchiveManager.h"

namespace ma
{
	Resource::Resource()
	{
		m_eResState = ResUnLoad;
	}

	Resource::~Resource()
	{
		m_pDataStream = NULL;
	}


	bool Resource::Load(const char* pFilePath)
	{
		ASSERT(pFilePath);
		if (pFilePath == NULL)
			return false;

		m_sResPath = pFilePath;

 		if (m_eResState == ResInited)
 			return true;

		if (m_eResState != ResUnLoad)
			return false;
		
		ASSERT(m_eResState == ResUnLoad);
		if (GetDataThread())
		{
			GetDataThread()->PushBackDataObj(this);

			return true;
		}
		else
		{
			LoadSync(pFilePath);

			return true;
		}
	}

	void Resource::LoadSync(const char* pFilePath)
	{
		ASSERT(pFilePath);
		if (pFilePath == NULL)
			return;

		m_sResPath = pFilePath;

		if (m_eResState == ResInited)
			return;

		LoadFileToMemeory();

		CreateFromMemeory();

		m_eResState = ResInited;
	}

	bool Resource::SaveToFile(const char* pszPath)
	{
		return false;
	}

	bool Resource::LoadFileToMemeory()
	{
		ASSERT(m_eResState == ResUnLoad);

		m_eResState = ResLoadIng;

		m_pDataStream = GetArchiveMananger()->ReadAll( m_sResPath.c_str() );
		if (m_pDataStream == NULL)
		{
			m_eResState = ResLoadError;
			return false;
		}

		m_eResState = ResLoaded;

		return true;
	}

	bool Resource::CreateFromMemeory()
	{
		ASSERT(m_eResState == ResLoaded);

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		bool bInit = InitRes();

		m_eResState = ResInited;
	
		IsReady();
			
		return true;
	}
	
	bool Resource::IsReady()
	{
		if (m_eResState == ResInited)
			return true;
	
		return false;
	}

	RefPtr<Resource> CreateResource(const char* pszPath)
	{
		Resource* pRes = new Resource();
		pRes->Load(pszPath);
		return pRes;
	}
}

