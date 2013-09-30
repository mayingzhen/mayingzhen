#include "ITerrain.h"

namespace ma
{
	ITerrain* gpTerrain = NULL;

	ITerrain* GetTerrain()
	{
		return gpTerrain;
	}

	void SetTerrain(ITerrain* pTerrain)
	{
		gpTerrain = pTerrain;
	}
}

