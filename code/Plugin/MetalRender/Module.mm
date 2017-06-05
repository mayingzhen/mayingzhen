#include "MetalRender/stdafx.h"
#include "MetalRender/Module.h"

#include "MetalRender/MetalRenderDevice.h"

/*
#include "MetalRender/MetalTexture.hxx"
#include "MetalRender/MetalVertexBuffer.hxx"
#include "MetalRender/MetalIndexBuffer.hxx"
#include "MetalRender/MetalMapping.hxx"
#include "MetalRender/MetalVertexDeclaration.hxx"
#include "MetalRender/MetalShaderProgram.hxx"
#include "MetalRender/MetalRenderDevice.hxx"
#include "MetalRender/MetalConstantBuffer.hxx"
#include "MetalRender/MetalRenderState.hxx"
#include "MetalRender/MetalSamplerState.hxx"
*/

using namespace ma;


void MetalRenderModuleInit()
{
	MetalRenderDevice* pDxRenderDevice = new MetalRenderDevice();
	SetRenderDevice(pDxRenderDevice);
}


void MetalRenderModuleShutdown()
{
	MetalRenderDevice* pDxRenderDevice = (MetalRenderDevice*)GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
	SetRenderDevice(NULL);
}


extern "C" METALRENDER_API bool dllStartPlugin()
{
	MetalRenderModuleInit();

	return true;
}


extern "C" METALRENDER_API bool dllStopPlugin()
{
	MetalRenderModuleShutdown();

	return true;
}
