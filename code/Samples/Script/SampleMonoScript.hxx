#include "SampleMonoScript.h"
#include "MonoScript/Module.h"

namespace ma
{
	SampleMonoScript::SampleMonoScript()
	{
	}

	void SampleMonoScript::Init()
	{
		Sample::Init();

		MonoScriptModuleInit();

		Load();
	}

	void SampleMonoScript::Shutdown()
	{
		MonoScriptModuleShutdown();
	}

	void SampleMonoScript::Load()
	{
		IScriptDevice* pScriptDeveice = GetScriptDevice();
		if (pScriptDeveice == NULL)
			return;

		pScriptDeveice->ParseScriptAll("../../code/Samples/Script/CSharpTest/CSharpTest/bin/Debug/CSharpTest.dll");
		m_pTestScript = GetScriptDevice()->CreateScriptObject("MyClass");

		pScriptDeveice->Start();
	
	}

	void SampleMonoScript::Unload()
	{

	}

	void SampleMonoScript::Update()
	{
		Sample::Update();

		m_pTestScript->InvokeMethod("Update");

	}

	void SampleMonoScript::Render()
	{
// 		IRender* pRender = ma::GetRender();
// 		if (pRender == NULL)
// 			return;

	}

}


