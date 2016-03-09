#ifndef __GameBase_h__
#define __GameBase_h__

#include "Input/Keyboard.h"
#include "Input/Mouse.h"


namespace ma
{	
	class Platform;

	class APP_API Game 
	{
	public:	
		Game(const char* pGameName);

		virtual ~Game();
	
		virtual void	Init(bool bRenderThread, bool bDataThread, bool bJobScheduler);

		virtual void	Shutdown();

		virtual void	Update();

		virtual void	Render();

		const char*		GetGameName();

		static Game&	GetInstance();

	private:
		std::string		m_sGameName;

	public:
		bool m_bOnWindowSized;
		uint32 m_OnWindowSizedWidth;
		uint32 m_OnWindowSizedHeight;

		Signal< void(Keyboard::KeyEvent, Keyboard::Key) > mkeyEvent;
		Signal< void(Mouse::MouseEvent, int, int, int) > mMouseEvent;
	};
}


#endif
