#include "D3D9Render/stdafx.h"
#include "D3D9Render/Module.h"

#include "D3D9Render/D3D9RenderDevice.hxx"
#include "D3D9Render/D3D9Render.hxx"
#include "D3D9Render/D3D9RendMesh.hxx"
#include "D3D9Render/D3D9RendTexture.hxx"
#include "D3D9Render/DxMeshHelp.hxx"


void D3D9RenderModuleInit()
{
	ma::D3D9RenderDevice* pDxRenderDevice = new ma::D3D9RenderDevice();
	ma::SetRenderDevice(pDxRenderDevice);

 	ma::D3D9Render* pDxRender = new ma::D3D9Render();
 	ma::SetRender(pDxRender);
	//pDxRender->InitDefaultShader();
}


void D3D9RenderModuleShutdown()
{
 	ma::D3D9Render* pDxRender = (ma::D3D9Render*)ma::GetRender();
 	SAFE_DELETE(pDxRender); 

	ma::D3D9RenderDevice* pDxRenderDevice = (ma::D3D9RenderDevice*)ma::GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
}

