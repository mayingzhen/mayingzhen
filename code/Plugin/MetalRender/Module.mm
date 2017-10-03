#include "MetalRender/stdafx.h"
#include "MetalRender/Module.h"

#include "MetalRender/MetalRenderDevice.h"

#include "glslang/SPIRV/GlslangToSpv.h"


using namespace ma;


void MetalRenderModuleInit()
{
    glslang::InitializeProcess();
    
	MetalRenderDevice* pMetalRenderDevice = new MetalRenderDevice();
	SetRenderDevice(pMetalRenderDevice);
}


void MetalRenderModuleShutdown()
{
	MetalRenderDevice* pMetalRenderDevice = (MetalRenderDevice*)GetRenderDevice();
	SAFE_DELETE(pMetalRenderDevice);
	SetRenderDevice(NULL);
    
    glslang::FinalizeProcess();
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
