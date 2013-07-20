#ifndef __Platform_h__
#define __Platform_h__


namespace ma
{
    
	class ENGINE_API Platform 
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

		const char*	GetAppPath() {return m_strAppPath.c_str();}

		void		SetAppPath(const char* pAppPath) {m_strAppPath = pAppPath;}

		static void		swapBuffers();

		static void		sleep(long ms);

		static Platform&	GetInstance();

	protected:
		HWND			m_windId;

		std::string		m_strAppPath;
	};
}



#endif //__S3AUTApplication_h__