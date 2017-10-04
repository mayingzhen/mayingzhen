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
#include "VulkanRender/VulkanPipeline.hxx"
#include "VulkanRender/VulkanRenderPass.hxx"
#include "VulkanRender/VulkanRenderCommand.hxx"

#include "glslang/SPIRV/GlslangToSpv.h"

using namespace ma;


void VulkanRenderModuleInit()
{
	glslang::InitializeProcess();

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