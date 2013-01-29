#include "Application/Application.h"
#include "Application/SceneContext.h"

namespace ma
{
	Application::Application(const char* appID)
	{	
		m_strAppID = appID;
		m_pSceneContext = NULL;
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
		CommonModuleInit();
	
		m_windId = CreateMainWnd(argc,argv);

 		m_input.Init(m_windId);
 
 		SetInput(&m_input);
		
		SetTimer(&m_appTime);
	}

	void Application::RegisterSceneFactory(const char* sceneName,FuncCreateScene sceneFactory)
	{
		m_sceneMap[sceneName] = sceneFactory;
	}

	void Application::SetScene(const char* sceneName,int argc, char* argv[])
	{
		SceneFactoryMap::iterator iter = m_sceneMap.find(sceneName);
		if (iter != m_sceneMap.end())
		{
			if(NULL != m_pSceneContext)
			{
				m_pSceneContext->Unload();
				SAFE_DELETE(m_pSceneContext);
			}

			FuncCreateScene fCreateScene = iter->second;
			m_pSceneContext = fCreateScene(sceneName);
			m_pSceneContext->Init(argc,argv,m_windId);
			m_pSceneContext->Load();

			m_curSceneName = sceneName;
		}
	}

	SceneContext*	Application::GetScene()
	{
		return m_pSceneContext;
	}

	void Application::OnResize(int w,int h)
	{
		if (NULL != m_pSceneContext)
		{
			m_pSceneContext->OnResize(w,h);
		}
		
		m_input.OnResize(w,h);
	};

	void Application::Update()
	{
		m_input.CaptureInput();

		m_appTime.UpdateFrame();

		if (m_pSceneContext)
		{
			m_pSceneContext->Tick(m_appTime.GetFrameDeltaTime());
		}
	}

	void Application::Render()
	{
		if (NULL != m_pSceneContext)
		{
			m_pSceneContext->Render();
		}
	}

	void Application::Shutdown()
	{
		if (NULL != m_pSceneContext)
		{
			m_pSceneContext->Unload();
			m_pSceneContext->Shutdown();
			SAFE_DELETE(m_pSceneContext);
		}

		SetTimer(NULL);

		SetInput(NULL);

		CommonModuleShutdown();
	}
}

