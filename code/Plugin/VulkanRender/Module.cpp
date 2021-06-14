#include "VulkanRender/stdafx.h"
#include "VulkanRender/Module.h"

#include "VulkanRender/VulkanTools.hxx"
#include "VulkanRender/VulkanDebug.hxx"

#include "VulkanRender/VulkanTexture.hxx"
#include "VulkanRender/VulkanVertexBuffer.hxx"
#include "VulkanRender/VulkanIndexBuffer.hxx"
#include "VulkanRender/VulkanMapping.hxx"
#include "VulkanRender/VulkanVertexDeclaration.hxx"
#include "VulkanRender/VulkanShaderProgram.hxx"
#include "VulkanRender/VulkanRenderDevice.hxx"
#include "VulkanRender/VulkanConstantBuffer.hxx"
#include "VulkanRender/VulkanRenderState.hxx"
#include "VulkanRender/VulkanSamplerState.hxx"
#include "VulkanRender/VulkanTechniqueh.hxx"
#include "VulkanRender/VulkanRenderPass.hxx"
#include "VulkanRender/VulkanRenderCommand.hxx"

#include "VulkanRender/util_init.hxx"

#if defined(__ANDROID__)
#include "VulkanRender/VulkanAndroid.hxx"
#endif

#include "glslang/SPIRV/GlslangToSpv.h"

using namespace ma;


void VulkanRenderModuleInit()
{
	glslang::InitializeProcess();

#if defined(__ANDROID__)
	// Vulkan library is loaded dynamically on Android
	bool libLoaded = vks::android::loadVulkanLibrary();
	assert(libLoaded);
#endif

	VulkanRenderDevice* pDxRenderDevice = new VulkanRenderDevice();
	SetRenderDevice(pDxRenderDevice);
}


void VulkanRenderModuleShutdown()
{
	VulkanRenderDevice* pDxRenderDevice = (VulkanRenderDevice*)GetRenderDevice();
	SAFE_DELETE(pDxRenderDevice);
	SetRenderDevice(NULL);

	glslang::FinalizeProcess();
}


extern "C" VULKAN_API bool dllStartPlugin()
{
	VulkanRenderModuleInit();

	return true;
}


extern "C" VULKAN_API bool dllStopPlugin()
{
	VulkanRenderModuleShutdown();

	return true;
}