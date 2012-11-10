#include "Framework/Object.h"

namespace ma
{
	Object::Object(const char* pName)
	{
		m_sName = pName;
	}

	Object::~Object()
	{

	}

	void Object::Render()
	{
	}

	void Object::Update()
	{
	}

	void Object::ParalleUpdate(float fTimeElapsed)
	{
	}

	void Object::FixedUpdate(float fTimeElapsed)
	{
	}

	void Object::LateUpdate(float fTimeElapsed)
	{
	}

	void Object::Start()
	{
	}

	void Object::Stop()
	{
	}

}
