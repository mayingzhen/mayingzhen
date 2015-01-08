#ifndef _SimpleBrowser_H_
#define _SimpleBrowser_H_

namespace ma
{
	class Sample;
	class CameraController;
	class Camera;
	class Systems;
	class Sample;

	typedef Sample* (*SampleCreator)();

	class SampleBrowser : public Game, /*Control::Listener,*/OIS::KeyListener
	{
	public:
		SampleBrowser(const char* pGameName);

		~SampleBrowser();

		virtual void		Init();

		virtual void		Shutdown();

		virtual void		Update();

		virtual void		Render();

 		//virtual void		controlEvent(Control* control, EventType evt);

		virtual bool		keyPressed(const OIS::KeyEvent &arg);

		virtual bool		keyReleased(const OIS::KeyEvent &arg);	

		CameraController*	GetCameraController() {return m_pCameraControl;}

		Camera*				GetCamera();

	private:
		void				LoadUI();

		void				ResetCamera();

		void				RunSample(const char* pSampleNma);

		void				InitResourcePath();

		void				InitBaseModule();
		void				UnInitBaseModule();

		void				LoadPlugin();
		void				UnLoadPlugin();

		void				LoadRenderScheme();

		void				InitSampleList();

	private:
		std::map<std::string,Sample*>	m_arrSamples;

		Sample*							m_pCurSample;

		//Form*							m_pSampleSelectForm; 

		bool							m_bPause;
		bool							m_bStepOneFrame;

		CameraController*				m_pCameraControl;

		Systems*						m_pSystems;
	};

	SampleBrowser* GetSampleBrowser();
}



#endif