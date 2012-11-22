#include "DXRender/stdafx.h"
#include "DXRender/Module.h"

#include "DXRender/DxRenderDevice.hxx"
#include "DXRender/DxRender.hxx"
#include "DXRender/DxRendMesh.hxx"
#include "DXRender/DxRendTexture.hxx"

void DxRenderModuleInit()
{
	ma::DxRender* pDxRender = new ma::DxRender();
	ma::SetRender(pDxRender);

}

void DxRenderModuleShutdown()
{
	ma::DxRender* pDxRender = (ma::DxRender*)ma::GetRender();
	SAFE_DELETE(pDxRender);
}
