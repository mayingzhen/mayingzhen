#include "DXRender/stdafx.h"
#include "DXRender/Module.h"

#include "DXRender/DxRenderDevice.hxx"
#include "DXRender/DxRender.hxx"
#include "DXRender/DxRendMesh.hxx"
#include "DXRender/DxRendTexture.hxx"

#include "MWD3D9IndexBuffer.hxx"
#include "MWD3D9VertexBuffer.hxx"
#include "MWD3D9Texture.hxx"
#include "MWD3D9Mapping.hxx"
#include "MWD3D9VertexDeclaration.hxx"

void DxRenderModuleInit()
{
	ma::DxRenderDevice* pDxRenderDevice = new ma::DxRenderDevice();
	ma::SetRenderDevice(pDxRenderDevice);

 	ma::DxRender* pDxRender = new ma::DxRender();
 	ma::SetRender(pDxRender);
	//pDxRender->InitDefaultShader();
}


void DxRenderModuleShutdown()
{
 	ma::DxRender* pDxRender = (ma::DxRender*)ma::GetRender();
 	SAFE_DELETE(pDxRender); 

	ma::DxRenderDevice* pDxRenderDevice = (ma::DxRenderDevice*)ma::GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
}
