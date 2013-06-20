#include "Samples/stdafx.h"
#include "SampleBrowser.h"

#include "Sample.hxx"

#include "Samples/Serialize/SampleFbxImport.hxx"

namespace ma
{
	static SampleBrowser __sampleBrowser;

	SampleBrowser::SampleBrowser()
	{
		SampleFbxImport* pSampleFbxImport = new SampleFbxImport();
		m_arrSamples.push_back(pSampleFbxImport);

		m_curSampleIndex = 0;

		SetTimer(&m_Timer);
	}

	void SampleBrowser::Init(const Platform* pPlatform)
	{
		FileSystem::setResourcePath("../../Data/");

		CommonModuleInit();
		EngineModuleInit();
		//D3D9RenderModuleInit();
		GLESRenderModuleInit();
		//AnimationModuleInit();

		GetRenderDevice()->Init(pPlatform->GetWindId());

		m_arrSamples[m_curSampleIndex]->Init(pPlatform);

// 		m_pDefaultTech = GetRenderDevice()->CreateTechnique();
// 		m_pDefaultTech->Load(NULL,"Default");
// 
// 		ASSERT(pApplication);
// 		if (pApplication == NULL)
// 			return;
// 
// 		m_pScene = new Scene();
// 		m_pCamera = new Camera();
// 		m_pScene->GetRootNode()->AddChildNode(m_pCamera);
// 
// 
// 		Vector3 vEyePos = Vector3(0, 200, 300);
// 		Vector3 VAtPos = Vector3(0,0,0); 
// 		Vector3 vUp = Vector3(0,1,0);
// 		m_pCamera->LookAt(vEyePos,VAtPos,vUp);
// 
 		int nWndWidth,nWndHeigh;
 		pPlatform->GetWindowSize(nWndWidth,nWndHeigh);
// 		float fFOV = PI / 4;
// 		float fAspect = (float)nWndWidth / (float)nWndHeigh;
// 		float fNearClip = 1.0f;
// 		float fFarClip = 3000.0f;
// 		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);


		Theme* theme = Theme::create("ui/default.theme");
		Theme::Style* formStyle = theme->getStyle("basicContainer");
		Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
		Theme::Style* titleStyle = theme->getStyle("title");	
		
		m_pSampleSelectForm = Form::create("sampleSelect", formStyle, Layout::LAYOUT_VERTICAL);
		//m_pSampleSelectForm->setAutoHeight(true);
		//m_pSampleSelectForm->setWidth(200.0f);
		m_pSampleSelectForm->setSize(200.0f,nWndHeigh);
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
		m_Timer.UpdateFrame();

		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->Update();
		

		m_pSampleSelectForm->update(m_Timer.GetFrameDeltaTime());
	}

	void SampleBrowser::Render()
	{
		GetRenderDevice()->BeginRender();

		//if (m_arrSamples[m_curSampleIndex])
		//	m_arrSamples[m_curSampleIndex]->Render();

		m_pSampleSelectForm->draw();
	
		GetRenderDevice()->EndRender();
	}

	///// Input
	void SampleBrowser::keyEvent(Keyboard::KeyEvent evt, int key)
	{
		
	}

	void SampleBrowser::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
	{

	}

	bool SampleBrowser::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
	{
		if ( Form::mouseEventInternal(evt,x,y,wheelDelta) )
			return true;
	
		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->mouseEvent(evt,x,y,wheelDelta);

		return true;
	}

	void SampleBrowser::resizeEvent(unsigned int width, unsigned int height)
	{

	}

	void SampleBrowser::controlEvent(Control* control, EventType evt)
	{
		ASSERT(control);
		if (control == NULL)
			return;

		//control->setText()
	}
}

