#include "Game.h"

namespace ma
{
	static Platform ___platform;

	static Game* __gameInstance = NULL;


	Game::Game(const char* pGameName)
	{
		__gameInstance = this;

		___platform.Init();
		m_sGameName = pGameName ? pGameName : "";

		Engine* pEngine = new Engine();
		SetEngine(pEngine);

		g_pInput = new Input();
	}

	Game::~Game()
	{
		Engine* pEngine = GetEngine();
		SAFE_DELETE(pEngine);
		SetEngine(NULL);

		SAFE_DELETE(g_pInput);

		___platform.Shutdown();
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

	void Game::Init(bool bRenderThread, bool bDataThread, bool bJobScheduler)
	{
		HWND hWnd = Platform::GetInstance().GetWindId();
		GetEngine()->Init(hWnd, bRenderThread, bDataThread, bJobScheduler);
	}

	void Game::Shutdown()
	{
		GetEngine()->Shutdown();
	}

	void Game::Run()
	{
		___platform.Run();
	}

	void Game::Update()
	{
		profile_code();
	
		GetEngine()->Update();
	}	

	void Game::Render()
	{
		GetEngine()->Render();
	}
}