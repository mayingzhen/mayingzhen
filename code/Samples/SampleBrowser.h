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
		SampleBrowser(const char* pGameName,const Setting& set,const char* pszSample);

		~SampleBrowser();

		virtual void		Init();

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

		void				ModuleInit();

		void				ModuleShutdown();

	private:
		RefPtr<Sample>		m_pCurSample;

		bool				m_bPause;
		bool				m_bStepOneFrame;

		CameraController*	m_pCameraControl;
        
        std::string         m_strSample;
	};

	SampleBrowser* GetSampleBrowser();
}



#endif
