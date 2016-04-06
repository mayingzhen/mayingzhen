#ifndef _SimpleBrowser_H_
#define _SimpleBrowser_H_

namespace ma
{
	class Sample;
	class CameraController;
	class Camera;
	class Systems;
	class Sample;
	class Scene;

	typedef Sample* (*SampleCreator)();

	class SampleBrowser : public Game, Scene::CCallback
	{
	public:
		SampleBrowser(const char* pGameName);

		~SampleBrowser();

		virtual void		ModuleInit(RenderDeviceType eType);

		virtual void		ModuleShutdown();

		virtual void		Init(bool bRenderThread, bool bDataThread, bool bJobScheduler,const char* pszRunSample);

		virtual void		Shutdown();

		virtual void		Update();

		virtual void		OnPreUpdate(Scene* pScene);
		virtual void		OnPostUpdate(Scene* pScene);
		virtual void		OnPreRender(Scene* pScene);
		virtual void		OnPosRender(Scene* pScene);

		CameraController*	GetCameraController() {return m_pCameraControl;}

		Camera*				GetCamera();

	private:
		void				RunSample(const char* pSample);

		void				InitResourcePath();

	private:
		RefPtr<Sample>		m_pCurSample;

		bool				m_bPause;
		bool				m_bStepOneFrame;

		CameraController*	m_pCameraControl;
	};

	SampleBrowser* GetSampleBrowser();
}



#endif