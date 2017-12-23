#ifndef __Platform_h__
#define __Platform_h__


namespace ma
{
    
	class APP_API Platform 
	{

	public:
		Platform();

		void		Init();

		void		Run();

		void		Shutdown();

		void		GetWindowSize(int& w, int& h) const;

		void		SetWindowSize(int w, int h);

		HWND		GetWindId() const {return m_windId;}

		void		SetWindId(HWND hWnd) {m_windId = hWnd;} 

		const char*	GetAppPath() const;

		void		SetAppPath(const char* pAppPath);

		static Platform&	GetInstance();

	protected:
		HWND			m_windId;

		std::string		m_strAppPath;

		bool			m_bInit;
	};
}



#endif //__Platform_h__
