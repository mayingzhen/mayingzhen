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

// 	void Game::Frame()
// 	{
// // 		GetAnimationSystem()->Update();
// // 
// // 		GetPhysicsSystem()->Update();
// // 
// // 		GetAiDevice()->Update();
// 	}
}