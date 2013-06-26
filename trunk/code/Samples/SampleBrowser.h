#ifndef _SimpleBrowser_H_
#define _SimpleBrowser_H_

namespace ma
{
	class Sample;

	class SampleBrowser : public Game,Control::Listener
	{
	public:
		SampleBrowser();

		virtual void Init(const Platform* pPlatform);

		virtual void Shutdown();

		virtual void Update();

		virtual void Render();

		///// Input
		virtual void keyEvent(Keyboard::KeyEvent evt, int key);

		virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

		virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

		virtual void resizeEvent(unsigned int width, unsigned int height);

		void		 controlEvent(Control* control, EventType evt);

	private:
		std::vector<Sample*>	m_arrSamples;
		int						m_curSampleIndex;

		Time					m_Timer;

		Form*					m_pSampleSelectForm; 

		LineRender*				m_pLineRender;
	};
}



#endif