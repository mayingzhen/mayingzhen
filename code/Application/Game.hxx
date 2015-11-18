#include "Game.h"

namespace ma
{
	static Game* __gameInstance = NULL;

	Game::Game(const char* pGameName)
	{
		__gameInstance = this;
		m_sGameName = pGameName ? pGameName : "";

		g_pEngine = new Engine();
	}

	Game::~Game()
	{
		SAFE_DELETE(g_pEngine);
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

	void Game::Init()
	{
		HWND hWnd = Platform::GetInstance().GetWindId();
		GetEngine()->Init(hWnd, false, false, false);
	}

	void Game::Shutdown()
	{
		GetEngine()->Shutdown();
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