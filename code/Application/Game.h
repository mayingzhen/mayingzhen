#pragma once

#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Engine/Module.h"


namespace ma
{	
	class Platform;
    
    struct APP_API Setting
    {
		RenderDeviceType eRenderType;
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
	
		virtual void	Reset(uint32_t nWidth,uint32_t nHeight);

		virtual void	Shutdown();

		virtual void	Run();

		virtual void	Update();

		virtual void	Render();

		const char*		GetGameName();
        
        const Setting&  GetSetting() {return m_setting;}

		static Game&	GetInstance();

	protected:
		std::string		m_sGameName;
        
        Setting         m_setting;
    

	public:
		bool m_bOnWindowSized;
		uint32_t m_OnWindowSizedWidth;
		uint32_t m_OnWindowSizedHeight;

		Signal< void(Keyboard::KeyEvent, Keyboard::Key) > mkeyEvent;
		Signal< void(Mouse::MouseEvent, int, int, int) > mMouseEvent;
	};
}



