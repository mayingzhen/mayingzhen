#include "GLESRenderDevice.h"
#include "GLESTexture.h"
#include "GLESBase.h"
#include "GLESRenderState.h"
#include "GlESSamplerState.h"

#include "DeviceContext/GLESDeviceContext.h"


namespace ma
{

	GLESRenderDevice::GLESRenderDevice()
	{
		m_pDeviceContext = new GLESDeviceContext();

		mStencilEnabledGL = false;
		mStencilMaskGL = 0xFFFFffff;

		m_pVertexDecl = NULL;
	}

	GLESRenderDevice::~GLESRenderDevice()
	{
		SAFE_DELETE(m_pDeviceContext);
	}


	Texture* GLESRenderDevice::CreateTexture()
	{
		return new GLESTexture();
	}

	Texture* GLESRenderDevice::CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		return new GLESTexture(nWidth,nHeight,nMipMap,format,false,bSRGB,USAGE_RENDERTARGET,eType);
	}

	Texture* GLESRenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		return new GLESTexture(nWidth,nHeight,1,format,bTypeLess,false,USAGE_DEPTHSTENCIL,TEXTYPE_2D);
	}

	VertexDeclaration* GLESRenderDevice::CreateVertexDeclaration()
	{
		return new GLESVertexDeclaration();
	}

	VertexBuffer*	GLESRenderDevice::CreateVertexBuffer()
	{
		return new GLESVertexBuffer();
	}

	IndexBuffer*	GLESRenderDevice::CreateIndexBuffer()
	{
		return new GLESIndexBuffer();
	}

	ShaderProgram*	GLESRenderDevice::CreateShaderProgram()
	{
		return new GLESShaderProgram();
	}

	BlendState*	GLESRenderDevice::CreateBlendState()
	{
		return new GLESBlendStateObject();
	}

	DepthStencilState* GLESRenderDevice::CreateDepthStencilState()
	{
		return new GLESDepthStencilStateObject();
	}

	RasterizerState* GLESRenderDevice::CreateRasterizerState()
	{
		return new GLESRasterizerStateObject();
	}

	SamplerState* GLESRenderDevice::CreateSamplerState()
	{
		return new GLESSamplerState();
	}

	Technique* GLESRenderDevice::CreateTechnique()
	{
		return new Technique();
	}

	void GLESRenderDevice::Shoutdown()
	{

	}

	void GLESRenderDevice::Init(HWND wndhandle)
	{
		m_pDeviceContext->Init(wndhandle);

 		GL_ASSERT( glGenFramebuffers(1, &m_hOffecreenFrameBuffer) );

#ifdef GL_MAX_COLOR_ATTACHMENTS
		GLint val;
		GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
#endif
		
		{
			mStencilEnabledGL = false;
			glDisable(GL_STENCIL_TEST);
			//GL_CHECK_ERROR;
		}

		{
			mStencilMaskGL = 0xFFFFffff;
			glStencilMask(mStencilMaskGL);
			//GL_CHECK_ERROR;
		}

		BuildDeviceCapabilities();
	}

	void GLESRenderDevice::BuildDeviceCapabilities()
	{
		// driver version
		const GLubyte* pcVer = glGetString(GL_VERSION);
		string tmpStr = (const char*)pcVer;
		string mVersion = tmpStr.substr(0, tmpStr.find(" "));
		vector<string> tokens = StringUtil::split(mVersion, ".");
		DriverVersion mDriverVersion;
		if (!tokens.empty())
		{
			mDriverVersion.major = StringConverter::parseInt(tokens[0]);
			if (tokens.size() > 1)
				mDriverVersion.minor = StringConverter::parseInt(tokens[1]);
			if (tokens.size() > 2)
				mDriverVersion.release = StringConverter::parseInt(tokens[2]);
		}
		mDriverVersion.build = 0;
		GetDeviceCapabilities()->SetDriverVersion(mDriverVersion);

		// determine vendor
		const char* vendorName = (const char*)glGetString(GL_VENDOR);  
		// Determine vendor
		if (strstr(vendorName, "Imagination Technologies"))
			GetDeviceCapabilities()->SetVendor(GPU_IMAGINATION_TECHNOLOGIES);
		else if (strstr(vendorName, "Apple Computer, Inc."))
			GetDeviceCapabilities()->SetVendor(GPU_APPLE);  // iPhone Simulator
		else if (strstr(vendorName, "NVIDIA"))
			GetDeviceCapabilities()->SetVendor(GPU_NVIDIA);
		else
			GetDeviceCapabilities()->SetVendor(GPU_UNKNOWN);

		const char* deviceName = (const char*)glGetString(GL_RENDERER);      
		if (deviceName)
		{
			GetDeviceCapabilities()->SetDeviceName(deviceName);
		}
		GetDeviceCapabilities()->SetRenderSystemName("GLES2RenderSystem");

		GetDeviceCapabilities()->SetNumMultiRenderTargets(1);

		// Point size
		GLfloat psRange[2] = {0.0, 0.0};
		GL_ASSERT( glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, psRange) );
		GetDeviceCapabilities()->SetMaxPointSize(psRange[1]);

		const GLubyte* pcExt = glGetString(GL_EXTENSIONS);
		LogInfo((const char*)pcExt);

		string strExt = (char*)pcExt;

		// Check for Float textures
#if GL_OES_texture_float || GL_OES_texture_half_float
		if (strExt.find("GL_OES_texture_float") != string::npos || strExt.find("GL_OES_texture_half_float") != string::npos)
		{
			GetDeviceCapabilities()->SetFloatTexturesSupported(true);
		}
#endif

#if GL_OES_vertex_array_object
		if(strExt.find("GL_OES_vertex_array_object") != string::npos || strExt.find("GL_ARB_vertex_array_object"))
		{
			g_bGL_OES_vertex_array_object = true;
#if defined(WIN32) || defined(__ANDROID__)
			glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC) eglGetProcAddress("glBindVertexArrayOES");
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC) eglGetProcAddress("glDeleteVertexArraysOES");
			glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC) eglGetProcAddress("glGenVertexArraysOES");
			glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC) eglGetProcAddress("glIsVertexArrayOES");
			//g_bGL_OES_vertex_array_object = (glBindVertexArrayOES != NULL);
#endif
		}
		else
#endif
			g_bGL_OES_vertex_array_object = false;

// #if GL_OES_mapbuffer
// 		if(strExt.find("GL_OES_mapbuffer") != string::npos)
// 		{
// 			g_bGL_OES_mapbuffer = true;
// #if defined(WIN32) || defined(__ANDROID__)
// 			//glMapBuffer = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
			//glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
			//glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVOESPROC)eglGetProcAddress("glGetBufferPointervOES");
// 			g_bGL_OES_mapbuffer = (glMapBufferOES != NULL);
// #endif
// 		}
// 		else
// #endif
// 			g_bGL_OES_mapbuffer = false;


#if GL_APPLE_framebuffer_multisample
		if(strExt.find("GL_APPLE_framebuffer_multisample") != string::npos)
		{
			g_bGL_APPLE_framebuffer_multisample = true;
#if defined(WIN32) || defined(__ANDROID__)
			glRenderbufferStorageMultisampleAPPLE = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEAPPLEPROC)eglGetProcAddress("glRenderbufferStorageMultisampleAPPLE");
			glResolveMultisampleFramebufferAPPLE = (PFNGLRESOLVEMULTISAMPLEFRAMEBUFFERAPPLEPROC)eglGetProcAddress("glResolveMultisampleFramebufferAPPLE");
			g_bGL_APPLE_framebuffer_multisample = (glRenderbufferStorageMultisampleAPPLE != NULL);
#endif
		}
		else
#endif
			g_bGL_APPLE_framebuffer_multisample = false;

#if GL_IMG_texture_compression_pvrtc
		if (strExt.find("GL_IMG_texture_compression_pvrtc") != string::npos)
		{
			GetDeviceCapabilities()->SetTexturePVRTCSupported(true);
		}
		else
#endif
			GetDeviceCapabilities()->SetTexturePVRTCSupported(false);

#if GL_EXT_texture_compression_dxt1 == 1 && GL_EXT_texture_compression_s3tc == 1
		if (strExt.find("GL_EXT_texture_compression_dxt1") != string::npos && strExt.find("GL_EXT_texture_compression_s3tc") != string::npos)
			GetDeviceCapabilities()->SetTextureDXTSupported(true);
		else
#endif
			GetDeviceCapabilities()->SetTextureDXTSupported(false);

#if GL_OES_compressed_ETC1_RGB8_texture
		if (strExt.find("GL_OES_compressed_ETC1_RGB8_texture") != string::npos)
			GetDeviceCapabilities()->SetTextureETC1Supported(true);
		else
#endif
			GetDeviceCapabilities()->SetTextureETC1Supported(false);

#if GL_EXT_texture_filter_anisotropic
		if (strExt.find("GL_EXT_texture_filter_anisotropic") != string::npos)
		{
			GetDeviceCapabilities()->SetTextureAnisotropy(true);
		}
		else
#endif
			GetDeviceCapabilities()->SetTextureAnisotropy(false);


#if GL_OES_packed_depth_stencil
		if (strExt.find("GL_OES_packed_depth_stencil") != string::npos)
		{
			g_bGL_OES_packed_depth_stencil = true;
		}
		else
#endif
			g_bGL_OES_packed_depth_stencil = false;

		GetDeviceCapabilities()->log();
	}

	void GLESRenderDevice::BeginRender()
	{
	}

	void GLESRenderDevice::EndRender()
	{
		GL_ASSERT( glFlush() );

		m_pDeviceContext->SwapBuffers();
	}

	void GLESRenderDevice::SetFrameBuffer(FrameBuffer* pFB)
	{

	}

	void GLESRenderDevice::SetRenderTarget(int index,Texture* pTexture,int level, int array_index, int face)
	{
		ASSERT(pTexture && index == 0);
		if (pTexture == NULL || index != 0)
			return ;

		GLESTexture* pGLESTarget = (GLESTexture*)pTexture;

		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, pGLESTarget->GetFrameBuffer()) );

		if (pGLESTarget->GetTexture() > 0)
		{
			GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
			GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pGLESTarget->GetTexture(), 0) );
			GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			{
				LogError("Framebuffer status incomplete: 0x%x", fboStatus);
			}
		}
	}

	Texture* GLESRenderDevice::GetDefaultRenderTarget(int index)
	{
		if (index > 0)
			 return NULL;

		GLESTexture* pGLESTarget = new GLESTexture();

		GLint hFboBinding;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &hFboBinding) );
		
		//GLint hTexture;
		//GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
		//GL_ASSERT( glGetFramebufferAttachmentParameteriv(hFboBinding,attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&hTexture) );

		pGLESTarget->SetFrameBuffer(hFboBinding);
		//pGLESTarget->SetTexture(hTexture);

		return pGLESTarget;
	}


	void GLESRenderDevice::SetDepthStencil(Texture* pTexture)
	{
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		GLESTexture* pGLESTarget = (GLESTexture*)pTexture;

		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, pGLESTarget->GetFrameBuffer()) );

		if (pGLESTarget->GetTexture() > 0)
		{
			GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pGLESTarget->GetTexture()) );
			GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			{
				LogError("Framebuffer status incomplete: 0x%x", fboStatus);
			}
		}
	}

	Texture* GLESRenderDevice::GetDefaultDepthStencil()
	{
		GLESTexture* pGLESTarget = new GLESTexture();

		GLint hFboBinding;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &hFboBinding) );
		
		//GLint hTexture;
		//GL_ASSERT( glGetFramebufferAttachmentParameteriv(hFboBinding,GL_DEPTH_ATTACHMENT,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&hTexture) );

		pGLESTarget->SetFrameBuffer(hFboBinding);
		//pGLESTarget->SetTexture(hTexture);

		return pGLESTarget;
	}

	void GLESRenderDevice::SetViewport(const Rectangle& rect)
	{
		glViewport((GLuint)rect.left, (GLuint)rect.top, (GLuint)rect.width(), (GLuint)rect.height());
	}

	Rectangle GLESRenderDevice::GetViewport()
	{
		Rectangle rect;

		GLint viewport[4]; 
 		GL_ASSERT( glGetIntegerv(GL_VIEWPORT, viewport) );
		
		rect.left = (float)viewport[0];
		rect.top = (float)viewport[1];
		rect.right = rect.left + (float)viewport[2];
		rect.bottom = rect.top + (float)viewport[3];

		return rect;
	}

	void GLESRenderDevice::SetBlendState(const BlendState* pBlendState)
	{
		switch (pBlendState->m_eBlendMode)
		{
		case BM_OPATICY:
			GL_ASSERT( glDisable(GL_BLEND) );
			break;

		case BM_TRANSPARENT:
			GL_ASSERT( glEnable(GL_BLEND) );
			GL_ASSERT( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
			break;

		case BM_ADD:
			GL_ASSERT( glEnable(GL_BLEND) );
			GL_ASSERT( glBlendFunc(GL_SRC_ALPHA, GL_ONE) );
			break;

		case BM_MULTIPLY:
			break;
		}

		bool b = pBlendState->m_bColorWrite;
		GL_ASSERT(glColorMask(/*r*/b, b/*g*/, b/*b*/, b/*a*/));
	}

	void GLESRenderDevice::SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
	{
		switch (pDSState->m_eDepthCheckMode)
		{
		case CMPF_ALWAYS_FAIL:
			GL_ASSERT( glDisable(GL_DEPTH_TEST) );
			break;

		case CMPF_LESS_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_LEQUAL) );
			break;

		case CMPF_LESS:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_LESS) );
			break;

		case CMPF_GREATER_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_GEQUAL) );
			break;

		case CMPF_GREATER:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_GREATER) );
			break;

		case CMPF_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_EQUAL) );
			break;
		default:
			ASSERT(false);
			break;
		}
	}

	void GLESRenderDevice::SetRasterizerState(const RasterizerState* pRSState)
	{

	}

	void GLESRenderDevice::SetStencilMaskGL(GLuint mask)
	{
		if (mStencilMaskGL == mask)
		{
			return;
		}
		mStencilMaskGL = mask;

		GL_ASSERT( glStencilMask(mask) );
	}

	void GLESRenderDevice::SetTexture(uint32 nIndex,Texture* sampler,bool bSRGBNotEqual)
	{
		GL_ASSERT( glActiveTexture(GL_TEXTURE0 + nIndex) );

		GLESTexture* pTexture = (GLESTexture*)sampler;
	
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, pTexture ? pTexture->GetTexture() : NULL) );
	}

	void GLESRenderDevice::SetTexture(Uniform* uniform,Texture* sampler)
	{
		ASSERT(uniform && sampler);
		ASSERT(uniform->m_type == GL_SAMPLER_2D);

		SetTexture(uniform->m_index,sampler,false);

		GL_ASSERT( glUniform1i(uniform->m_location, uniform->m_index) );
	}

	void GLESRenderDevice::SetSamplerState(Uniform* uniform,SamplerState* pSampler)
	{
		ASSERT(uniform);
		ASSERT(uniform->m_type == GL_SAMPLER_2D);
		ASSERT(pSampler);

		GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->m_index) );

		GLESTexture* pTexture = (GLESTexture*)pSampler->GetTexture();
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, pTexture->GetTexture() ) );

		GL_ASSERT( glUniform1i(uniform->m_location, uniform->m_index) );
		
		//if (pSampler->GetWrapMode() != pTexture->GetTextureWrap() )
		{
			GLenum wrapS = GLESMapping::GetGLESWrap(pSampler->GetWrapMode());
			GLenum wrapT = GLESMapping::GetGLESWrap(pSampler->GetWrapMode());

			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS) );
			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT) );
		}
 
		//if (pSampler->GetFilterMode() != pTexture->GetTextureFilter())
		{
			GLenum minFilter,magFilter;
			GLESMapping::GetGLESFilter(pSampler->GetFilterMode(),minFilter,magFilter);

			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter) );
			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter) );	
		}	
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, int value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform1i(uniform->m_location, value) );
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, float value)
	{
 		ASSERT(uniform);
 		GL_ASSERT( glUniform1f(uniform->m_location, value) );
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniformMatrix4fv(uniform->m_location, count, GL_FALSE, (GLfloat*)values) );
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, const Vector2& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform2f(uniform->m_location, value.x, value.y) );
	}


	void GLESRenderDevice::SetValue(Uniform* uniform, const Vector3& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform3f(uniform->m_location, value.x, value.y, value.z) );
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform4fv(uniform->m_location, count, (GLfloat*)values) );
	}

	void GLESRenderDevice::SetValue(Uniform* uniform, const ColourValue& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform4f(uniform->m_location, value.r, value.g, value.b, value.a) );
	}

	void GLESRenderDevice::SetVertexDeclaration(const VertexDeclaration* pDec)
	{
		m_pVertexDecl = (GLESVertexDeclaration*)pDec;
	}

	void GLESRenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pIB;
		GL_ASSERT( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() ) );
	}

	void GLESRenderDevice::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pVB;
		GL_ASSERT( glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() ) );
	}



	#define BUFFER_OFFSET(offset) ((int8 *) NULL + offset)

	void GLESRenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		profile_code();

		ASSERT(pTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pTech->GetShaderProgram();
        GLESVertexDeclaration* pVertexDecl = (GLESVertexDeclaration*)pProgram->GetVertexDeclaration();

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData.get();

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		//UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffer->GetNumber();
		UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;

		int vertexStartByte = nVertexStart * pVertexDecl->GetStreanmStride();

		int nSteam = pVertexDecl->GetElementCount();
		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = pVertexDecl->GetElement(i);

			GLint typeCount = 1; 
			GLenum type = 0;
			GLboolean normalized = false;
			std::string name;
			GLESMapping::GetGLESDeclType(ve.Usage,ve.UsageIndex,ve.Type,type,typeCount,normalized,name);
			VertexAttribute attr = pProgram->GetVertexAttribute(name.c_str());
			if (attr == -1)
				continue;

			void* pVBufferData = BUFFER_OFFSET( vertexStartByte + ve.Offset );		

			GL_ASSERT( glVertexAttribPointer( attr,typeCount,type,normalized, pVertexDecl->GetStreanmStride(), pVBufferData ) );

			GL_ASSERT( glEnableVertexAttribArray(attr) );
		}


		int indexStride = pRenderable->m_pIndexBuffer->GetStride();
		void* pIBufferData = BUFFER_OFFSET(indexStride * nIndexStart);

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pRenderable->m_pIndexBuffer->GetIndexType());

		GL_ASSERT( glDrawElements(ePrimType, nIndexCount, eIndexType, pIBufferData) );

		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = pVertexDecl->GetElement(i);
			GLint typeCount = 1; 
			GLenum type = 0;
			GLboolean normalized = false;
			std::string name;
			GLESMapping::GetGLESDeclType(ve.Usage,ve.UsageIndex,ve.Type,type,typeCount,normalized,name);
			VertexAttribute attr = pProgram->GetVertexAttribute(name.c_str());
			if (attr == -1)
				continue;

			GL_ASSERT( glDisableVertexAttribArray(attr) );
		}
	}

	void GLESRenderDevice::DrawDyRenderable(const Renderable* pRenderable,Technique* pTech)
	{
// 		if (pRenderable == NULL)
// 			return;
// 
// 		ASSERT(pTech);
// 		GLESShaderProgram* pProgram = (GLESShaderProgram*)pTech->GetShaderProgram();
// 
// 		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
// 		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
// 
// 		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration.get();
// 		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderable->m_pVertexBuffer.get();
// 		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderable->m_pIndexBuffer.get();
// 
// 		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData.get();
// 
// 		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
// 		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;
// 
// 		UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffer->GetNumber();
// 		UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;
// 
// 		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
// 		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
// 		int vertexStartByte = nVertexStart * pVertexDeclar->GetStreanmStride();
// 		GLvoid* pIndices = pIndexBuffer->GetData();
// 		GLvoid*	pVertex = pVertexBuffer->GetData();
// 
// 		int nSteam = pVertexDeclar->GetElementCount();
// 		for (int i = 0; i < nSteam; ++i)
// 		{
// 			const VertexElement& ve = pVertexDeclar->GetElement(i);
// 
// 			GLint typeCount = 1; 
// 			GLenum type = 0;
// 			GLboolean normalized = false;
// 			std::string name;
// 			GLESMapping::GetGLESDeclType(ve.Usage,ve.UsageIndex,ve.Type,type,typeCount,normalized,name);
// 			VertexAttribute attr = pProgram->GetVertexAttribute(name.c_str());
// 			if (attr == -1)
// 				continue;
// 
// 			int offset = vertexStartByte + ve.Offset;
// 			void* pVBufferData = (void*)(((unsigned char*)pVertex) + offset);	
// 
// 			GL_ASSERT( glVertexAttribPointer( attr,typeCount,type,normalized, pVertexDeclar->GetStreanmStride(), pVBufferData ) );
// 
// 			GL_ASSERT( glEnableVertexAttribArray(attr) );
// 		}
// 
// 
// 		if (pIndexBuffer)
// 		{
// 			GL_ASSERT( glDrawElements(ePrimType, nIndexCount, eIndexType, pIndices) );
// 		}
// 		else
// 		{
// 			//GL_ASSERT( glDrawArrays(_primitiveType, 0, _vertexCount) );
// 		}
	}


	void GLESRenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
		GLbitfield mask = 0;
		if (bColor)
		{
			GL_ASSERT( glClearColor(c.r, c.g, c.b, c.a) );
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if (bDepth)
		{
			GL_ASSERT( glClearDepthf(z) ); // glClearDepthf �ڱʼǱ��ϳ���
			mask |= GL_DEPTH_BUFFER_BIT;

			GL_ASSERT( glDepthMask(GL_TRUE) );
		}

		if (bStencil)
		{
			GL_ASSERT( glClearStencil(s) );
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		GL_ASSERT( glClear(mask) );
	}


	Matrix4 GLESRenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
	{
		float yScale = Math::Tan(Math::HALF_PI - fovy * 0.5f);
		float xScale = yScale/Aspect;
		float inv = 1.f/(zn - zf);

		out[0][0] = xScale; out[0][1] = 0.f;    out[0][2] = 0.f;            out[0][3] = 0.f;
		out[1][0] = 0.f;    out[1][1] = yScale; out[1][2] = 0.f;            out[1][3] = 0.f;
		out[2][0] = 0.f;    out[2][1] = 0.f;    out[2][2] = (zn+zf)*inv;    out[2][3] = 2.f*zn*zf*inv;
		out[3][0] = 0.f;    out[3][1] = 0.f;    out[3][2] = -1;             out[3][3] = 0.f;

		return out;
	}

	Matrix4 GLESRenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
	{
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;
		return MakeOrthoMatrixOffCenter(out, -halfWidth, halfWidth, -halfHeight, halfHeight, zn, zf);
	}

	Matrix4 GLESRenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
	{
		float r_l = 1.0f / (right - left);
		float t_b = 1.0f / (top - bottom);
		float f_n = 1.0f / (zf - zn);
		out = Matrix4::ZERO;
		out[0][0] = 2 * r_l;
		out[1][1] = 2 * t_b;
		out[2][2] = -2 * f_n;
		out[0][3] = -(right + left) * r_l;
		out[1][3] = -(top + bottom) * t_b;
		out[2][3] = -(zf+zn)*f_n;
		out[3][3] = 1.f;

		return out;
	}

	void GLESRenderDevice::BeginProfile(const char* pszLale)
	{
		//if (glPushGroupMarkerEXT)
		//	glPushGroupMarkerEXT(0,pszLale);
	}

	void GLESRenderDevice::EndProfile()
	{
		//if (glPopGroupMarkerEXT) 
		//	glPopGroupMarkerEXT();
	}

	bool GLESRenderDevice::CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage)
	{
		return true;
	}

}



