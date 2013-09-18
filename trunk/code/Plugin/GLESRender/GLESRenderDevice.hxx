#include "GLESRenderDevice.h"
#include "GLESTexture.h"
#include "GLESBase.h"




namespace ma
{

	GLESRenderDevice::GLESRenderDevice()
	{
	}

	GLESRenderDevice::~GLESRenderDevice()
	{
	}


	Texture* GLESRenderDevice::CreateTexture(const char* pszPath)
	{
		return new GLESTexture(pszPath);
	}

	Texture* GLESRenderDevice::CreateTexture(int nWidth,int nHeight,FORMAT format)
	{
		return new GLESTexture(nWidth,nHeight,format);
	}

	VertexDeclaration* GLESRenderDevice::CreateVertexDeclaration()
	{
		return new GLESVertexDeclaration();
	}

	VertexBuffer*	GLESRenderDevice::CreateVertexBuffer(void* pData, int nsize, int nStride, USAGE Usgae)
	{
		return new GLESVertexBuffer(pData,nsize,nStride,Usgae);
	}

	IndexBuffer*	GLESRenderDevice::CreateIndexBuffer(void* Data, int nSize, INDEX_TYPE eIndexType, USAGE Usgae)
	{
		return new GLESIndexBuffer(Data,nSize,eIndexType,Usgae);
	}

	ShaderProgram*	GLESRenderDevice::CreateShaderProgram(const char* pszName,const char* pszDefine)
	{
		return new GLESShaderProgram(pszName,pszDefine);
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
#if PLATFORM_WIN == 1
		//------------------------------------------------------------------------------
		//Create OpenGL Context
		//------------------------------------------------------------------------------
		m_hDC = GetDC(wndhandle);
		int iPF = 0;

		PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
			1,                     // version number 
			PFD_DRAW_TO_WINDOW |   // support window 
			PFD_SUPPORT_OPENGL |   // support OpenGL 
			PFD_DOUBLEBUFFER,      // double buffered 
			PFD_TYPE_RGBA,         // RGBA type 
			24,                    // 24-bit color depth 
			8, 0, 8, 0, 8, 0,      // color bits ignored 
			0,                     // no alpha buffer 
			0,                     // shift bit ignored 
			0,                     // no accumulation buffer 
			0, 0, 0, 0,            // accum bits ignored 
			24,                    // 32-bit z-buffer     
			8,                     // no stencil buffer 
			0,                     // no auxiliary buffer 
			PFD_MAIN_PLANE,        // main layer 
			0,                     // reserved 
			0, 0, 0                // layer masks ignored 
		}; 
		bool bOK = true;

		if( !(iPF = ChoosePixelFormat( m_hDC, &pfd )) ) 
		{
			bOK = false;
		}

		if( !SetPixelFormat( m_hDC, iPF, &pfd ) ) 
		{
			bOK =  false;
		}

		if( !(m_hGLRC = wglCreateContext( m_hDC )) ) 
		{
			bOK =  false;
		}


		BOOL bMCOK = wglMakeCurrent(m_hDC,m_hGLRC);
		ASSERT(bMCOK && "Fail to init gl context");

		// Initialize GLEW
		if (GLEW_OK != glewInit())
		{
			wglDeleteContext(m_hGLRC);
			DestroyWindow(wndhandle);
			GP_ERROR("Failed to initialize GLEW.");
			return ;
		}

#endif	
		
		GLint fbo;
		GL_ASSERT( glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo) );
		GLESRenderTarget* pRenderTarget = new GLESRenderTarget();
		pRenderTarget->SetFrameBuffer(fbo);
		m_pRenderTarget.push(pRenderTarget);
		

		m_hDefaultFrameBuffer = fbo;
		GL_ASSERT( glGenFramebuffers(1, &m_hOffecreenFrameBuffer) );

		GLint viewport[4]; 
		GL_ASSERT( glGetIntegerv(GL_VIEWPORT, viewport) );
		m_viewport.push(Rectangle(viewport[0],viewport[1],viewport[2],viewport[3]));

#ifdef GL_MAX_COLOR_ATTACHMENTS
		GLint val;
		GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
#endif
	}

	void GLESRenderDevice::BeginRender()
	{

#if PLATFORM_WIN == 1
		wglMakeCurrent(m_hDC,m_hGLRC);
#endif
	}

	void GLESRenderDevice::EndRender()
	{
		GL_ASSERT( glFlush() );


#if PLATFORM_WIN == 1
		GL_ASSERT( SwapBuffers(m_hDC) );
#endif
	}

	RenderTarget* GLESRenderDevice::CreateRenderTarget(int nWidth,int nHeight,FORMAT format)
	{
		GLESRenderTarget* pTarget = new GLESRenderTarget(nWidth,nHeight,format);
		pTarget->Create();
		pTarget->SetFrameBuffer(m_hOffecreenFrameBuffer);
		return pTarget;
	}

	void GLESRenderDevice::PushRenderTarget(RenderTarget* pTarget,int index)
	{
		ASSERT(pTarget && index == 0);
		if (pTarget == NULL || index != 0)
			return ;

		GLESRenderTarget* pGLESTargert = (GLESRenderTarget*)pTarget;
		GLESTexture* pGLESTexure = (GLESTexture*)pGLESTargert->GetTexture();

		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, pGLESTargert->GetFrameBuffer()) );
	
		if (pGLESTexure)
		{
			GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
			GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pGLESTexure->GetTexture(), 0) );
			GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			{
				GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
			}
		}
		
		m_pRenderTarget.push(pGLESTargert);
	}

	void GLESRenderDevice::PopRenderTarget(int index)
	{
		ASSERT(index == 0);
		if (index != 0)
			return;

		m_pRenderTarget.pop();
		GLESRenderTarget* pGLESTargert = m_pRenderTarget.top();
		GLESTexture* pGLESTexure = (GLESTexture*)pGLESTargert->GetTexture();

		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, pGLESTargert->GetFrameBuffer()) );

		if (pGLESTexure && pGLESTexure->GetTexture() > 0)
		{
			GLenum attachment = GL_COLOR_ATTACHMENT0 + index;
			GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pGLESTexure->GetTexture(), 0) );
			GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			{
				GP_ERROR("Framebuffer status incomplete: 0x%x", fboStatus);
			}
		}
	}


	void GLESRenderDevice::PushDepthStencil(RenderTarget* pTexture)
	{
		ASSERT(false);
	}

	void GLESRenderDevice::PopDepthStencil()
	{
		ASSERT(false);
	}

	void GLESRenderDevice::PushViewport(const Rectangle& rect)
	{
		glViewport((GLuint)rect.x, (GLuint)rect.y, (GLuint)rect.width, (GLuint)rect.height);	
		m_viewport.push(rect);
	}

	void GLESRenderDevice::PopViewport()
	{
		m_viewport.pop();
		Rectangle rect = m_viewport.top();
		glViewport((GLuint)rect.x, (GLuint)rect.y, (GLuint)rect.width, (GLuint)rect.height);
	}

	void GLESRenderDevice::SetRenderState(const RenderState& state)
	{
// 		//		DWORD cullMode = state.cullMode;
// 
// 		// 		if (mFlipCullMode)
// 		// 		{
// 		// 			if (cullMode == D3DCULL_CCW)
// 		// 				cullMode = D3DCULL_CW;
// 		// 			else if (cullMode == D3DCULL_CW)
// 		// 				cullMode = D3DCULL_CCW;
// 		// 		}
// 
 
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
			GL_ASSERT( glBlendFunc(GL_ONE, GL_ONE) );
			break;

		case BM_MULTIPLY:
			break;
		}
	}



	#define BUFFER_OFFSET(offset) ((Int8 *) NULL + offset)

	void GLESRenderDevice::DrawIndexMesh(const IndexMesh& indexMesh)
	{
		Technique* pCurTech = indexMesh.m_pTech;
		ASSERT(pCurTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pCurTech->GetShaderProgram();

		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)indexMesh.m_pIndBuf;
		GL_ASSERT( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() ) );

		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)indexMesh.m_pVerBuf;
		GL_ASSERT( glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() ) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)indexMesh.m_pDecl;

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(indexMesh.m_eMeshType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = indexMesh.m_nVertexStart * pVertexDeclar->GetStreanmStride();

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
		void* pIBufferData = BUFFER_OFFSET(indexStride * indexMesh.m_nIndexStart);

		GL_ASSERT( glDrawElements(ePrimType, indexMesh.m_nIndexCount, eIndexType, pIBufferData) );


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

	void GLESRenderDevice::DrawDyIndexMesh(const IndexMesh& indexMesh)
	{
		Technique* pCurTech = indexMesh.m_pTech;
		ASSERT(pCurTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pCurTech->GetShaderProgram();

		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)indexMesh.m_pDecl;
		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)indexMesh.m_pVerBuf;
		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)indexMesh.m_pIndBuf;

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(indexMesh.m_eMeshType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = indexMesh.m_nVertexStart * pVertexDeclar->GetStreanmStride();
		GLsizei nIndexCount = indexMesh.m_nIndexCount;
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


	void GLESRenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
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


	Matrix4x4 GLESRenderDevice::MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
	{
		 MatrixPerspectiveFovGL_LH(pOut,fovy,Aspect,zn,zf);
		 return *pOut;
	}

	Matrix4x4 GLESRenderDevice::MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf)
	{
		MatrixOrthoGL_LH(pOut,width,height,zn,zf);
		return *pOut;
	}

	Matrix4x4 GLESRenderDevice::MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf)
	{
		MatrixOrthoOffCenterGL_LH(pOut,left,right,bottom,top,zn,zf);
		return *pOut;
	}


}



