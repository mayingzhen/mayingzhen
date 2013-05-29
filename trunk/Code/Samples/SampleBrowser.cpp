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
		FileSystem::setResourcePath("../");

		CommonModuleInit();
		EngineModuleInit();
		D3D9RenderModuleInit();
		//GLESRenderModuleInit();
		AnimationModuleInit();

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
// 		int nWndWidth,nWndHeigh;
// 		pApplication->GetWindowSize(nWndWidth,nWndHeigh);
// 		float fFOV = PI / 4;
// 		float fAspect = (float)nWndWidth / (float)nWndHeigh;
// 		float fNearClip = 1.0f;
// 		float fFarClip = 3000.0f;
// 		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	void SampleBrowser::Shutdown()
	{

	}

	void SampleBrowser::Update()
	{
		m_Timer.UpdateFrame();

		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->Update();
	}

	void SampleBrowser::Render()
	{
		GetRenderDevice()->BeginRender();

		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->Render();
	
		GetRenderDevice()->EndRender();
	}

	///// Input
	void SampleBrowser::keyEvent(Keyboard::KeyEvent evt, int key)
	{
		if (evt == Keyboard::KEY_PRESS && key == Keyboard::KEY_TAB)
		{
			int i = 5;
		}
	}

	void SampleBrowser::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
	{

	}

	bool SampleBrowser::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
	{
		if (m_arrSamples[m_curSampleIndex])
			m_arrSamples[m_curSampleIndex]->mouseEvent(evt,x,y,wheelDelta);

		return false;
	}

	void SampleBrowser::resizeEvent(unsigned int width, unsigned int height)
	{

	}
}

