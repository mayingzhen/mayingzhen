#include "Resource.h"

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

	bool Resource::LoadFileToMemeory()
	{
		if (m_eResState == ResLoadIng || m_eResState == ResLoaded)
			return true;
			
		m_eResState = ResLoadIng;

		SAFE_DELETE(m_pDataStream);
		m_pDataStream =  FileSystem::readAll(m_sResPath.c_str());

		return true;
	}

// 	bool Resource::Save(const char* pszPath)
// 	{
// 		if (pszPath)
// 		{
// 			m_sResPath = pszPath;
// 		}
// 
// 		BinaryOutputArchive ar;
// 		bool bLoadOK = ar.Open(m_sResPath.c_str());
// 		if (!bLoadOK)
// 		{
// 			ASSERT(false && "Fail to save mesh from file");
// 			return NULL;
// 		}
// 
// 		Serialize(ar);
// 
// 		return true;
// 	}


}