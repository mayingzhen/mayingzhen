#include "Systems.h"

namespace ma
{

	void Systems::Start()
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Start();

		if (GetScriptSystem())
			GetScriptSystem()->Start();
	}

	void Systems::Stop()
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Stop();

		if (GetScriptSystem())
			GetScriptSystem()->Stop();
	}

	void Systems::Update()
	{
		profile_code();

		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (GetPhysicsSystem())
			GetPhysicsSystem()->BeginUpdate();

		if (GetResourceSystem())
			GetResourceSystem()->Update();

		if (GetAnimationSystem())
			GetAnimationSystem()->Update();

		if (GetScriptSystem())
			GetScriptSystem()->Update();

		if (GetUISystem())
			GetUISystem()->Update();

		if (GetTerrain())
			GetTerrain()->Update();

		if (GetPhysicsSystem())
			GetPhysicsSystem()->EndUpdate();

		if (GetEntitySystem())
			GetEntitySystem()->Update();

		if ( GetSceneSystem() )
			GetSceneSystem()->Update();

		if ( GetRenderSystem() )
			GetRenderSystem()->Update();

		if ( GetLightSystem() )
			GetLightSystem()->Update();

		// Update particle system as late as possible, only renderer is dependent on it.
		if (GetParticleSystem())
			GetParticleSystem()->Update();
	}

}

