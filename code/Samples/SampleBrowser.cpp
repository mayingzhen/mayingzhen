#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#include "Systems.hxx"
#include "CameraController.hxx"
#include "Sample.hxx"

#if PLATFORM_WIN == 1
#include "Samples/Serialize/SampleFbxImport.hxx"
//#include "Samples/Script/SampleMonoScript.hxx"
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
	static SampleBrowser __sampleBrowser("SampleBrowser");

	SampleBrowser* GetSampleBrowser()
	{
		return &__sampleBrowser;
	}

	SampleBrowser::SampleBrowser(const char* pGameName)
		:Game(pGameName)
	{
#if PLATFORM_WIN == 1
		SampleFbxImport* pSampleFbxImport = new SampleFbxImport();
		m_arrSamples["FbxImport"] = pSampleFbxImport;

		//SampleMonoScript* pSampleScript = new SampleMonoScript();
		//m_arrSamples["CSharpScript"] = pSampleScript;
#endif

		SampleSceneSerialize* pSceneSerial = new SampleSceneSerialize();
		m_arrSamples["SceneSerialize"] = pSceneSerial;

		SampleRigidBody* pSampleRigidBody = new SampleRigidBody();
		m_arrSamples["RigidBody"] = pSampleRigidBody;

		SampleCharaControl* pSampleCharControl = new SampleCharaControl();
		m_arrSamples["CharControl"] = pSampleCharControl;

		SampleJoint* pSampleJoint = new SampleJoint();
		m_arrSamples["PhysicsJoint"] = pSampleJoint;

		//SampleRagdoll* pSampleRagdoll = new SampleRagdoll();
		//m_arrSamples["Ragdoll"] = pSampleRagdoll;

		SampleTerrain* pSampleTerrain = new SampleTerrain();
		m_arrSamples["Terrain"] = pSampleTerrain;

		SampleAnimationRetarget* pSampleAnimRetar = new SampleAnimationRetarget();
		m_arrSamples["AnimationRetarget"] = pSampleAnimRetar;

		SampleAnimationTree* pSampleAniTree = new SampleAnimationTree();
		m_arrSamples["AnimationTree"] = pSampleAniTree;

		SampleParticle* pParticle = new SampleParticle();
		m_arrSamples["Particle"] = pParticle;

		m_pCurSample = pSampleAnimRetar;

		m_bPause = false;
		m_bStepOneFrame = false;
	}

	void SampleBrowser::Init()
	{
#if PLATFORM_WIN == 1
		FileSystem::setResourcePath("../../Data/");
#elif PLAFTORM_IOS == 1
		std::string sDataDir = Platform::GetInstance().GetAppPath();
		sDataDir += "/Data/";
		FileSystem::setResourcePath(sDataDir.c_str());
#elif PLATFORM_ANDROID == 1
		FileSystem::setResourcePath("/sdcard/MyData/Data/");    
#endif
	
		CommonModuleInit();
		D3D9RenderModuleInit();
		//GLESRenderModuleInit();
		EngineModuleInit();
        AnimationModuleInit();
        BtPhysicsModuleInit();
		EntitySystemModuleInit();

		m_pSystems = new Systems();
		GetInput()->AddKeyListener(this);

		InitCamera();

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
		m_pCamera->LookAt(vEyePos,VAtPos,vUp);

		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = PI / 4;
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 1.0f;
		float fFarClip = 30000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);

	}

	void SampleBrowser::InitCamera()
	{
		m_pCamera = new Camera();
		m_pCameraControl = new CameraController(m_pCamera);

		GetRenderSystem()->SetCamera(m_pCamera);

		ResetCamera();
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

			m_pSystems->Stop();

			GetEntitySystem()->DeleteAll();
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

		Theme* theme = Theme::create("ui/default.theme");
		Theme::Style* formStyle = theme->getStyle("basicContainer");
		Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
		Theme::Style* titleStyle = theme->getStyle("title");	

		m_pSampleSelectForm = Form::create("sampleSelect", formStyle, Layout::LAYOUT_VERTICAL);
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
			sampleButton->setHeight(60);      // Tall enough to touch easily on a BB10 device.
			sampleButton->setConsumeInputEvents(false);   // This lets the user scroll the container if they swipe starting from a button.
			sampleButton->addListener(this, Control::Listener::CLICK);
			m_pSampleSelectForm->addControl(sampleButton);
		}

	}

	void SampleBrowser::Shutdown()
	{
		
	}

	void SampleBrowser::Update()
	{
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

	void SampleBrowser::Render()
	{
		GetRenderSystem()->BeginFrame();

		if (m_pCurSample)
			m_pCurSample->Render();

		GetPhysicsSystem()->DebugRender();

		GetRenderSystem()->Flush();
		
		LineRender::Flush();

		m_pSampleSelectForm->draw();
		
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

