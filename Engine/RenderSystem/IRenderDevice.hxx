#include "IRenderDevice.h"

namespace ma
{
	IRenderDevice* g_pRenderDevice = NULL;

	void SetRenderDevice(IRenderDevice* pRenderDevice)
	{
		g_pRenderDevice = pRenderDevice;
	}

	IRenderDevice* GetRenderDevice()
	{
		return g_pRenderDevice;
	}

}