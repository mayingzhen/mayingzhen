#include "Framework/Resource.h"

namespace ma
{
	Resource::Resource(const char* pResPath)
	{
		m_sResPath = pResPath ? pResPath : "";
	}

	Resource::~Resource()
	{

	}

	void Resource::Load()
	{

	}
}