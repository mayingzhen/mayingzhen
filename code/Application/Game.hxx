#include "Game.h"

namespace ma
{
	static Platform ___platform;

	static Game* __gameInstance = NULL;


	Game::Game(const char* pGameName,const Setting& set)
	{
         m_setting = set;
        
		__gameInstance = this;

		___platform.Init();
		m_sGameName = pGameName ? pGameName : "";

		Engine* pEngine = new Engine();
		SetEngine(pEngine);

		g_pInput = new Input();

		m_bOnWindowSized = false;
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

	void Game::Init()
	{
		void* hWnd = Platform::GetInstance().GetWindId();
		GetEngine()->Init(hWnd, m_setting.bRenderThread, m_setting.bDataThread, m_setting.bJobScheduler);
	}

	void Game::Reset(uint32_t nWidth,uint32_t nHeight)
	{
		GetEngine()->Reset(nWidth,nHeight);
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

		if (m_bOnWindowSized)
		{
			m_bOnWindowSized = false;
			this->Reset(m_OnWindowSizedWidth, m_OnWindowSizedHeight);
		}
	
		GetEngine()->Update();
	}	

	void Game::Render()
	{
		GetEngine()->Render();
	}
}
