#ifndef __Application_h__
#define __Application_h__

#include "Engine/Input/Input.h"
#include "Engine/Timer/Timer.h"

namespace ma
{
	class SceneView;

	class ENGINE_API Application 
	{
	public:

		Application(const char* appID);

		virtual ~Application();

		virtual int			CreateMainWnd(int argc, char* argv[]) = 0;

		virtual void		Run() = 0;

		virtual	void		GetWindowSize(int& w, int& h) = 0;

		virtual	int			GetWindID() {return m_windId;}

		virtual const char*	GetAppTitle();

		virtual void		Init(int argc, char* argv[]);

		virtual void		SetSceneView(SceneView* pSceneView);

		virtual SceneView*	GetSceneView();

		virtual void		OnResize(int w,int h);

		virtual void 		Update();

		virtual void 		Render();

		virtual void 		Shutdown();

	protected:

		Time		m_appTime;

		Input		m_input;

		std::string m_strAppID;

		int			m_windId;

		SceneView*	m_pSceneView;
	};
}

#endif //__Application_h__

