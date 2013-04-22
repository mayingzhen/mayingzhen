#include "Samples/Script/SampleMonoScript.h"
#include "Framework/Module.h"
#include "MonoScript/MonoScriptModule.h"

namespace ma
{
	SampleMonoScript::SampleMonoScript()
	{
	}

	void SampleMonoScript::Init(Application* pApplication)
	{
		SimpleSceneView::Init(pApplication);

		FrameWorkModuleInit();
		MonoScriptModuleInit();
	}

	void SampleMonoScript::Shutdown()
	{
		FrameWorkModuleShutdown();
		MonoScriptModuleShutdown();
	}

	void SampleMonoScript::Load()
	{
		MonoScriptDevice* pScriptDeveice = (MonoScriptDevice*)GetScriptDevice();
		if (pScriptDeveice == NULL)
			return;

		//pScriptDeveice->ParseScriptDll("");
	}

	void SampleMonoScript::Unload()
	{

	}

	void SampleMonoScript::Tick(float timeElapsed)
	{
		__super::Tick(timeElapsed);

	}

	void SampleMonoScript::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

	}

}


