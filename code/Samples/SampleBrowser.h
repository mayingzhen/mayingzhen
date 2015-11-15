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

		virtual void		Init();

		virtual void		Shutdown();

		virtual void		Update();

		//virtual void		Render();

 		//virtual void		controlEvent(Control* control, EventType evt);

		//virtual bool		keyPressed(const OIS::KeyEvent &arg);
		//virtual bool		keyReleased(const OIS::KeyEvent &arg);	

		virtual void		OnPreUpdate(Scene* pScene);
		virtual void		OnPostUpdate(Scene* pScene);
		virtual void		OnPreRender(Scene* pScene);
		virtual void		OnPosRender(Scene* pScene);

		CameraController*	GetCameraController() {return m_pCameraControl;}

		Camera*				GetCamera();

	private:
		void				LoadUI();

		void				RunSample(const char* pSampleNma);

		void				InitResourcePath();

		void				InitSampleList();

	private:
		std::map<std::string,Sample*>	m_arrSamples;

		Sample*							m_pCurSample;

		//Form*							m_pSampleSelectForm; 

		bool							m_bPause;
		bool							m_bStepOneFrame;

		CameraController*				m_pCameraControl;
	};

	SampleBrowser* GetSampleBrowser();
}



#endif