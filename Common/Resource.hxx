#include "Common/Resource.h"

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

	bool Resource::Load()
	{
		return false;
	}

	void Resource::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_sResPath);
		
		sl.EndSection();
	}

}