#ifndef  _DXRENDER_MODULE__H__
#define  _DXRENDER_MODULE__H__

#include "DXRender/DxRenderExport.h"
#include "DXRender/DxRenderDevice.h"
#include "DXRender/DxRender.h"
#include "DXRender/DxRendMesh.h"
#include "DxRender/DxRendTexture.h"

#include "MWD3D9IndexBuffer.h"
#include "MWD3D9VertexBuffer.h"
#include "MWD3D9Texture.h"
#include "MWD3D9Mapping.h"
#include "MWD3D9VertexDeclaration.h"


DXRENDER_API void DxRenderModuleInit();

DXRENDER_API void DxRenderModuleShutdown();

#endif