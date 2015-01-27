#include "Uniform.h"


namespace ma
{
	Uniform::Uniform()
	{
		m_location = -1;
		m_index = 0;
		m_type = 0;
		m_vshShder = false;
		m_nCount = 0;

		m_pMethod = NULL;
	}

	Uniform::~Uniform()
	{
		SAFE_DELETE(m_pMethod);
	}

	void Uniform::Bind()
	{
		if (m_pMethod) 
			m_pMethod->SetValue();
	}

	const char* Uniform::GetName() const
	{
		return m_name.c_str();
	}

	void Uniform::SetName(const char* pszName)
	{
		m_name = pszName ? pszName : "";
	}

}

