#include "SampleMonoScript.h"
#include "MonoScript/Module.h"

namespace ma
{
	SampleMonoScript::SampleMonoScript()
	{
	}

	void SampleMonoScript::Load()
	{
		MonoScriptModuleInit();

		IScriptSystem* pScriptDeveice = GetScriptSystem();
		if (pScriptDeveice == NULL)
			return;

		pScriptDeveice->ParseScriptAll("../../code/Samples/Script/CSharpTest/CSharpTest/bin/Debug/CSharpTest.dll");
		m_pTestScript = GetScriptSystem()->CreateScriptObject("MyClass");

		pScriptDeveice->Start();
	}

	void SampleMonoScript::UnLoad()
	{
		MonoScriptModuleShutdown();
	}

	void SampleMonoScript::Update()
	{
		m_pTestScript->InvokeMethod("Update");
	}

	void SampleMonoScript::Render()
	{
// 		IRender* pRender = ma::GetRender();
// 		if (pRender == NULL)
// 			return;

	}

}


