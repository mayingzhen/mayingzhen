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
		Vector3 vEyePos = Vector3(0, 3, 3);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		ScriptSystem* pScriptSystem = GetScriptSystem();
		if (pScriptSystem == NULL)
			return;
		
		std::string strDir = GetArchiveMananger()->GetArchiveByIndex(0)->GetName();	
		std::string	strDllPath = strDir + "/Script/ScriptTest/bin/Debug/ScriptTest.dll";

		pScriptSystem->ParseScriptAll(strDllPath.c_str());
		
		RefPtr<SceneNode> pGameObj = CreateSceneNode();

		RefPtr<MeshComponent> pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load("Fbx/Box.skn","Fbx/Box.mtl");

		m_pTestScript = pGameObj->CreateComponent<ScriptObject>();
		m_pTestScript->SetName("SharpTest");

		m_pScene->GetRootNode()->AddChild(pGameObj.get());
	}

	void SampleMonoScript::UnLoad()
	{
		m_pTestScript = NULL;
	}

	void SampleMonoScript::Update()
	{
		if (m_pTestScript == NULL)
			return;

// 		float fRotSpeed = m_pTestScript->GetFloat("m_fRotSpeed");
// 		if ( GetInput()->IsKeyDown(OIS::KC_DOWN) )
// 		{
// 			fRotSpeed -= 1.0f;
// 			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
// 		}
// 
// 		if (GetInput()->IsKeyDown(OIS::KC_UP))
// 		{
// 			fRotSpeed += 1.0f;
// 			m_pTestScript->SetFloat("m_fRotSpeed",fRotSpeed);
// 		}
		
	}


}


