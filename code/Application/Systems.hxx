#include "Systems.h"

namespace ma
{
	void Systems::Init()
	{
		Input* pInput = new Input();
		SetInput(pInput);
		pInput->Init(Platform::GetInstance().GetWindId());
		int w,h;
		Platform::GetInstance().GetWindowSize(w,h);
		pInput->OnResize(w,h);

		GetRenderSystem()->Init(Platform::GetInstance().GetWindId());
	}

	void Systems::Shutdown()
	{
		GetRenderSystem()->ShoutDown();

		Input* pInput = GetInput();
		pInput->Shutdown();
		SAFE_DELETE(pInput);
		SetInput(NULL);
	}

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

		if ( GetRenderSystem() )
			GetRenderSystem()->Update( GetRenderSystem()->GetMainCamera() );

		if ( GetLightSystem() )
			GetLightSystem()->Update();

		// Update particle system as late as possible, only renderer is dependent on it.
		if (GetParticleSystem())
			GetParticleSystem()->Update();
	}

}

