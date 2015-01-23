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
#include "S3Serialize/Module.h"
#endif

#include "CameraController.hxx"
#include "Sample.hxx"

#if PLATFORM_WIN == 1
//#include "Samples/Serialize/SampleFbxImport.hxx"
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

//#include "Samples/Serialize/SampleS3Import.hxx"


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

		ModuleInit();
	}

	SampleBrowser::~SampleBrowser()
	{
		ModuleShutdown();
	}

	void SampleBrowser::ModuleInit()
	{

#if PLATFORM_WIN == 1
		D3D9RenderModuleInit();
		//GLESRenderModuleInit();
		MonoScriptModuleInit();
		BtPhysicsModuleInit();
		AnimationModuleInit();
		//FBXImporterModuleInit();

#else
		AnimationModuleInit();
		GLESRenderModuleInit();
		BtPhysicsModuleInit();
#endif
	}

	void SampleBrowser::ModuleShutdown()
	{
#if PLATFORM_WIN == 1
		//FBXImporterModuleShutdown();
		AnimationModuleShutdown();
		BtPhysicsModuleShutdown();
		MonoScriptModuleShutdown();
		D3D9RenderModuleShutdown();
		//GLESRenderModuleShutdown();
#else
		AnimationModuleShutdown();
		GLESRenderModuleShutdown();
		BtPhysicsModuleShutdown();
#endif	

	}

	void SampleBrowser::InitSampleList()
	{
#if PLATFORM_WIN == 1
		//m_arrSamples["FbxImport"] = new SampleFbxImport();

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

		//m_arrSamples["SampleS3Import"] = new SampleS3Import();

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
			GetPhysicsSystem()->Stop();
			GetScriptSystem()->Stop();
		}

		SAFE_DELETE(m_pCameraControl);
		
		GetInput()->RemoveKeyListener(this);
		
		std::map<std::string,Sample*>::iterator it = m_arrSamples.begin();
		for (; it != m_arrSamples.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		m_arrSamples.clear();
		
		GetPhysicsSystem()->ShoutDown();
		GetScriptSystem()->ShutDown();

		Game::Shutdown();
	}

	void SampleBrowser::Init()
	{
		InitResourcePath();
		
		Game::Init();

		GetPhysicsSystem()->Init();
		GetScriptSystem()->Init();

		GetInput()->AddKeyListener(this);
		
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
			GetPhysicsSystem()->Stop();
			GetScriptSystem()->Stop();

			m_pCameraControl->ResetCamera();

 			m_pCurSample->GetScene()->Reset();
		}
		
		Sample* pSameple = it->second;

		pSameple->Load();
		GetPhysicsSystem()->Start();
		GetScriptSystem()->Start();

		m_pCurSample = pSameple;
	}

// 	void SampleBrowser::LoadUI()
// 	{
// 		int nWndWidth,nWndHeigh;
// 		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
// 
// 		Theme* theme =  Theme::create("ui/default.theme");
// 		Theme::Style* formStyle = theme->getStyle("basicContainer");
// 		Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
// 		Theme::Style* titleStyle = theme->getStyle("title");	
// 
// 		UISystem* pUISystem = (UISystem*)GetUISystem();
// 
// 		m_pSampleSelectForm = pUISystem->Create("sampleSelect", formStyle, Layout::LAYOUT_VERTICAL);
// 		m_pSampleSelectForm->setSize(200.0f,(float)nWndHeigh);
// 		m_pSampleSelectForm->setScroll(Container::SCROLL_VERTICAL);
// 		m_pSampleSelectForm->setConsumeInputEvents(true);
// 
// 		std::map<std::string,Sample*>::iterator it = m_arrSamples.begin();
// 		for (; it != m_arrSamples.end(); ++it)
// 		{
// 			const char* pSamepeName = it->first.c_str();
// 			Button* sampleButton = Button::create(pSamepeName, buttonStyle);
// 			sampleButton->setText(pSamepeName);
// 			sampleButton->setAutoWidth(true);
// 			sampleButton->setHeight(60);      
// 			sampleButton->setConsumeInputEvents(false);   
// 			sampleButton->addListener(this, Control::Listener::CLICK);
// 			m_pSampleSelectForm->addControl(sampleButton);
// 		}
// 
// 	}

	void SampleBrowser::Update()
	{
		//Log("................. Update() ..................");

		Game::Update();

		m_pCameraControl->UpdateInput();

		if (m_bPause && !m_bStepOneFrame)
		{
			return;				
		}
		m_bStepOneFrame = false;

					
		if (m_pCurSample)
			m_pCurSample->Update();
	
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

	bool SampleBrowser::keyPressed(const OIS::KeyEvent &arg)
	{
// 		if (arg.key == OIS::KC_S)
// 		{
// 			m_pSystems->Start();
// 		}

		if (arg.key == OIS::KC_P)
		{
			m_bPause = !m_bPause;
			if (m_bPause)
				m_bStepOneFrame = false;
		}
		
		if (arg.key == OIS::KC_N)
		{
			m_bStepOneFrame = true;
		}

		return false;
	}

	bool SampleBrowser::keyReleased(const OIS::KeyEvent &arg)
	{
		return false;
	}

	void SampleBrowser::OnPreUpdate(Scene* pScene)
	{
		GetPhysicsSystem()->BeginUpdate();
	}

	void SampleBrowser::OnPostUpdate(Scene* pScene)
	{
		GetPhysicsSystem()->EndUpdate();
	}

	void SampleBrowser::OnPreRender(Scene* pScene)
	{

	}

	void SampleBrowser::OnPosRender(Scene* pScene)
	{
// 		if (GetPhysicsSystem())
// 			GetPhysicsSystem()->DebugRender();
// 
// 		if (m_pCurSample)
// 			m_pCurSample->Render();

		//GetUISystem()->Render();
	}
}

