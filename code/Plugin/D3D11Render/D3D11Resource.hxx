#include "D3D11Resource.h"
#include "D3D11RenderDevice.h"

namespace ma
{

	D3D11Resource::D3D11Resource(void)
	{
		ASSERT(GetRenderDevice());
		if (GetRenderDevice())
		{
			((D3D11RenderDevice*)GetRenderDevice())->NotifyResourceCreated(this);
		}
	}

	D3D11Resource::~D3D11Resource(void)
	{
		ASSERT(GetRenderDevice());
		if (GetRenderDevice())
		{
			((D3D11RenderDevice*)GetRenderDevice())->NotifyResourceDestroyed(this);
		}
	}

}