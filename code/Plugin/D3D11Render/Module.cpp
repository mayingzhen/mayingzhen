#include "D3D11Render/stdafx.h"
#include "D3D11Render/Module.h"


#include "D3D11Render/D3D11Texture.hxx"
#include "D3D11Render/D3D11VertexBuffer.hxx"
#include "D3D11Render/D3D11IndexBuffer.hxx"
#include "D3D11Render/D3D11Mapping.hxx"
#include "D3D11Render/D3D11VertexDeclaration.hxx"
#include "D3D11Render/D3D11ShaderProgram.hxx"
#include "D3D11Render/D3D11RenderDevice.hxx"
#include "D3D11Render/D3D11Resource.hxx"
#include "D3D11Render/D3D11ConstantBuffer.hxx"

using namespace ma;


void D3D11RenderModuleInit()
{
	D3D11RenderDevice* pDxRenderDevice = new D3D11RenderDevice();
	SetRenderDevice(pDxRenderDevice);
}


void D3D11RenderModuleShutdown()
{
	D3D11RenderDevice* pDxRenderDevice = (D3D11RenderDevice*)GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
	SetRenderDevice(NULL);
}


extern "C" D3D11RENDER_API bool dllStartPlugin()
{
	D3D11RenderModuleInit();

	return true;
}


extern "C" D3D11RENDER_API bool dllStopPlugin()
{
	D3D11RenderModuleShutdown();

	return true;
}