#include "Common/IRender/IRender.h"

namespace ma
{
	IRender* g_pRender = NULL;

	void SetRender(IRender* pRender)
	{
		g_pRender = pRender;
	}

	IRender* GetRender()
	{
		return g_pRender;
	}

}