#include "Game.h"

namespace ma
{
	static Game* __gameInstance = NULL;

	Game::Game(const char* pGameName)
	{
		__gameInstance = this;
		m_sGameName = pGameName ? pGameName : "";
	}

	Game::~Game()
	{

	}

	Game& Game::GetInstance()
	{
		ASSERT(__gameInstance);
		return *__gameInstance;
	}

	const char* Game::GetGameName()
	{
		return m_sGameName.c_str();
	}

	void Game::ModuleInit()
	{
		EngineModuleInit();
	}

	void Game::ModuleShutdown()
	{
		EngineModuleShutdown();
	}

	void Game::Init()
	{
		Input* pInput = new Input;
		SetInput(pInput);
		pInput->Init(Platform::GetInstance().GetWindId());
		int w,h;
		Platform::GetInstance().GetWindowSize(w,h);
		pInput->OnResize(w,h);

		if ( GetRenderSystem() )
			GetRenderSystem()->Init(Platform::GetInstance().GetWindId());

		if ( GetResourceSystem() )
			GetResourceSystem()->Init();
	}

	void Game::Shutdown()
	{
		if (GetResourceSystem())
			GetResourceSystem()->ShoutDown();

		if ( GetRenderSystem() )
			GetRenderSystem()->ShoutDown();

		Input* pInput = GetInput();
		pInput->Shutdown();
		SAFE_DELETE(pInput);
		SetInput(NULL);
	}

	void Game::Update()
	{
		profile_code();

		if (GetInput())
			GetInput()->Capture();

		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (GetResourceSystem())
			GetResourceSystem()->Update();

		if ( GetRenderSystem() )
			GetRenderSystem()->Update();
	}	

	void Game::Render()
	{
		if ( GetRenderSystem() )
			GetRenderSystem()->Render();	
	}
}