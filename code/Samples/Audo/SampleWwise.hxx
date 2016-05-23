#include "SampleWwise.h"

namespace ma
{
	SampleWwise::SampleWwise()
	{
	}

	void SampleWwise::Load()
	{
		Vector3 vEyePos = Vector3(0, -6, 2);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->LookAt(vEyePos,VAtPos);

		CreateWwiseBank("Human.bnk");

		RefPtr<SceneNode> pScenNode = m_pScene->CreateSceneNode();
		m_pWwiseComp = CreateWwiseComponent("test");
		pScenNode->AddComponent(m_pWwiseComp.get());
	
		m_pWwiseComp->PostEvent("Play_Hello");
	
	}

	void SampleWwise::UnLoad()
	{		
		m_pWwiseComp = NULL;
	}

	void SampleWwise::Update()
	{

	}

	void SampleWwise::Render()
	{

	}

	void SampleWwise::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{
		if (evt != Keyboard::KEY_PRESS)
			return;

		if (key == Keyboard::KEY_D)
		{

		}
	}
}


