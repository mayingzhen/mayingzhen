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
        ASSERT(m_pDataStream);
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
		if (m_eResState != ResLoaded)
			return false;

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		bool bInit = InitRes();

		this->SetResState(ResInited);
				
		return true;
	}

	bool Resource::IsReady()
	{
		switch(m_eResState)
		{
		case ResLoaded:
// 			if (!GetRenderSystem()->TestScene())
// 			{
// 				return false;
// 			}

			if (!this->InitRes())
			{
				this->SetResState(ResLoadError);
				return false;
			}

			this->SetResState(ResInited);
		case ResInited:
			{
				for (LST_RESOURCE::iterator iter = m_lstChild.begin();iter != m_lstChild.end();)
				{
					Resource* pChild = (*iter).get();
					if (pChild->IsReady())
					{
						iter = m_lstChild.erase(iter);
					}
					else
					{
						if (pChild->GetResState() == ResLoadError)
						{
							this->SetResState(ResLoadError);
							return false;
						}
						++iter;
					}
				}

				if (m_lstChild.empty())
				{
					if (!this->ChildResFinish())
					{
						this->SetResState(ResLoadError);
						return false;
					}

					this->SetResState(ResReady);
					return true;
				}

				return false;
			}
		case ResReady:
			return true;
		default:
			break;
		}

		return false;
	}

	void Resource::AddRes(Resource* pRes)
	{
		ASSERT(pRes);
		if (pRes == NULL)
			return;

		m_lstChild.push_back(pRes);
	}

	RefPtr<Resource> CreateResource(const char* pszPath)
	{
		Resource* pRes = new Resource();
		pRes->Load(pszPath);
		return pRes;
	}
}

