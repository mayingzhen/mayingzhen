#ifndef __ApplicationWin_h__
#define __ApplicationWin_h__


namespace ma
{
	class Application_API ApplicationWin : public Application
	{
	public:
		ApplicationWin(const char* appID);

		virtual int			CreateMainWnd(int argc, char* argv[]);

		virtual void		Run();	

		virtual	void		GetWindowSize(int& w, int& h);

		HWND				GetMainHWND() {return m_hMainWnd;}

	private:
		HWND	m_hMainWnd;
	};
}

#endif
