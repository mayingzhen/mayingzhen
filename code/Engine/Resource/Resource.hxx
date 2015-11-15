#include "Resource.h"
#include "ArchiveManager.h"

namespace ma
{
	Resource::Resource(const char* pszPath)
	{
		m_sResPath = pszPath ? pszPath : ""; 
		m_eResState = ResUnLoad;
		m_pDataStream = NULL;
		m_eCreateType = MEM_RES;
	}

	Resource::~Resource()
	{
		m_pDataStream = NULL;
	}

	bool Resource::LoadFromXML(const char* pFilePath)
	{
		m_sResPath = pFilePath ? pFilePath  : "";
		m_eResState = ResUnLoad;
		m_pDataStream = NULL;
		m_eCreateType = XML_RES;

		return Load();
	}

	bool Resource::SaveToXML(const char* pFilePath)
	{
		std::string strSavePath = pFilePath ? pFilePath : m_sResPath;

		XMLOutputSerializer ar;

		Serialize(ar,this->GetClassName());

		return ar.Save(strSavePath.c_str());
	}

	bool Resource::LoadFromFile(const char* pFilePath)
	{
		m_sResPath = pFilePath ? pFilePath : "";
		m_eResState = ResUnLoad;
		m_pDataStream = NULL;
		m_eCreateType = BIN_RES;

		return Load();
	}

	bool Resource::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		BinaryOutputSerializer ar;
		bool bLoadOK = ar.Open(strSavePath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to save mesh from file");
			return false;
		}

		Serialize(ar,this->GetClassName());

		ar.Close();

		return true;
	}

	bool Resource::Load()
	{
		ASSERT(m_sResPath != "");
		if (m_sResPath == "")
			return false;

 		if (m_eResState == ResInited)
 			return true;

		if (m_eResState != ResUnLoad)
			return false;
		
		ASSERT(m_eResState == ResUnLoad);
		if ( GetResourceSystem()->GetDataThread() )
		{
			GetResourceSystem()->GetDataThread()->PushBackDataObj(this);

			return false;
		}
		else
		{
			LoadSync();

			return true;
		}
	}

	void Resource::LoadSync()
	{
		if (m_eResState == ResInited)
			return;

		LoadFileToMemeory();

		CreateFromMemeory();

		m_eResState = ResInited;
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
	
		if (m_eCreateType == XML_RES)
		{
			XMLInputSerializer inAr;
			inAr.Open(m_pDataStream.get());
			Serialize(inAr,this->GetClassName());
			inAr.Close();

			m_pDataStream = NULL;
		}
		else if (m_eCreateType == BIN_RES)
		{
			BinaryInputSerializer inAr;
			inAr.Open(m_pDataStream.get());
			Serialize(inAr,this->GetClassName());
			inAr.Close();

			m_pDataStream = NULL;
		}

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

	void Resource::Serialize(Serializer& sl, const char* pszLable)
	{

	}

	RefPtr<Resource> CreateResource(const char* pszPath)
	{
		Resource* pRes = new Resource(pszPath);
		pRes->Load();
		return pRes;
	}
}

