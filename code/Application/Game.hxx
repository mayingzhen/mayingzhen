#include "Game.h"
#include "ui.h"

namespace ma
{
	static Platform __platform;

	static Game* __gameInstance = NULL;


	Game::Game(const char* pGameName,const Setting& set)
	{
         m_setting = set;
        
		__gameInstance = this;

		__platform.Init();
		m_sGameName = pGameName ? pGameName : "";

		Engine* pEngine = new Engine();
		SetEngine(pEngine);

		g_pInput = new Input();
		g_pUI = new UI();

		m_bOnWindowSized = false;
	}

	Game::~Game()
	{
		Engine* pEngine = GetEngine();
		SAFE_DELETE(pEngine);
		SetEngine(NULL);

		SAFE_DELETE(g_pInput);

		__platform.Shutdown();
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
		SYSTRACE_INIT();

		void* hWnd = Platform::GetInstance().GetWindId();
		int windth = 800;
		int height = 600;
		Platform::GetInstance().SetWindowSize(windth, height);
		GetEngine()->Init(hWnd, windth, height, m_setting.bRenderThread, m_setting.bDataThread, m_setting.bJobScheduler);

		g_pUI->Init();
	}

	void Game::Reset(uint32_t nWidth,uint32_t nHeight)
	{
		GetEngine()->Reset(nWidth,nHeight);
	}

	void Game::Shutdown()
	{
		g_pInput->Shutdown();

		GetEngine()->Shutdown();
	}

	void Game::Run()
	{
		__platform.Run();
	}

	void Game::Update()
	{
		//MicroProfileFlip(nullptr);

		if (m_bOnWindowSized)
		{
			m_bOnWindowSized = false;
			this->Reset(m_OnWindowSizedWidth, m_OnWindowSizedHeight);
		}

		g_pUI->Update();
	
		GetEngine()->Update();

	}	

	void Game::Render()
	{
		g_pUI->Render();

		GetEngine()->Render();
	}
}

