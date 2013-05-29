#include "Game.h"

namespace ma
{
	static Game* __gameInstance = NULL;

	Game::Game()
	{
		__gameInstance = this;
	}

	Game::~Game()
	{

	}

	Game& Game::GetInstance()
	{
		ASSERT(__gameInstance);
		return *__gameInstance;
	}
}