#include "Samples/Render/SampleGI.h"
#include "Application/ui.h"

namespace ma
{
	SampleGI::SampleGI()
	{
	}




	void SampleGI::Load()
	{

		GetCamera()->LookAt(Vector3(0, 1.5, -5.5), Vector3(0, 1.5, -0.5));

		RefPtr<SceneNode> pNode = m_pScene->CreateSceneNode();
		RefPtr<ScreenRender> pRenderCom = CreateScreenRender();
		pRenderCom->SetTech("shader/gi.tech");
		pNode->AddComponent(pRenderCom.get());
	
	}

	void SampleGI::UnLoad()
	{

	}


	void SampleGI::Update()
	{


	}

	void SampleGI::PreRender()
	{

	}

	void SampleGI::PostRender()
	{
	}

	void SampleGI::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;
	}


}



