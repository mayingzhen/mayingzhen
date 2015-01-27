#include "Game.h"

namespace ma
{
	static Game* __gameInstance = NULL;

	Game::Game(const char* pGameName)
	{
		__gameInstance = this;
		m_sGameName = pGameName ? pGameName : "";

		Engine* pEngine = new Engine();
		SetEngine(pEngine);
	}

	Game::~Game()
	{
		Engine* pEngine = GetEngine();
		SAFE_DELETE(pEngine);
		SetEngine(NULL);
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
		Input* pInput = new Input;
		SetInput(pInput);
		pInput->Init(hWnd);
		int w,h;
		Platform::GetInstance().GetWindowSize(w,h);
		pInput->OnResize(w,h);

		GetEngine()->Init(hWnd, true, false, false);
	}

	void Game::Shutdown()
	{
		GetEngine()->Shutdown();

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
	
		GetEngine()->Update();
	}	

	void Game::Render()
	{
		GetEngine()->Render();
	}
}