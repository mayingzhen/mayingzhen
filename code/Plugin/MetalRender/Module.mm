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
	MetalRenderDevice* pMetalRenderDevice = new MetalRenderDevice();
	SetRenderDevice(pMetalRenderDevice);
}


void MetalRenderModuleShutdown()
{
	MetalRenderDevice* pMetalRenderDevice = (MetalRenderDevice*)GetRenderDevice();
	SAFE_DELETE(pMetalRenderDevice);
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
