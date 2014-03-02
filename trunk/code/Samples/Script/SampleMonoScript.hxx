#include "SampleMonoScript.h"
#include "MonoScript/Module.h"

namespace ma
{
	SampleMonoScript::SampleMonoScript()
	{
		m_pTestScript = NULL;
	}

	void SampleMonoScript::Load()
	{
		Vector3 vEyePos = Vector3(0, 1.5f, 2);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		IScriptSystem* pScriptSystem = GetScriptSystem();
		if (pScriptSystem == NULL)
			return;

		std::string	strDllPath = FileSystem::getFullPath("Script/ScriptTest/bin/Debug/ScriptTest.dll");
		pScriptSystem->ParseScriptAll(strDllPath.c_str());
		
		m_pGameObj =  GetEntitySystem()->CreateGameObject("Test");

		MeshComponent* pMeshComp = m_pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load("Fbx/Box.skn","Fbx/Box.mat");

		m_pTestScript = m_pGameObj->CreateComponent<IScriptObject>();
		m_pTestScript->SetName("SharpTest");
	}

	void SampleMonoScript::UnLoad()
	{
	}

	void SampleMonoScript::Update()
	{
		if (m_pTestScript == NULL)
			return;

		float fRotSpeed = m_pTestScript->GetFloat("m_fRotSpeed");
		if ( GetInput()->IsKeyDown(OIS::KC_DOWN) )
		{
			fRotSpeed -= 1.0f;
			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
		}

		if (GetInput()->IsKeyDown(OIS::KC_UP))
		{
			fRotSpeed += 1.0f;
			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
		}
		
	}


}


