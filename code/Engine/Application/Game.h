#ifndef __GameBase_h__
#define __GameBase_h__



namespace ma
{	
	class Platform;

	class ENGINE_API Game 
	{
	public:	
		Game(const char* pGameName);

		virtual ~Game();
			
		virtual void	Init() = 0;

		virtual void	Shutdown() = 0;

		virtual void	Update() = 0;

		virtual void	Render() = 0;

		//virtual	void	Frame();

		const char*		GetGameName() {return m_sGameName.c_str();}

		///// Input
// 		virtual void	keyEvent(Keyboard::KeyEvent evt, int key) = 0;
// 
// 		virtual void	touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) = 0;
// 
// 		virtual bool	mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta) = 0;
//     
// 		virtual void	resizeEvent(unsigned int width, unsigned int height) = 0;

		static Game&	GetInstance();

	private:
		std::string		m_sGameName;
	};
}


#endif
