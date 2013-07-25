#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#include "CameraController.hxx"
#include "Sample.hxx"

#include "Samples/Serialize/SampleFbxImport.hxx"
#include "Samples/Serialize/SampleSceneSerialize.hxx"
#include "Samples/Physics/SampleRigidBody.hxx"
#include "Samples/Render/SampleTerrain.hxx"
#include "Samples/Animation/SampleAnimationRetarget.hxx"
#include "Samples/Animation/SampleAnimationTree.hxx"
#include "Samples/Render/SampleParticle.hxx"

namespace ma
{
	static SampleBrowser __sampleBrowser("SampleBrowser");

	SampleBrowser::SampleBrowser(const char* pGameName)
		:Game(pGameName)
	{
		SampleFbxImport* pSampleFbxImport = new SampleFbxImport();
		m_arrSamples.push_back(pSampleFbxImport);

		SampleSceneSerialize* pSceneSerial = new SampleSceneSerialize();
		m_arrSamples.push_back(pSceneSerial);

		SampleRigidBody* pSampleRigidBody = new SampleRigidBody();
		m_arrSamples.push_back(pSampleRigidBody);

		SampleTerrain* pSampleTerrain = new SampleTerrain();
		m_arrSamples.push_back(pSampleTerrain);

		SampleAnimationRetarget* pAnimRetar = new SampleAnimationRetarget();
		m_arrSamples.push_back(pAnimRetar);

		SampleAnimationTree* pAniTree = new SampleAnimationTree();
		m_arrSamples.push_back(pAniTree);

		SampleParticle* pParticle = new SampleParticle();
		m_arrSamples.push_back(pParticle);

		m_curSampleIndex = 2;

		SetTimer(&m_Timer);
		SetInput(&m_Input);

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
#elif PLAFTORM_ANDROID == 1
        
#endif
	
		CommonModuleInit();
		EngineModuleInit();
        AnimationModuleInit();
        BtPhysicsModuleInit();
		//D3D9RenderModuleInit();
		GLESRenderModuleInit();
		FramworkModuleInit();

		GetRenderDevice()->Init(Platform::GetInstance().GetWindId());

		LineRender::Init();

		m_Input.Init(Platform::GetInstance().GetWindId());
		m_Input.GetKeyboard()->setEventCallback(this);

		m_arrSamples[m_curSampleIndex]->Init();

		LoadUI();
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
		//m_pSampleSelectForm->setAutoHeight(true);
		//m_pSampleSelectForm->setWidth(200.0f);
		m_pSampleSelectForm->setSize(200.0f,(float)nWndHeigh);
		m_pSampleSelectForm->setScroll(Container::SCROLL_VERTICAL);
		m_pSampleSelectForm->setConsumeInputEvents(true);

		Button* sampleButton = Button::create("xxxx", buttonStyle);
		sampleButton->setText("xxxx");
		sampleButton->setAutoWidth(true);
		sampleButton->setHeight(60);      // Tall enough to touch easily on a BB10 device.
		sampleButton->setConsumeInputEvents(false);   // This lets the user scroll the container if they swipe starting from a button.
		sampleButton->addListener(this, Control::Listener::CLICK);
		m_pSampleSelectForm->addControl(sampleButton);

		{
			Button* sampleButton = Button::create("xxxxTT", buttonStyle);
			sampleButton->setText("xxxxTT");
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
		m_Input.Capture();

		m_Timer.UpdateFrame();

		if (m_bPause && !m_bStepOneFrame)
		{
			return;				
		}
		m_bStepOneFrame = false;

		Form::updateInternal(m_Timer.GetFrameDeltaTime());

        if (GetDataThread())
            GetDataThread()->Process();

		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->Update();
		

		//m_pSampleSelectForm->update(m_Timer.GetFrameDeltaTime());
	}

	void SampleBrowser::Render()
	{
		GetRenderDevice()->BeginRender();

		LineRender::BeginFrame();

		//RenderQueue::Clear();

		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->Render();

		//RenderQueue::Fulsh();

		m_pSampleSelectForm->draw();

		//m_pLineRender->Start();
		//LineRender::DrawBox(Matrix4x4::identity(),Vector3(5,5,5),Color(1,0,0,0));
		
		LineRender::EndFrame();
	
		GetRenderDevice()->EndRender();
	}

	void SampleBrowser::controlEvent(Control* control, EventType evt)
	{
		ASSERT(control);
		if (control == NULL)
			return;

		//control->setText()
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

