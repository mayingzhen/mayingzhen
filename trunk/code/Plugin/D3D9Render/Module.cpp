#include "D3D9Render/stdafx.h"
#include "D3D9Render/Module.h"

#include "D3D9Render/D3D9RenderDevice.hxx"
#include "D3D9Render/LineRender.hxx"
#include "D3D9Render/ScreenQuad.hxx"
#include "D3D9Render/D3D9Texture.hxx"
#include "D3D9Render/UnitSphere.hxx"
#include "D3D9Render/D3D9VertexBuffer.hxx"
#include "D3D9Render/D3D9IndexBuffer.hxx"
#include "D3D9Render/D3D9Mapping.hxx"
#include "D3D9Render/D3D9VertexDeclaration.hxx"
#include "D3D9Render/D3D9ShaderProgram.hxx"
#include "D3D9Render/D3D9RenderTarget.hxx"



void D3D9RenderModuleInit()
{
	ma::D3D9RenderDevice* pDxRenderDevice = new ma::D3D9RenderDevice();
	ma::SetRenderDevice(pDxRenderDevice);
}


void D3D9RenderModuleShutdown()
{
	ma::D3D9RenderDevice* pDxRenderDevice = (ma::D3D9RenderDevice*)ma::GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
}

