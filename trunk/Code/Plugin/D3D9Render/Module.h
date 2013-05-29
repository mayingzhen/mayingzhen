#ifndef  _D3D9RENDER_MODULE__H__
#define  _D3D9RENDER_MODULE__H__

#include <d3dx9.h>
#include <dxerr.h>

#define  _D3D_V(str) str

#include "D3D9Render/Export.h"
#include "D3D9Render/D3D9RenderDevice.h"
#include "D3D9Render/LineRender.h"
#include "D3D9Render/D3D9Texture.h"
#include "D3D9Render/LineRender.h"
#include "D3D9Render/ScreenQuad.h"
#include "D3D9Render/UnitSphere.h"
//#include "D3D9Render/ShadowMap.h"
#include "D3D9Render/D3D9VertexBuffer.h"
#include "D3D9Render/D3D9IndexBuffer.h"
#include "D3D9Render/D3D9Mapping.h"
#include "D3D9Render/D3D9VertexDeclaration.h"
#include "D3D9Render/D3D9Technique.h"


D3D9RENDER_API void D3D9RenderModuleInit();

D3D9RENDER_API void D3D9RenderModuleShutdown();

#endif
