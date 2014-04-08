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

		virtual void	Init() = 0;

		virtual void	Shutdown() = 0;

		virtual void	Update() = 0;

		virtual void	Render() = 0;

		const char*		GetGameName() {return m_sGameName.c_str();}

		static Game&	GetInstance();

	private:
		std::string		m_sGameName;
	};
}


#endif
