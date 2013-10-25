#ifndef _SimpleBrowser_H_
#define _SimpleBrowser_H_

namespace ma
{
	class Sample;
	class CameraController;
	class Camera;
	class Systems;

	class SampleBrowser : public Game,Control::Listener,OIS::KeyListener
	{
	public:
		SampleBrowser(const char* pGameName);

		virtual void		Init();

		virtual void		Shutdown();

		virtual void		Update();

		virtual void		Render();

 		virtual void		controlEvent(Control* control, EventType evt);

		virtual bool		keyPressed(const OIS::KeyEvent &arg);

		virtual bool		keyReleased(const OIS::KeyEvent &arg);	

		CameraController*	GetCameraController() {return m_pCameraControl;}

		//Camera*				GetCamera() {return m_pCamera;}

	private:
		void				LoadUI();

		//void				InitCamera();

		void				ResetCamera();

		void				RunSample(const char* pSampleNma);

		void				InitResourcePath();

		void				InitModule();

		void				LoadPlugin();

	private:
		std::map<std::string,Sample*>	m_arrSamples;

		Sample*							m_pCurSample;

		Form*							m_pSampleSelectForm; 

		bool							m_bPause;
		bool							m_bStepOneFrame;

		CameraController*				m_pCameraControl;
		//Camera*							m_pCamera;

		Systems*						m_pSystems;
	};

	SampleBrowser* GetSampleBrowser();
}



#endif