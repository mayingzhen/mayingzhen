#ifndef __Platform_h__
#define __Platform_h__


#include <string>

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

		void*		GetWindId() const {return m_windId;}

		void		SetWindId(void* hWnd) {m_windId = hWnd;} 

		const char*	GetAppPath() const;

		void		SetAppPath(const char* pAppPath);

		static Platform&	GetInstance();

	protected:
		void*			m_windId;

		std::string		m_strAppPath;

		bool			m_bInit = false;
	};
}



#endif //__Platform_h__
