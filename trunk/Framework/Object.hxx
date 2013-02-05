#include "Framework/Object.h"

namespace ma
{
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

// 	void Object::Render()
// 	{
// 	}
// 
// 	void Object::Update()
// 	{
// 	}
// 
// 	void Object::ParalleUpdate(float fTimeElapsed)
// 	{
// 	}
// 
// 	void Object::FixedUpdate(float fTimeElapsed)
// 	{
// 	}
// 
// 	void Object::LateUpdate(float fTimeElapsed)
// 	{
// 	}
// 
// 	void Object::Start()
// 	{
// 	}
// 
// 	void Object::Stop()
// 	{
// 	}

}
