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

		if ( GetRenderSystem() )
			GetRenderSystem()->Init(Platform::GetInstance().GetWindId());

		if ( GetScriptSystem() )
			GetScriptSystem()->Init();

		if ( GetPhysicsSystem() )
			GetPhysicsSystem()->Init();
	}

	void Systems::Shutdown()
	{
		if ( GetPhysicsSystem() )
			GetPhysicsSystem()->ShoutDown();

		if ( GetScriptSystem() )
			GetScriptSystem()->Shutdown();

		if ( GetRenderSystem() )
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

		if (GetScriptSystem())
			GetScriptSystem()->Update();

		//if (GetUISystem())
		//	GetUISystem()->Update();

		if (GetPhysicsSystem())
			GetPhysicsSystem()->EndUpdate();

		if ( GetRenderSystem() )
			GetRenderSystem()->Update();

		if (GetAnimationSystem())
			GetAnimationSystem()->Update();

		if ( GetLightSystem() )
			GetLightSystem()->Update();

		// Update particle system as late as possible, only renderer is dependent on it.
		if (GetParticleSystem())
			GetParticleSystem()->Update();
	}

}

