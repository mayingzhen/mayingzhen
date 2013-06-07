#include "GLESRender/stdafx.h"
#include "Module.h"


#ifndef GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_COMPONENT16_OES                                0x81A5
#endif

#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES        0
#endif

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG	0x8C00		
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG	0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG	0x8C02	
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG	0x8C03

#define GL_ATC_RGB_AMD						0x8C92
#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD		0x8C93
#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD	0x87EE

GLenum __gl_error_code = GL_NO_ERROR;

#include "GLESRenderDevice.hxx"
#include "GLESTexture.hxx"
#include "GLESVertexBuffer.hxx"
#include "GLESIndexBuffer.hxx"
#include "GLESMapping.hxx"
#include "GLESVertexDeclaration.hxx"
//#include "GLESTechnique.hxx"
#include "GLESEffect.hxx"




void GLESRenderModuleInit()
{
	ma::GLESRenderDevice* pGLESRenderDevice = new ma::GLESRenderDevice();
	ma::SetRenderDevice(pGLESRenderDevice);

	ma::InitTextureSystem();
}


void GLESRenderModuleShutdown()
{
	ma::ShundownTextureSystem();

	ma::GLESRenderDevice* pGLESRenderDevice = (ma::GLESRenderDevice*)ma::GetRenderDevice();
	SAFE_DELETE(pGLESRenderDevice);
}

