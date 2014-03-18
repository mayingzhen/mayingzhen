#include "Samples/stdafx.h"
#include "SampleBrowser.h"

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
//#include "Samples/Physics/SampleRagdoll.hxx"
#include "Samples/Render/SampleTerrain.hxx"
#include "Samples/Animation/SampleAnimationRetarget.hxx"
#include "Samples/Animation/SampleAnimationTree.hxx"
#include "Samples/Render/SampleParticle.hxx"
//#include "Samples/Serialize/SampleS3Import.hxx"

#if PLATFORM_WIN != 1
#include "Animation/Module.h"
#include "GLESRender/Module.h"
#include "BulletPhysics/Module.h"
#endif


namespace ma
{
	//static SampleBrowser __sampleBrowser("SampleBrowser");

	SampleBrowser* GetSampleBrowser()
	{
		return (SampleBrowser*)&Game::GetInstance();
	}

	SampleBrowser::SampleBrowser(const char* pGameName)
		:Game(pGameName)
	{
#if PLATFORM_WIN == 1
		m_arrSamples["FbxImport"] = new SampleFbxImport();
		//m_arrSamples["S3Import"] = new SampleS3Import();

		m_arrSamples["CSharpScript"] = new SampleMonoScript();
		
		m_arrSamples["Lighting"] = new SampleLighting();
		m_arrSamples["ShadowMap"] = new SampleShadowMap();
#endif

		m_arrSamples["Terrain"] = new SampleTerrain();

		m_arrSamples["Particle"] = new SampleParticle();

		m_arrSamples["SceneSerialize"] = new SampleSceneSerialize();
		
		// Physics
		m_arrSamples["RigidBody"] = new SampleRigidBody();
		m_arrSamples["CharControl"] = new SampleCharaControl();
		m_arrSamples["PhysicsJoint"] = new SampleJoint();
		//m_arrSamples["Ragdoll"] = new SampleRagdoll();
		
		// Animation
		m_arrSamples["AnimationRetarget"] = new SampleAnimationRetarget();
		m_arrSamples["AnimationTree"] = new SampleAnimationTree();

		m_pCurSample = m_arrSamples["AnimationRetarget"];

		m_bPause = false;
		m_bStepOneFrame = false;

		m_pSystems = new Systems();
	}

	void SampleBrowser::InitResourcePath()
	{
#if PLATFORM_WIN == 1
		char pszPath[MAX_PATH] = {0};
		GetFullPathName("../../Data/",MAX_PATH,pszPath,NULL);
		FileSystem::setResourcePath(pszPath);
#elif PLAFTORM_IOS == 1
		std::string sDataDir = Platform::GetInstance().GetAppPath();
		sDataDir += "/data/";
		FileSystem::setResourcePath(sDataDir.c_str());
#elif PLATFORM_ANDROID == 1
		FileSystem::setResourcePath("/sdcard/MyData/Data/");    
#endif
	}

	void SampleBrowser::InitModule()
	{
		EngineModuleInit();
		RenderModuleInit();
		UIModuleInit();
		
		LoadPlugin();
	}

	void SampleBrowser::LoadRenderScheme()
	{
	#if PLATFORM_WIN == 1
		std::string configPath = FileSystem::getFullPath("config/RenderScheme_PC.xml");
	#else 
		std::string configPath = FileSystem::getFullPath("config/RenderScheme_mobile.xml");
	#endif

		RenderScheme* pRenderScheme = new RenderScheme();

		GetRenderSystem()->SetRenderScheme(pRenderScheme);

		TiXmlDocument doc;
		bool bLoadOK = doc.LoadFile(configPath.c_str());
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return;

		TiXmlElement* pRootElem = doc.RootElement();
		ASSERT(pRootElem);
		if (pRootElem == NULL)
			return;

		TiXmlElement* pElePass = pRootElem->FirstChildElement("Pass");
		while(pElePass)
		{
			const char* pszPassName = pElePass->Attribute("name");

			RenderPass* pRenderPass = CreateObject<RenderPass>(pszPassName);

			pRenderScheme->AddRenderPass(pRenderPass);

			pElePass = pElePass->NextSiblingElement();
		}
	}
	
	void SampleBrowser::LoadPlugin()
	{
#if PLATFORM_WIN == 1

		typedef bool (*DLL_START_PLUGIN)(void);

		std::string configPath = FileSystem::getFullPath("config/Plugins.xml");
		TiXmlDocument doc;
		bool bLoadOK = doc.LoadFile(configPath.c_str());
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return;

		TiXmlElement* pRootElem = doc.RootElement();
		ASSERT(pRootElem);
		if (pRootElem == NULL)
			return;

		TiXmlElement* pElePlugin = pRootElem->FirstChildElement("Plugin");
		while(pElePlugin)
		{
			const char* pszPath = pElePlugin->Attribute("path");
			
			HMODULE hInst = LoadLibraryExA(pszPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
			DLL_START_PLUGIN pFunPtr = (DLL_START_PLUGIN)GetProcAddress(hInst,"dllStartPlugin");	
			ASSERT(pFunPtr);
			if (pFunPtr)
				pFunPtr();

			pElePlugin = pElePlugin->NextSiblingElement();
		}
#else
		AnimationModuleInit();
		GLESRenderModuleInit();
		BtPhysicsModuleInit();
#endif

	}

	void SampleBrowser::Init()
	{
		InitResourcePath();

		InitModule();

		LoadRenderScheme();

		m_pSystems->Init();
	
		GetInput()->AddKeyListener(this);

		m_pCameraControl = new CameraController( GetCamera() );
		
		ResetCamera();

		LoadUI();

		if (m_pCurSample)
		{
			m_pCurSample->Load();
		}

		m_pSystems->Start();
	}

	void SampleBrowser::ResetCamera()
	{
		Vector3 vEyePos = Vector3(0, 200, 300);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = PI / 4;
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 1.0f;
		float fFarClip = 20000.0f;
		GetCamera()->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);

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

			ResetCamera();

			GetEntitySystem()->Reset();

			m_pSystems->Stop();
		}

		Sample* pSameple = it->second;
		pSameple->Load();
		m_pCurSample = pSameple;

		m_pSystems->Start();
	}

	void SampleBrowser::LoadUI()
	{
		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);

		Theme* theme =  Theme::create("ui/default.theme");
		Theme::Style* formStyle = theme->getStyle("basicContainer");
		Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
		Theme::Style* titleStyle = theme->getStyle("title");	

		UISystem* pUISystem = (UISystem*)GetUISystem();

		m_pSampleSelectForm = pUISystem->Create("sampleSelect", formStyle, Layout::LAYOUT_VERTICAL);
		m_pSampleSelectForm->setSize(200.0f,(float)nWndHeigh);
		m_pSampleSelectForm->setScroll(Container::SCROLL_VERTICAL);
		m_pSampleSelectForm->setConsumeInputEvents(true);

		std::map<std::string,Sample*>::iterator it = m_arrSamples.begin();
		for (; it != m_arrSamples.end(); ++it)
		{
			const char* pSamepeName = it->first.c_str();
			Button* sampleButton = Button::create(pSamepeName, buttonStyle);
			sampleButton->setText(pSamepeName);
			sampleButton->setAutoWidth(true);
			sampleButton->setHeight(60);      
			sampleButton->setConsumeInputEvents(false);   
			sampleButton->addListener(this, Control::Listener::CLICK);
			m_pSampleSelectForm->addControl(sampleButton);
		}

	}

	void SampleBrowser::Shutdown()
	{
		
	}

	void SampleBrowser::Update()
	{
		//Log("................. Update() ..................");


		if (GetInput())
			GetInput()->Capture();

		m_pCameraControl->UpdateInput();

		if (m_bPause && !m_bStepOneFrame)
		{
			return;				
		}
		m_bStepOneFrame = false;
		
		m_pSystems->Update();
					
		if (m_pCurSample)
			m_pCurSample->Update();


		
	}

	Camera*	SampleBrowser::GetCamera()
	{
		return  GetRenderSystem()->GetMainCamera().get();
	}

	void SampleBrowser::Render()
	{
		//Log("................. Render() ..................");

		profile_code();

		GetRenderSystem()->BeginFrame();

		if ( GetStringRender() )
		{
			char buffer[MAX_PATH];
			sprintf(buffer, "%u", (UINT)(1.0f / GetTimer()->GetFrameDeltaTime()) );
			GetStringRender()->DrawScreenString(buffer,500,1,Vector4(1,1,1,1));
		}

		if (GetPhysicsSystem())
			GetPhysicsSystem()->DebugRender();

		if (m_pCurSample)
			m_pCurSample->Render();

		GetRenderSystem()->Render();

		GetRenderSystem()->EndFrame();
	}


	void SampleBrowser::controlEvent(Control* control, EventType evt)
	{
		Log("..............controlEvent.............");

		Button* pButton = static_cast<Button*>(control);
		if (pButton == NULL)
			return;

		const char* pText = pButton->getText();
		RunSample(pText);
	}

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
}

