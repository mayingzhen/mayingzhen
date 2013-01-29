#ifndef __Application_h__
#define __Application_h__

#include "Application/Input/Input.h"
#include "Application/Timer/Timer.h"

namespace ma
{
	class SceneContext;

	typedef SceneContext* (*FuncCreateScene)(const char* pszName);
	
	class Application_API Application 
	{
	public:

		Application(const char* appID);

		virtual ~Application();

		virtual int			CreateMainWnd(int argc, char* argv[]) = 0;

		virtual void		Run() = 0;

		virtual const char*	GetAppTitle();

		virtual void		Init(int argc, char* argv[]);

		virtual void		RegisterSceneFactory(const char* sceneName,FuncCreateScene sceneFactory);

		virtual void		SetScene(const char* pszSceneName,int argc, char* argv[]);

		virtual SceneContext*	GetScene();

		virtual void		OnResize(int w,int h);

		virtual void 		Update();

		virtual void 		Render();

		virtual void 		Shutdown();

	protected:

		Time		m_appTime;

		Input		m_input;

		std::string m_strAppID;

		int			m_windId;

		std::string		m_curSceneName;
		SceneContext*	m_pSceneContext;
		typedef std::map<std::string,FuncCreateScene> SceneFactoryMap;
		SceneFactoryMap m_sceneMap;
	};
}

#endif //__Application_h__

