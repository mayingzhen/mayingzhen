#include "GLESRenderDevice.h"
#include "GLESTexture.h"
#include "GLESBase.h"

#include "DeviceContext/GLESDeviceContext.h"


namespace ma
{

	GLESRenderDevice::GLESRenderDevice()
	{
		m_pDeviceContext = new GLESDeviceContext();
	}

	GLESRenderDevice::~GLESRenderDevice()
	{
		SAFE_DELETE(m_pDeviceContext);
	}


	Texture* GLESRenderDevice::CreateTexture(const char* pszPath)
	{
		return new GLESTexture(pszPath);
	}

	Texture* GLESRenderDevice::CreateTexture(int nWidth,int nHeight,FORMAT format,bool bDepthStencil)
	{
		GLESTexture* pTarget = new GLESTexture(nWidth,nHeight,format,bDepthStencil);
		pTarget->SetFrameBuffer(m_hOffecreenFrameBuffer);
		return pTarget;
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

	ShaderProgram*	GLESRenderDevice::CreateShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine)
	{
		return new GLESShaderProgram(pTech,pVSFile,pPSFile,pszDefine);
	}

	const char*	GLESRenderDevice::GetShaderPath()
	{
		return "shader/gles/";
	}

	void	GLESRenderDevice::ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom)
	{
		float fUVTop = fBottom;
		float fUVBottom = fTop;
		
		fTop = fUVTop;
		fBottom = fUVBottom;
	}

	float	GLESRenderDevice::GetHalfPixelOffset(float fHalfPiexl)
	{
		return 0;
	}

	void GLESRenderDevice::Init(HWND wndhandle)
	{
		m_pDeviceContext->Init(wndhandle);

// 		GLint fbo;
// 		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo) );
// 		m_pRenderTarget = new GLESTexture(-1,-1);
// 		m_pRenderTarget->SetFrameBuffer(fbo);
// 		
// 		GLint hDefaultFrameBuffer = fbo;
 		GL_ASSERT( glGenFramebuffers(1, &m_hOffecreenFrameBuffer) );
// 
// 		GLint viewport[4]; 
// 		GL_ASSERT( glGetIntegerv(GL_VIEWPORT, viewport) );
// 		m_viewport.push(Rectangle((float)viewport[0],(float)viewport[1],(float)viewport[2],(float)viewport[3]));

#ifdef GL_MAX_COLOR_ATTACHMENTS
		GLint val;
		GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
#endif
	}

	void GLESRenderDevice::BeginRender()
	{
// #if PLATFORM_WIN == 1
// 		wglMakeCurrent(m_hDC,m_hGLRC);
// #endif
        //m_pDeviceContext->MakeCurrent();
	}

	void GLESRenderDevice::EndRender()
	{
		GL_ASSERT( glFlush() );

		m_pDeviceContext->SwapBuffers();
	}

	void GLESRenderDevice::SetRenderTarget(Texture* pTexture,int index)
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
				GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
			}
		}
	}

	Texture* GLESRenderDevice::GetRenderTarget(int index)
	{
		if (index > 0)
			 return NULL;

		GLESTexture* pGLESTarget = new GLESTexture(-1,-1);

		GLint hFboBinding;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &hFboBinding) );

		pGLESTarget->SetFrameBuffer(hFboBinding);

		return pGLESTarget;
	}


	void GLESRenderDevice::SetDepthStencil(Texture* pTexture)
	{
		ASSERT(false);
	}

	Texture* GLESRenderDevice::GetDepthStencil()
	{
		//ASSERT(false);
		return NULL;
	}

	void GLESRenderDevice::SetViewport(const Rectangle& rect)
	{
		glViewport((GLuint)rect.x, (GLuint)rect.y, (GLuint)rect.width, (GLuint)rect.height);
	}

	Rectangle GLESRenderDevice::GetViewport()
	{
		Rectangle rect;

		GLint viewport[4]; 
 		GL_ASSERT( glGetIntegerv(GL_VIEWPORT, viewport) );
		
		rect.x = (float)viewport[0];
		rect.y = (float)viewport[1];
		rect.width = (float)viewport[2];
		rect.height = (float)viewport[3];

		return rect;
	}

	void GLESRenderDevice::SetRenderState(const RenderState& state)
	{
		GL_ASSERT( glDepthMask(state.m_bDepthWrite ? GL_TRUE : GL_FALSE) );

		switch (state.m_eDepthCheckMode)
		{
		case DCM_NONE:
			GL_ASSERT( glDisable(GL_DEPTH_TEST) );
			break;

		case DCM_LESS_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_LEQUAL) );
			break;

		case DCM_LESS:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_LESS) );
			break;

		case DCM_GREATER_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_GEQUAL) );
			break;

		case DCM_GREATER:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_GREATER) );
			break;

		case DCM_EQUAL:
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
			GL_ASSERT( glDepthFunc(GL_EQUAL) );
			break;
		default:
			ASSERT(false);
			break;
		}
 
		switch (state.m_eBlendMode)
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
	}



	#define BUFFER_OFFSET(offset) ((Int8 *) NULL + offset)

	void GLESRenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		profile_code();

		ASSERT(pTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pTech->GetShaderProgram();

		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderable->m_pIndexBuffer.get();
		GL_ASSERT( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() ) );

		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderable->m_pVertexBuffers.get();
		GL_ASSERT( glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() ) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration.get();

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData.get();

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = pSubMeshData->m_nVertexStart * pVertexDeclar->GetStreanmStride();

		int nSteam = pVertexDeclar->GetElementCount();
		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = pVertexDeclar->GetElement(i);

			GLint typeCount = 1; 
			GLenum type = 0;
			GLboolean normalized = false;
			std::string name;
			GLESMapping::GetGLESDeclType(ve.Usage,type,typeCount,normalized,name);
			VertexAttribute attr = pProgram->getVertexAttribute(name.c_str());
			if (attr == -1)
				continue;

			void* pVBufferData = BUFFER_OFFSET( vertexStartByte + ve.Offset );		

			GL_ASSERT( glVertexAttribPointer( attr,typeCount,type,normalized, pVertexDeclar->GetStreanmStride(), pVBufferData ) );

			GL_ASSERT( glEnableVertexAttribArray(attr) );
		}


		int indexStride = pIndexBuffer->GetStride();
		void* pIBufferData = BUFFER_OFFSET(indexStride * pSubMeshData->m_nIndexStart);

		GL_ASSERT( glDrawElements(ePrimType, pSubMeshData->m_nIndexCount, eIndexType, pIBufferData) );


		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = pVertexDeclar->GetElement(i);
			GLint typeCount = 1; 
			GLenum type = 0;
			GLboolean normalized = false;
			std::string name;
			GLESMapping::GetGLESDeclType(ve.Usage,type,typeCount,normalized,name);
			VertexAttribute attr = pProgram->getVertexAttribute(name.c_str());
			if (attr == -1)
				continue;

			GL_ASSERT( glDisableVertexAttribArray(attr) );
		}
	}

	void GLESRenderDevice::DrawDyRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		ASSERT(pTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pTech->GetShaderProgram();

		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration.get();
		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderable->m_pVertexBuffers.get();
		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderable->m_pIndexBuffer.get();

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData.get();

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = pSubMeshData->m_nVertexStart * pVertexDeclar->GetStreanmStride();
		GLsizei nIndexCount = pSubMeshData->m_nIndexCount;
		GLvoid* pIndices = pIndexBuffer->GetData();
		GLvoid*	pVertex = pVertexBuffer->GetData();

		int nSteam = pVertexDeclar->GetElementCount();
		for (int i = 0; i < nSteam; ++i)
		{
			const VertexElement& ve = pVertexDeclar->GetElement(i);

			GLint typeCount = 1; 
			GLenum type = 0;
			GLboolean normalized = false;
			std::string name;
			GLESMapping::GetGLESDeclType(ve.Usage,type,typeCount,normalized,name);
			VertexAttribute attr = pProgram->getVertexAttribute(name.c_str());
			if (attr == -1)
				continue;

			int offset = vertexStartByte + ve.Offset;
			void* pVBufferData = (void*)(((unsigned char*)pVertex) + offset);	

			GL_ASSERT( glVertexAttribPointer( attr,typeCount,type,normalized, pVertexDeclar->GetStreanmStride(), pVBufferData ) );

			GL_ASSERT( glEnableVertexAttribArray(attr) );
		}


		if (pIndexBuffer)
		{
			GL_ASSERT( glDrawElements(ePrimType, nIndexCount, eIndexType, pIndices) );
		}
		else
		{
			//GL_ASSERT( glDrawArrays(_primitiveType, 0, _vertexCount) );
		}
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
			GL_ASSERT( glClearDepth(z) ); // glClearDepthf 在笔记本上出错
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
		Radian thetaY(fovy / 2.0f);
		float factor = 1.f/Math::Tan(thetaY);
		float f_n = 1.f/(zf-zn);

		// Calc matrix elements
		float w = factor / Aspect;
		float h = factor;
		float q = -(zf + zn)*f_n;
		float qn = -2 * (zf * zn)*f_n;

		// NB This creates Z in range [-1,1]
		//
		// [ w   0   0   0  ]
		// [ 0   h   0   0  ]
		// [ 0   0   q   qn ]
		// [ 0   0   -1  0  ]

		out = Matrix4::ZERO;
		out[0][0] = w;
		out[1][1] = h;
		out[2][2] = q;
		out[2][3] = qn;
		out[3][2] = -1;

		return out;
	}

	Matrix4 GLESRenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
	{
		float halfWidth = width*0.5f;
		float halfHeight = height*0.5f;
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
        glPushGroupMarkerEXT(0,pszLale);
	}

	void GLESRenderDevice::EndProfile()
	{
        glPopGroupMarkerEXT();
	}

	bool GLESRenderDevice::CheckTextureFormat(FORMAT eFormat,USAGE eUsage)
	{
		return true;
	}

}



