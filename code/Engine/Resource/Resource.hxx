#include "Resource.h"
#include "ArchiveManager.h"

namespace ma
{
	IMPL_OBJECT(Resource,Object)

	Resource::Resource(const char* pszPath)
	{
		m_sResPath = pszPath ? pszPath : ""; 
		m_eResState = ResUnLoad;
		m_pDataStream = NULL;
	}

	Resource::~Resource()
	{	
	}

	void Resource::LoadSync()
	{
		if (m_eResState == ResLoaded)
			return ;

		LoadFileToMemeory();

		CreateFromMemeory();

		m_eResState = ResLoaded;
	}

	bool Resource::LoadFileToMemeory()
	{
		if (m_eResState == ResLoadIng || m_eResState == ResLoaded)
			return true;
			
		m_eResState = ResLoadIng;

		m_pDataStream = NULL;
		m_pDataStream = GetArchiveMananger()->ReadAll( m_sResPath.c_str() );
		if (m_pDataStream == NULL)
		{
			m_eResState = ResLoadError;
			return false;
		}

		return true;
	}

	bool Resource::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return true;

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		BinaryInputSerializer inAr;
		inAr.Open(m_pDataStream.get());
		Serialize(inAr);
		inAr.Close();

		m_eResState = ResLoaded;
	
		return true;
	}


	void Resource::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		BinaryOutputSerializer ar;
		bool bLoadOK = ar.Open(strSavePath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to save mesh from file");
			return ;
		}

		Serialize(ar);

		ar.Close();

		return ;
	}

	void Resource::Serialize(Serializer& sl, const char* pszLable)
	{

	}


}