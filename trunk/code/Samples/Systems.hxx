#include "Systems.h"

namespace ma
{

	void Systems::Start()
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Start();

		if (GetScriptSystem())
			GetScriptSystem()->Start();

		if (GetEntitySystem())
			GetEntitySystem()->Start();
	}

	void Systems::Stop()
	{
		if (GetEntitySystem())
			GetEntitySystem()->Stop();

		if (GetPhysicsSystem())
			GetPhysicsSystem()->Stop();

		if (GetScriptSystem())
			GetScriptSystem()->Stop();
	}

	void Systems::Update()
	{
		if (GetInput())
			GetInput()->Capture();

		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (GetResourceSystem())
			GetResourceSystem()->Update();

		if (GetAnimationSystem())
			GetAnimationSystem()->Update();

		if (GetPhysicsSystem())
			GetPhysicsSystem()->Update();

		if (GetScriptSystem())
			GetScriptSystem()->Update();

		if (GetEntitySystem())
			GetEntitySystem()->Update();

		Form::updateInternal(GetTimer()->GetFrameDeltaTime());
	}

}

