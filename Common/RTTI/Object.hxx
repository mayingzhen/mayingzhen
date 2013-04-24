#include "Object.h"
#include "Property.h"

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

	void Object::TravelProperty(PropertyVisitor* pVisitor)
	{
		//pVisitor->VisitProperty(m_nPropTag,"propTag",0);
		pVisitor->VisitProperty(m_sName,"name","object");
	}
	
}

