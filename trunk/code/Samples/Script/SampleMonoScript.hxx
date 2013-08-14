#include "SampleMonoScript.h"
#include "MonoScript/Module.h"

namespace ma
{
	SampleMonoScript::SampleMonoScript()
	{
	}

	void SampleMonoScript::Load()
	{
		Vector3 vEyePos = Vector3(0, 6, 8);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		IScriptSystem* pScriptSystem = GetScriptSystem();
		if (pScriptSystem == NULL)
			return;

		pScriptSystem->ParseScriptAll("../../code/Samples/Script/CSharpTest/CSharpTest/bin/Debug/CSharpTest.dll");
		
		m_pGameObj =  GetEntitySystem()->CreateGameObject("Test");

		MeshComponent* pMeshComp = m_pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load("Fbx/Box.skn","Fbx/Box.tga");

		ScriptComponent* pScriptComp = m_pGameObj->CreateComponent<ScriptComponent>();
		m_pTestScript = pScriptComp->CreatScriptObject("MyClass");
	}

	void SampleMonoScript::UnLoad()
	{
	}

	void SampleMonoScript::Update()
	{
		float fRotSpeed = m_pTestScript->GetFloat("m_fRotSpeed");
		if ( GetInput()->IsKeyDown(OIS::KC_DOWN) )
		{
			fRotSpeed -= 0.1f;
			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
		}

		if (GetInput()->IsKeyDown(OIS::KC_UP))
		{
			fRotSpeed += 0.1f;
			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
		}
		
	}


}


