#include "Common/RTTI/Object.h"

namespace ma
{
	Class* Object::ms_pObjectClass = NULL;
	void Object::StaticInitClass()
	{
		ms_pObjectClass = new Class("Object",NULL);
	}
	void Object::StaticShutdownClass()
	{
		SAFE_DELETE(ms_pObjectClass);
	}
	const Class*		Object::StaticGetClass()
	{
		return ms_pObjectClass;
	}
	Class*	Object::GetClass() const
	{
		return ms_pObjectClass;
	}
	//IMPL_OBJECT(Object)

	Object::Object(const char* pName)
	{
		m_sName = pName ? pName : "";
	}

	Object::~Object()
	{

	}
	
	void Object::Serialize(SerializeListener& sl,const char* pszLable) 
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName);

		sl.EndSection();
	}
}

