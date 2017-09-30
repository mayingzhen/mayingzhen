#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#include "Animation/Module.h"
#include "GLESRender/Module.h"
#include "BulletPhysics/Module.h"

#if PLATFORM_WIN == 1
#include "MonoScript/Module.h"
#include "D3D11Render/Module.h"
#include "VulkanRender/Module.h"
#endif

#if defined(__APPLE__)
#include "MetalRender/Module.h"
#endif

#include "CameraController.hxx"
#include "Sample.hxx"

#if PLATFORM_WIN == 1
//#include "Samples/Serialize/SampleFbxImport.hxx"
#include "Samples/Script/SampleMonoScript.hxx"
#endif
#include "Samples/Render/SampleLighting.hxx"
#include "Samples/Render/SampleShadowMap.hxx"
#include "Samples/Render/SampleMaterial.hxx"
#include "Samples/Render/SampleIBL.hxx"
#include "Samples/Serialize/SampleSceneSerialize.hxx"
#include "Samples/Physics/SampleRigidBody.hxx"
#include "Samples/Physics/SampleCharaControl.hxx"
#include "Samples/Physics/SampleJoint.hxx"
#include "Samples/Physics/SampleRagdoll.hxx"
#include "Samples/Render/SampleTerrain.hxx"
#include "Samples/Animation/SampleAnimationRetarget.hxx"
#include "Samples/Animation/SampleAnimationTree.hxx"
#include "Samples/Animation/SampleAnimationIK.hxx"


namespace ma
{
	SampleBrowser* GetSampleBrowser()
	{
		return (SampleBrowser*)&Game::GetInstance();
	}


	SampleBrowser::SampleBrowser(const char* pGameName,const Setting& set,const char* pszRunSample)
		:Game(pGameName,set)
	{
        m_strSample = pszRunSample;
        
		m_bPause = false;
		m_bStepOneFrame = false;
		m_pCurSample = NULL;
		m_pCameraControl = NULL;
	}

	SampleBrowser::~SampleBrowser()
	{
	}

	void SampleBrowser::ModuleInit()
	{
		AnimationModuleInit();
		BtPhysicsModuleInit();
        

#if PLATFORM_WIN == 1
        VulkanRenderModuleInit();
#elif PLAFTORM_IOS == 1
        MetalRenderModuleInit();
#elif PLATFORM_ANDROID == 1
        VulkanRenderModuleInit();
#endif
      
#if PLATFORM_WIN == 1
        MonoScriptModuleInit();
		//FBXImporterModuleInit();
#endif
        
	}

	void SampleBrowser::ModuleShutdown()
	{
		AnimationModuleShutdown();
		BtPhysicsModuleShutdown();
        
#if PLATFORM_WIN == 1
        VulkanRenderModuleShutdown();
#elif PLAFTORM_IOS == 1
        MetalRenderModuleShutdown();
#elif PLATFORM_ANDROID == 1
        VulkanRenderModuleShutdown();
#endif
       
#if PLATFORM_WIN == 1
        MonoScriptModuleShutdown();
		//FBXImporterModuleShutdown();
#endif	
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

		//std::string sDataDir = sAppDir + "data.zip";
		//GetArchiveMananger()->AddArchive( CreateZipArchive(sDataDir.c_str()).get() );

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
		m_pCurSample = NULL;

		SAFE_DELETE(m_pCameraControl);
		
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Shoutdown();
        
#if PLATFORM_WIN == 1
		if (GetScriptSystem())
			GetScriptSystem()->Shoutdown();
#endif
        
		Game::Shutdown();

		ModuleShutdown();
	}

	void SampleBrowser::Init()
	{
        ModuleInit();
		
		InitResourcePath();

		Game::Init();
	
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Init();
        
#if PLATFORM_WIN == 1
		if (GetScriptSystem())
			GetScriptSystem()->Init();
#endif
			
		HWND hWnd = Platform::GetInstance().GetWindId();
		
		Scene* pScene = GetRenderSystem()->GetScene();
		pScene->SetCallback(this);

		m_pCameraControl = new CameraController( pScene->GetCamera() );

		RunSample(m_strSample.c_str());

		if (GetPhysicsSystem())
			GetPhysicsSystem()->Start();
	}

	void SampleBrowser::RunSample(const char* pSample)
	{
#ifdef WIN32
// 		if (stricmp(pSample,"SampleFbxImport") == 0)
// 		{
// 			m_pCurSample = new SampleFbxImport();
// 		}
//		else 
		if (strcmp(pSample,"SampleMonoScript") == 0)
		{
			m_pCurSample = new SampleMonoScript();
		}
		else
#endif
        if (strcmp(pSample,"SampleLighting") == 0)
		{
			m_pCurSample = new SampleLighting();
		}
		else if(strcmp(pSample,"SampleShadowMap") == 0)
		{
			m_pCurSample = new SampleShadowMap();
		}
		else if(strcmp(pSample,"SampleMaterial") == 0)
		{
			m_pCurSample = new SampleMaterial();
		}
		else if(strcmp(pSample,"SampleIBL") == 0)
		{
			m_pCurSample = new SampleIBL();
		}
		else if(strcmp(pSample,"SampleTerrain") == 0)
		{
			m_pCurSample = new SampleTerrain();
		}
		else if(strcmp(pSample,"SampleSceneSerialize") == 0)
		{
			m_pCurSample = new SampleSceneSerialize();
		}
		else if(strcmp(pSample,"SampleRigidBody") == 0)
		{
			m_pCurSample = new SampleRigidBody();
		}
		else if(strcmp(pSample,"SampleCharaControl") == 0)
		{
			m_pCurSample = new SampleCharaControl();
		}
		else if(strcmp(pSample,"SampleJoint") == 0)
		{
			m_pCurSample = new SampleJoint();
		}
		else if(strcmp(pSample,"SampleRagdoll") == 0)
		{
			m_pCurSample = new SampleRagdoll();
		}
		else if(strcmp(pSample,"SampleAnimationRetarget") == 0)
		{
			m_pCurSample = new SampleAnimationRetarget();
		}
		else if(strcmp(pSample,"SampleAnimationTree") == 0)
		{
			m_pCurSample = new SampleAnimationTree();
		}		
		else if(strcmp(pSample,"SampleAnimationIK") == 0)
		{
			m_pCurSample = new SampleAnimationIK();
		}
	
		m_pCurSample->Load();
	}


	void SampleBrowser::Update()
	{
		//Log("................. Update() ..................");

		if (GetInput())
			GetInput()->InjectInputEnd();

		Game::Update();

		if (m_pCameraControl)
			m_pCameraControl->Process(GetTimer()->GetFrameDeltaTime());

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
		if (m_pCurSample)
		{
			m_pCurSample->Render();
		}
	}

	void SampleBrowser::OnPosRender(Scene* pScene)
	{
		if (GetPhysicsSystem())
			GetPhysicsSystem()->DebugRender();
	}
}

