#ifndef __GameBase_h__
#define __GameBase_h__



namespace ma
{	
	class Platform;

	class APP_API Game 
	{
	public:	
		Game(const char* pGameName);

		virtual ~Game();
	
		virtual void	Init();

		virtual void	Shutdown();

		virtual void	Update();

		virtual void	Render();

		const char*		GetGameName();

		static Game&	GetInstance();

	private:
		std::string		m_sGameName;
	};
}


#endif
