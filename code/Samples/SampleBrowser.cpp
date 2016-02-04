#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#if PLATFORM_WIN != 1
#include "Animation/Module.h"
#include "GLESRender/Module.h"
#include "BulletPhysics/Module.h"
#else
#include "Animation/Module.h"
#include "D3D9Render/Module.h"
#include "GLESRender/Module.h"
#include "MonoScript/Module.h"
#include "BulletPhysics/Module.h"
#endif

#include "CameraController.hxx"
#include "Sample.hxx"

#if PLATFORM_WIN == 1
#include "Samples/Serialize/SampleFbxImport.hxx"
#include "Samples/Script/SampleMonoScript.hxx"
#include "Samples/Render/SampleLighting.hxx"
#include "Samples/Render/SampleShadowMap.hxx"
#endif

#include "Samples/Serialize/SampleSceneSerialize.hxx"
#include "Samples/Physics/SampleRigidBody.hxx"
#include "Samples/Physics/SampleCharaControl.hxx"
#include "Samples/Physics/SampleJoint.hxx"
#include "Samples/Physics/SampleRagdoll.hxx"
#include "Samples/Render/SampleTerrain.hxx"
#include "Samples/Animation/SampleAnimationRetarget.hxx"
#include "Samples/Animation/SampleAnimationTree.hxx"
#include "Samples/Render/SampleParticle.hxx"



namespace ma
{
	SampleBrowser* GetSampleBrowser()
	{
		return (SampleBrowser*)&Game::GetInstance();
	}


	SampleBrowser::SampleBrowser(const char* pGameName)
		:Game(pGameName)
	{
		m_bPause = false;
		m_bStepOneFrame = false;
		m_pCurSample = NULL;
		m_pCameraControl = NULL;

		ModuleInit(RenderDevice_D3D9);
	}

	SampleBrowser::~SampleBrowser()
	{
		ModuleShutdown();
	}

	void SampleBrowser::ModuleInit(RenderDeviceType eType)
	{
		AnimationModuleInit();
		BtPhysicsModuleInit();

#if PLATFORM_WIN == 1
		if (eType == RenderDevice_D3D9)
		{
			D3D9RenderModuleInit();
		}
		else
		{
			GLESRenderModuleInit();
		}

		MonoScriptModuleInit();
		FBXImporterModuleInit();
#else
		GLESRenderModuleInit();		
#endif
	}

	void SampleBrowser::ModuleShutdown()
	{
		AnimationModuleShutdown();
		BtPhysicsModuleShutdown();

#if PLATFORM_WIN == 1
		FBXImporterModuleShutdown();
		MonoScriptModuleShutdown();

		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_D3D9)
		{
			D3D9RenderModuleShutdown();
		}
		else
		{
			GLESRenderModuleShutdown();
		}
#else

		GLESRenderModuleShutdown();
#endif	
	}

	void SampleBrowser::InitSampleList()
	{
#if PLATFORM_WIN == 1
		m_arrSamples["FbxImport"] = new SampleFbxImport();

		m_arrSamples["CSharpScript"] = new SampleMonoScript();

		m_arrSamples["Lighting"] = new SampleLighting();
		m_arrSamples["ShadowMap"] = new SampleShadowMap();
#endif

		m_arrSamples["Terrain"] = new SampleTerrain();
		m_arrSamples["Particle"] = new SampleParticle();

		// Serialize
		m_arrSamples["SceneSerialize"] = new SampleSceneSerialize();

		// Physics
		m_arrSamples["RigidBody"] = new SampleRigidBody();
		m_arrSamples["CharControl"] = new SampleCharaControl();
		m_arrSamples["PhysicsJoint"] = new SampleJoint();
		m_arrSamples["Ragdoll"] = new SampleRagdoll();

		// Animation
		m_arrSamples["AnimationRetarget"] = new SampleAnimationRetarget();
		m_arrSamples["AnimationTree"] = new SampleAnimationTree();


		RunSample("AnimationRetarget");
	}

	void SampleBrowser::InitResourcePath()
	{
#if PLATFORM_WIN == 1
		char pszPath[MAX_PATH] = {0};
		
 		GetFullPathName("../../data/",MAX_PATH,pszPath,NULL);
 		GetArchiveMananger()->AddArchive( CreateFileArchive(pszPath).get() );

		GetArchiveMananger()->SetSaveDir(pszPath);

#elif PLAFTORM_IOS == 1
		std::string sAppDir = Platform::GetInstance().GetAppPath();

		sDataDir = sAppDir + "data/data.zip";
		GetArchiveMananger()->AddArchive( CreateZipArchive(sDataDir.c_str()).get() );

        std::string sDataDir = sAppDir + "data";
		GetArchiveMananger()->AddArchive( CreateFileArchive(sDataDir.c_str()).get() );

#elif PLATFORM_ANDROID == 1
		GetArchiveMananger()->AddArchive( new AAssetArchive("data") );
#endif
	}

	void SampleBrowser::Shutdown()
	{
		if (m_pCurSample)
		{
			m_pCurSample->UnLoad();
			if (GetPhysicsSystem())
				GetPhysicsSystem()->Stop();

#if PLATFORM_WIN == 1
			if (GetScriptSystem())
				GetScriptSystem()->Stop();
#endif
		}

		SAFE_DELETE(m_pCameraControl);
		
		//GetInput()->RemoveKeyListener(this);
		
		std::map<std::string,Sample*>::iterator it = m_arrSamples.begin();
		for (; it != m_arrSamples.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		m_arrSamples.clear();
		
		if (GetPhysicsSystem())
			GetPhysicsSystem()->ShoutDown();
#if PLATFORM_WIN == 1
		if (GetScriptSystem())
			GetScriptSystem()->ShutDown();
#endif
		Game::Shutdown();
	}

	void SampleBrowser::Init()
	{


		InitResourcePath();
		
		Game::Init();
		
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Init();

#if PLATFORM_WIN == 1
		if (GetScriptSystem())
			GetScriptSystem()->Init();
#endif

		//GetInput()->AddKeyListener(this);
		
		Scene* pScene = GetRenderSystem()->GetScene();
		pScene->SetCallback(this);

		m_pCameraControl = new CameraController( pScene->GetCamera() );
	
		//LoadUI();

		InitSampleList();
	}

	void SampleBrowser::RunSample(const char* pSampleNma)
	{
		std::map<std::string,Sample*>::iterator it = m_arrSamples.find(pSampleNma);
		ASSERT(it != m_arrSamples.end());
		if  (it == m_arrSamples.end())
			return;

		if (m_pCurSample)
		{
			m_pCurSample->UnLoad();
			if (GetPhysicsSystem())
				GetPhysicsSystem()->Stop();

#if PLATFORM_WIN == 1
			if (GetScriptSystem())
				GetScriptSystem()->Stop();
#endif

			if (m_pCameraControl)
				m_pCameraControl->ResetCamera();

 			m_pCurSample->GetScene()->Reset();
		}
		
		Sample* pSameple = it->second;

		pSameple->Load();
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Start();

#if PLATFORM_WIN == 1
		if (GetScriptSystem())
			GetScriptSystem()->Start();
#endif

		m_pCurSample = pSameple;
	}


	void SampleBrowser::Update()
	{
		//Log("................. Update() ..................");

		if (GetInput())
			GetInput()->InjectInputEnd();

		Game::Update();

		if (m_pCameraControl)
			m_pCameraControl->UpdateInput();

		if (m_bPause && !m_bStepOneFrame)
		{
			return;				
		}
		m_bStepOneFrame = false;

					
		if (m_pCurSample)
			m_pCurSample->Update();

		if (GetInput())
			GetInput()->InjectInputBegin();
	
	}

	Camera*	SampleBrowser::GetCamera()
	{
		return  GetRenderSystem()->GetScene()->GetCamera();
	}




// 	void SampleBrowser::controlEvent(Control* control, EventType evt)
// 	{
// 		Log("..............controlEvent.............");
// 
// 		Button* pButton = static_cast<Button*>(control);
// 		if (pButton == NULL)
// 			return;
// 
// 		const char* pText = pButton->getText();
// 		RunSample(pText);
// 	}

// 	bool SampleBrowser::keyPressed(const OIS::KeyEvent &arg)
// 	{
// // 		if (arg.key == OIS::KC_S)
// // 		{
// // 			m_pSystems->Start();
// // 		}
// 
// 		if (arg.key == OIS::KC_P)
// 		{
// 			m_bPause = !m_bPause;
// 			if (m_bPause)
// 				m_bStepOneFrame = false;
// 		}
// 		
// 		if (arg.key == OIS::KC_N)
// 		{
// 			m_bStepOneFrame = true;
// 		}
// 
// 		return false;
// 	}

// 	bool SampleBrowser::keyReleased(const OIS::KeyEvent &arg)
// 	{
// 		return false;
// 	}

	void SampleBrowser::OnPreUpdate(Scene* pScene)
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->BeginUpdate();
	}

	void SampleBrowser::OnPostUpdate(Scene* pScene)
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->EndUpdate();
	}

	void SampleBrowser::OnPreRender(Scene* pScene)
	{

	}

	void SampleBrowser::OnPosRender(Scene* pScene)
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->DebugRender();
// 
// 		if (m_pCurSample)
// 			m_pCurSample->Render();

		//GetUISystem()->Render();
	}
}

