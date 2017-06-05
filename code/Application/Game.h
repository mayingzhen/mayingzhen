#ifndef __GameBase_h__
#define __GameBase_h__

#include "Input/Keyboard.h"
#include "Input/Mouse.h"


namespace ma
{	
	class Platform;
    
    struct APP_API Setting
    {
		bool bForceGLES;
        bool bRenderThread;
        bool bDataThread;
        bool bJobScheduler;
    };

	class APP_API Game 
	{
	public:	
		Game(const char* pGameName,const Setting& set);

		virtual ~Game();
	
		virtual void	Init();
	
		virtual void	Reset(uint32 nWidth,uint32 nHeight);

		virtual void	Shutdown();

		virtual void	Run();

		virtual void	Update();

		virtual void	Render();

		const char*		GetGameName();

		static Game&	GetInstance();

	protected:
		std::string		m_sGameName;
        
        Setting         m_setting;
    

	public:
		bool m_bOnWindowSized;
		uint32 m_OnWindowSizedWidth;
		uint32 m_OnWindowSizedHeight;

		Signal< void(Keyboard::KeyEvent, Keyboard::Key) > mkeyEvent;
		Signal< void(Mouse::MouseEvent, int, int, int) > mMouseEvent;
	};
}


#endif
