#include "Resource.h"

namespace ma
{
	Resource::Resource(const char* pszPath)
	{
		m_sResPath = pszPath ? pszPath : ""; 
		m_bLoaded = false;
	}

	Resource::~Resource()
	{

	}

	bool Resource::Load(const char* pszPath)
	{
		if (pszPath)
		{
			m_sResPath = pszPath;
		}

		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(m_sResPath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to open mesh from file");
			return NULL;
		}

		Serialize(ar);

		return true;
	}

	bool Resource::Save(const char* pszPath)
	{
		if (pszPath)
		{
			m_sResPath = pszPath;
		}

		BinaryOutputArchive ar;
		bool bLoadOK = ar.Open(m_sResPath.c_str());
		if (!bLoadOK)
		{
			Log("Fail to open mesh from file %s:",m_sResPath.c_str());
			return NULL;
		}

		Serialize(ar);

		return true;
	}

	void Resource::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_sResPath);
		
		sl.EndSection();
	}

}