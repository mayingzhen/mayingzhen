#include "D3D9Render/stdafx.h"
#include "D3D9Render/Module.h"


#include "D3D9Render/D3D9Texture.hxx"
#include "D3D9Render/D3D9VertexBuffer.hxx"
#include "D3D9Render/D3D9IndexBuffer.hxx"
#include "D3D9Render/D3D9Mapping.hxx"
#include "D3D9Render/D3D9VertexDeclaration.hxx"
#include "D3D9Render/D3D9ShaderProgram.hxx"
#include "D3D9Render/D3D9RenderDevice.hxx"
#include "D3D9Render/D3D9Resource.hxx"

using namespace ma;


void D3D9RenderModuleInit()
{
	D3D9RenderDevice* pDxRenderDevice = new D3D9RenderDevice();
	SetRenderDevice(pDxRenderDevice);
}


void D3D9RenderModuleShutdown()
{
	D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
	SetRenderDevice(NULL);
}


extern "C" D3D9RENDER_API bool dllStartPlugin()
{
	D3D9RenderModuleInit();

	return true;
}


extern "C" D3D9RENDER_API bool dllStopPlugin()
{
	D3D9RenderModuleShutdown();

	return true;
}