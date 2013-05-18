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

	protected:
		HWND			m_windId;

		std::string		m_strAppID;
	};
}



#endif //__S3AUTApplication_h__