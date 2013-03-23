#include "Samples/stdafx.h"

#include "Samples/SimpleSceneView.hxx"

// Sample
#include "Samples/Serialize/SampleFbxImport.hxx"
#include "Samples/Serialize/SampleSceneSerialize.hxx"
#include "Samples/Animation/SampleAnimationRetarget.hxx"
#include "Samples/Physics/SampleRigidBody.hxx"
#include "Samples/Render/SampleLighting.hxx"
#include "Samples/Render/SampleShadowMap.hxx"

using namespace ma;

class SampleBrowser : public ApplicationWin
{
public:
	SampleBrowser(const char* pszName):ApplicationWin(pszName)
	{
		SampleFbxImport* pFbxImport = new SampleFbxImport();
		m_vSceneView.push_back(pFbxImport);

		SampleAnimationRetarget* pAnimation = new SampleAnimationRetarget();
		m_vSceneView.push_back(pAnimation);

		SampleRigidBody* pPhysics = new SampleRigidBody();
		m_vSceneView.push_back(pPhysics);

		SampleSceneSerialize* pSceneSerialize = new SampleSceneSerialize();
		m_vSceneView.push_back(pSceneSerialize);

		SampleLighting*	pLighting = new SampleLighting();
		m_vSceneView.push_back(pLighting);

		SampleShadowMap* pShadowMap = new SampleShadowMap();
		m_vSceneView.push_back(pShadowMap);

		m_nCurSceneInde = m_vSceneView.size() - 2;
	}

	virtual void Init(int argc, char* argv[])
	{
		__super::Init(argc,argv);

		if (m_vSceneView.empty())
			return;

		if (m_nCurSceneInde >= 0 && m_nCurSceneInde < m_vSceneView.size() )
		{
			SetSceneView(m_vSceneView[m_nCurSceneInde]);
		}
	}

	virtual void Update()
	{
		__super::Update();

		Input* pInput = GetInput();
		if (pInput == NULL)
			return;

		if (pInput->IsKeyPressed(OIS::KC_TAB))
		{
			if (m_vSceneView.empty())
				return;

			m_nCurSceneInde++;
			if (m_nCurSceneInde >= m_vSceneView.size())
				m_nCurSceneInde = 0;

			SetSceneView(m_vSceneView[m_nCurSceneInde]);
		}
	}

private:
	std::vector<SceneView*> m_vSceneView;
	int						m_nCurSceneInde;
};


int main(int argc, char* argv[])
{
	SampleBrowser app("SamleBrowser");
	app.Init(argc,argv);
	app.Run();
	app.Shutdown();
	return 0;
}

