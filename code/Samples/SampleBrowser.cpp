#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#include "Animation/Module.h"
#include "GLESRender/Module.h"
#include "BulletPhysics/Module.h"
#include "MonoScript/Module.h"

#if PLATFORM_WIN == 1
#include "D3D11Render/Module.h"
#endif

#include "CameraController.hxx"
#include "Sample.hxx"

#if PLATFORM_WIN == 1
#include "Samples/Serialize/SampleFbxImport.hxx"
#endif
#include "Samples/Script/SampleMonoScript.hxx"
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
	}

	SampleBrowser::~SampleBrowser()
	{
	}

	void SampleBrowser::ModuleInit(RenderDeviceType eType)
	{
		AnimationModuleInit();
		BtPhysicsModuleInit();
		MonoScriptModuleInit();

#if PLATFORM_WIN == 1
		if (eType == RenderDevice_D3D11)
		{
			D3D11RenderModuleInit();
		}
		else
		{
			GLESRenderModuleInit();
		}

		FBXImporterModuleInit();
#else
		GLESRenderModuleInit();		
#endif
	}

	void SampleBrowser::ModuleShutdown()
	{
		AnimationModuleShutdown();
		BtPhysicsModuleShutdown();
		MonoScriptModuleShutdown();

#if PLATFORM_WIN == 1
		FBXImporterModuleShutdown();
		
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_D3D11)
		{
			D3D11RenderModuleShutdown();
		}
		else
		{
			GLESRenderModuleShutdown();
		}
#else

		GLESRenderModuleShutdown();
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

			if (GetScriptSystem())
				GetScriptSystem()->Stop();
		}
		m_pCurSample = NULL;

		SAFE_DELETE(m_pCameraControl);
		
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Shoutdown();

		if (GetScriptSystem())
			GetScriptSystem()->Shoutdown();

		Game::Shutdown();

		ModuleShutdown();
	}

	void SampleBrowser::Init(bool bForceGLES, bool bRenderThread, bool bDataThread, bool bJobScheduler,const char* pszRunSample)
	{
		if (bForceGLES)
		{
			ModuleInit(RenderDevice_GLES2);
		}
		else
		{
			ModuleInit(RenderDevice_D3D11);	
		}
		
		InitResourcePath();

		Game::Init(bRenderThread,bDataThread,bJobScheduler);
	
		if (GetPhysicsSystem())
			GetPhysicsSystem()->Init();

		if (GetScriptSystem())
			GetScriptSystem()->Init();
			
		HWND hWnd = Platform::GetInstance().GetWindId();
		
		Scene* pScene = GetRenderSystem()->GetScene();
		pScene->SetCallback(this);

		m_pCameraControl = new CameraController( pScene->GetCamera() );

		RunSample(pszRunSample);

		if (GetPhysicsSystem())
			GetPhysicsSystem()->Start();

		if (GetScriptSystem())
			GetScriptSystem()->Start();
	}

	void SampleBrowser::RunSample(const char* pSample)
	{
		if (stricmp(pSample,"SampleFbxImport") == 0)
		{
			m_pCurSample = new SampleFbxImport();
		}
		else if (stricmp(pSample,"SampleMonoScript") == 0)
		{
			m_pCurSample = new SampleMonoScript();
		}
		else if (stricmp(pSample,"SampleLighting") == 0)
		{
			m_pCurSample = new SampleLighting();
		}
		else if(stricmp(pSample,"SampleShadowMap") == 0)
		{
			m_pCurSample = new SampleShadowMap();
		}
		else if(stricmp(pSample,"SampleMaterial") == 0)
		{
			m_pCurSample = new SampleMaterial();
		}
		else if(stricmp(pSample,"SampleIBL") == 0)
		{
			m_pCurSample = new SampleIBL();
		}
		else if(stricmp(pSample,"SampleTerrain") == 0)
		{
			m_pCurSample = new SampleTerrain();
		}
		else if(stricmp(pSample,"SampleParticle") == 0)
		{
			m_pCurSample = new SampleParticle();
		}
		else if(stricmp(pSample,"SampleSceneSerialize") == 0)
		{
			m_pCurSample = new SampleSceneSerialize();
		}
		else if(stricmp(pSample,"SampleRigidBody") == 0)
		{
			m_pCurSample = new SampleRigidBody();
		}
		else if(stricmp(pSample,"SampleCharaControl") == 0)
		{
			m_pCurSample = new SampleCharaControl();
		}
		else if(stricmp(pSample,"SampleJoint") == 0)
		{
			m_pCurSample = new SampleJoint();
		}
		else if(stricmp(pSample,"SampleRagdoll") == 0)
		{
			m_pCurSample = new SampleRagdoll();
		}
		else if(stricmp(pSample,"SampleAnimationRetarget") == 0)
		{
			m_pCurSample = new SampleAnimationRetarget();
		}
		else if(stricmp(pSample,"SampleAnimationTree") == 0)
		{
			m_pCurSample = new SampleAnimationTree();
		}		
		else if(stricmp(pSample,"SampleAnimationIK") == 0)
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

