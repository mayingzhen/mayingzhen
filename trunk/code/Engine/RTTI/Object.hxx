#include "Object.h"

namespace ma
{
	RefPtr<RTTIClass> Object::ms_pObjectClass = NULL;
	void Object::StaticInitClass()
	{
		ms_pObjectClass = CreateRTTIClass("Object",NULL);
	}
	void Object::StaticShutdownClass()
	{
		ms_pObjectClass = NULL;
	}
	const RTTIClass*		Object::StaticGetClass()
	{
		return ms_pObjectClass.get();
	}
	RTTIClass*	Object::GetClass() const
	{
		return ms_pObjectClass.get();
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

