#include "Samples/stdafx.h"

#include "Samples/FbxImport/FbxImport.hxx"

using namespace ma;

class SampleBrowser : public ApplicationWin
{
public:
	SampleBrowser(const char* pszName):ApplicationWin(pszName)
	{
		SampleFbxImport* pFbxImport = new SampleFbxImport();
		m_vSceneView.push_back(pFbxImport);

		m_nCurSceneInde = 0;
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

		if (pInput->IsKeyDown(OIS::KC_TAB))
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
