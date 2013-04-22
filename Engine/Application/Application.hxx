#include "Application/Application.h"
#include "Application/SceneView.h"

namespace ma
{
	Application::Application(const char* appID)
	{	
		m_strAppID = appID;
		m_pSceneView = NULL;
	}

	Application::~Application()
	{

	}

	const char*	Application::GetAppTitle()
	{
		return m_strAppID.c_str();
	}

	void Application::Init(int argc, char* argv[])
	{
		m_windId = CreateMainWnd(argc,argv);

 		m_input.Init(m_windId);
 
 		SetInput(&m_input);
		
		SetTimer(&m_appTime);
	}

	void Application::Shutdown()
	{
		if (NULL != m_pSceneView)
		{
			m_pSceneView->Unload();
			m_pSceneView->Shutdown();
			SAFE_DELETE(m_pSceneView);
		}

		SetTimer(NULL);

		SetInput(NULL);
	}

	void Application::SetSceneView(SceneView* pSceneView)
	{
		if (pSceneView == NULL)
			return;

		if (m_pSceneView == pSceneView)
			return;

		if (m_pSceneView)
		{
			m_pSceneView->Unload();
		}

		m_pSceneView = pSceneView;
		m_pSceneView->Init(this);
		m_pSceneView->Load();
	}

	SceneView*	Application::GetSceneView()
	{
		return m_pSceneView;
	}

	void Application::OnResize(int w,int h)
	{
		if (NULL != m_pSceneView)
		{
			m_pSceneView->OnResize(w,h);
		}
		
		m_input.OnResize(w,h);
	};

	void Application::Update()
	{
		m_input.CaptureInput();

		m_appTime.UpdateFrame();

		if (m_pSceneView)
		{
			m_pSceneView->Tick(m_appTime.GetFrameDeltaTime());
		}
	}

	void Application::Render()
	{
		if (NULL != m_pSceneView)
		{
			m_pSceneView->Render();
		}
	}
}

