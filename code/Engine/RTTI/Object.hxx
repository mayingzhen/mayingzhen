#include "Object.h"

namespace ma
{
	Object::Object()
	{

	}

	Object::Object(const char* pName)
	{
		m_sName = pName ? pName : "";
	}

	Object::~Object()
	{

	}

	const char*	Object::GetName() const 
	{
		return m_sName.c_str();
	}

	void Object::SetName(const char* pName) 
	{
		m_sName = pName ? pName : "";
	}

}

