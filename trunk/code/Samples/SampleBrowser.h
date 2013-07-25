#ifndef _SimpleBrowser_H_
#define _SimpleBrowser_H_

namespace ma
{
	class Sample;

	class SampleBrowser : public Game,Control::Listener,OIS::KeyListener
	{
	public:
		SampleBrowser(const char* pGameName);

		virtual void	Init();

		virtual void	Shutdown();

		virtual void	Update();

		virtual void	Render();

 		virtual void	controlEvent(Control* control, EventType evt);

		virtual bool	keyPressed(const OIS::KeyEvent &arg);

		virtual bool	keyReleased(const OIS::KeyEvent &arg);		

	private:
		void			LoadUI();

	private:
		std::vector<Sample*>	m_arrSamples;
		int						m_curSampleIndex;

		Time					m_Timer;
		Input					m_Input;

		Form*					m_pSampleSelectForm; 

		bool					m_bPause;
		bool					m_bStepOneFrame;
	};
}



#endif