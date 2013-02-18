#include "Framework/Component.h"

namespace ma
{
	IMPL_OBJECT(Component,Object);

	Component::Component()
	{
		m_pGameObject = NULL;
	}

	Component::~Component()
	{

	}

	void Component::Render()
	{

	}

	void Component::Update()
	{

	}


	void Component::ParalleUpdate(float fTimeElapsed)
	{

	}

	void Component::FixedUpdate(float fTimeElapsed)
	{

	}

	void Component::LateUpdate(float fTimeElapsed)
	{

	}

	void Component::Start()
	{

	}

	void Component::Stop()
	{

	}
}

