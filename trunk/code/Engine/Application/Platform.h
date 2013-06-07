#ifndef __Platform_h__
#define __Platform_h__


namespace ma
{
    
	class ENGINE_API Platform 
	{

	public:
		Platform(const char* appID = NULL);

		void		Init();

		void		Run();

		void		Shutdown();

		void		GetWindowSize(int& w, int& h) const;

		void		SetWindowSize(int w, int h);

		HWND		GetWindId() const {return m_windId;}

		static void		swapBuffers();

		static void		sleep(long ms);

		static float	GetAbsoluteTime();

		static Platform&	GetInstance();

	protected:
		HWND			m_windId;

		std::string		m_strAppID;
	};
}



#endif //__S3AUTApplication_h__