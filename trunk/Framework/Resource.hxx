#include "Framework/Resource.h"

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

	void Resource::Load()
	{

	}
}