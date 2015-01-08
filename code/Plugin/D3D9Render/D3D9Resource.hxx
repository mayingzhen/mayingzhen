#include "D3D9Resource.h"
#include "D3D9RenderDevice.h"
namespace ma
{

	D3D9Resource::D3D9Resource(void)
	{
		ASSERT(GetRenderDevice());
		if (GetRenderDevice())
		{
			((D3D9RenderDevice*)GetRenderDevice())->NotifyResourceCreated(this);
		}
	}

	D3D9Resource::~D3D9Resource(void)
	{
		ASSERT(GetRenderDevice());
		if (GetRenderDevice())
		{
			((D3D9RenderDevice*)GetRenderDevice())->NotifyResourceDestroyed(this);
		}
	}

}