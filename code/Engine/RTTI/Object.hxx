#include "Object.h"

namespace ma
{
	RTTIClass* Object::ms_pObjectClass = NULL;
	void Object::StaticInitClass()
	{
		ms_pObjectClass = new RTTIClass("Object",NULL);
	}
	void Object::StaticShutdownClass()
	{
		SAFE_DELETE(ms_pObjectClass);
	}
	const RTTIClass*		Object::StaticGetClass()
	{
		return ms_pObjectClass;
	}
	RTTIClass*	Object::GetClass() const
	{
		return ms_pObjectClass;
	}
	//IMPL_OBJECT(Object)
	
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

}

