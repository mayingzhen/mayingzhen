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


	Texture* GLESRenderDevice::CreateRendTexture()
	{
		return new GLESTexture();
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

	ShaderProgram*	GLESRenderDevice::CreateShaderProgram()
	{
		return new GLESShaderProgram();
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
		m_pCurRenderTarget = new GLESRenderTarget();
		m_pCurRenderTarget->SetFrameBuffer(fbo);

		m_hDefaultFrameBuffer = fbo;
		GL_ASSERT( glGenFramebuffers(1, &m_hOffecreenFrameBuffer) );

		GLint viewport[4]; 
		GL_ASSERT( glGetIntegerv(GL_VIEWPORT, viewport) );
		m_curViewport.x = viewport[0];
		m_curViewport.y = viewport[1];
		m_curViewport.width = viewport[2];
		m_curViewport.height = viewport[3];

#ifdef GL_MAX_COLOR_ATTACHMENTS
		GLint val;
		GL_ASSERT( glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &val) );
#endif
	}

	void GLESRenderDevice::BeginRender()
	{
// 		Color clearColor(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
// 		GL_ASSERT( ClearBuffer(true,true,true,clearColor,1.0f,0) );

		//GL_ASSERT( glEnable(GL_DEPTH_TEST) );
		//GL_ASSERT( glDepthMask(GL_TRUE) );
		//GL_ASSERT( glDisable(GL_CULL_FACE) );
		//GL_ASSERT( glEnable(GL_CULL_FACE) );
		//GL_ASSERT( glCullFace(GL_FRONT) );

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
		GLESRenderTarget* pTarget = new GLESRenderTarget();
		pTarget->Create(nWidth,nHeight,format);
		pTarget->SetFrameBuffer(m_hOffecreenFrameBuffer);
		return pTarget;
	}

	RenderTarget* GLESRenderDevice::SetRenderTarget(RenderTarget* pTarget,int index)
	{
		ASSERT(pTarget && index == 0);
		if (pTarget == NULL || index != 0)
			return NULL;

		GLESRenderTarget* preTarger = m_pCurRenderTarget;
		m_pCurRenderTarget = (GLESRenderTarget*)pTarget;
		GLESTexture* pGLESTexure = (GLESTexture*)m_pCurRenderTarget->GetTexture();

		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, m_pCurRenderTarget->GetFrameBuffer()) );
	
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
		return preTarger;
	}

	RenderTarget* GLESRenderDevice::SetDepthStencil(RenderTarget* pTexture)
	{
		ASSERT(false);
		return NULL;
	}

	Rectangle GLESRenderDevice::SetViewport(const Rectangle& rect)
	{
		Rectangle preViewport = m_curViewport;
		m_curViewport = rect;
		glViewport((GLuint)rect.x, (GLuint)rect.y, (GLuint)rect.width, (GLuint)rect.height);
		return preViewport;
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
// 		//GetGLESDxDevive()->SetRenderState(D3DRS_CULLMODE, cullMode);
// 		//GetGLESDxDevive()->SetRenderState(D3DRS_FILLMODE, state.fillMode);
// 
// 		//mD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, state.colorWrite);
// 
		//GL_ASSERT( glDepthMask(state.m_bDepthWrite ? GL_TRUE : GL_FALSE) );
		

// 		switch (state.m_eDepthCheckMode)
// 		{
// 		case DCM_NONE:
// 			GL_ASSERT( glDisable(GL_DEPTH_TEST) );
// 			break;
// 
// 		case DCM_LESS_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
// 			GL_ASSERT( glDepthFunc(GL_LEQUAL) );
// 			break;
// 
// 		case DCM_LESS:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
// 			GL_ASSERT( glDepthFunc(GL_LESS) );
// 			break;
// 
// 		case DCM_GREATER_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
// 			GL_ASSERT( glDepthFunc(GL_GEQUAL) );
// 			break;
// 
// 		case DCM_GREATER:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
// 			GL_ASSERT( glDepthFunc(GL_GREATER) );
// 			break;
// 
// 		case DCM_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
// 			GL_ASSERT( glDepthFunc(GL_EQUAL) );
// 			break;
// 		default:
// 			ASSERT(false);
// 			break;
// 		}
// 
		switch (state.m_eBlendMode)
		{
		case BM_OPATICY:
			GL_ASSERT( glDisable(GL_BLEND) );
			break;

		case BM_TRANSPARENT:
			GL_ASSERT( glEnable(GL_BLEND) );
			GL_ASSERT( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
			//GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;

		case BM_ADD:
			GL_ASSERT( glEnable(GL_BLEND) );
			GL_ASSERT( glBlendFunc(GL_SRC_ALPHA, GL_ONE) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case BM_MULTIPLY:
			//GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			//GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			//GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			break;
		}
	}


	void GLESRenderDevice::GetRenderWndSize(int& Width,int& Heigh)
	{
// 		RECT rect;
// 		GetWindowRect(m_hWnd,&rect);
// 		Width = rect.right - rect.left;
// 		Heigh = rect.bottom - rect.top;
	}


	#define BUFFER_OFFSET(offset) ((Int8 *) NULL + offset)

	void GLESRenderDevice::DrawRenderable(Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		if (pRenderable->m_pSubMeshData && pRenderable->m_pSubMeshData->m_nVertexCount <= 0)
			return;

		glEnable(GL_DEPTH_TEST);
		//glDepthMask(TRUE);
		//glEnable(GL_BLEND);
		//glDisable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		Material* pMaterial = pRenderable->m_pMaterial;
		pMaterial->Bind();
		
		Technique* pCurTech = pMaterial->GetCurTechnqiue();
		ASSERT(pCurTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pCurTech->GetShaderProgram();

		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderable->m_pIndexBuffer;
		GL_ASSERT( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() ) );

		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderable->m_pVertexBuffers;
		GL_ASSERT( glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() ) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration;

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = pRenderable->m_pSubMeshData->m_nVertexStart * pVertexDeclar->GetStreanmStride();

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
		void* pIBufferData = BUFFER_OFFSET(indexStride * pRenderable->m_pSubMeshData->m_nIndexStart);

		GL_ASSERT( glDrawElements(ePrimType, pRenderable->m_pSubMeshData->m_nIndexCount, eIndexType, pIBufferData) );



		//nSteam = pVertexDeclar->GetElementCount();
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

		pMaterial->UnBind();
	}

	void GLESRenderDevice::DrawDynamicRenderable(Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		if (pRenderable->m_pSubMeshData && pRenderable->m_pSubMeshData->m_nVertexCount <= 0)
			return;

		glEnable(GL_DEPTH_TEST);

		Material* pMaterial = pRenderable->m_pMaterial;
		pMaterial->Bind();

		Technique* pCurTech = pMaterial->GetCurTechnqiue();
		ASSERT(pCurTech);
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pCurTech->GetShaderProgram();

		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 ) );
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration;

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pRenderable->m_pIndexBuffer->GetIndexType());
		int vertexStartByte = pRenderable->m_pSubMeshData->m_nVertexStart * pVertexDeclar->GetStreanmStride();
		GLsizei nIndexCount = pRenderable->m_pSubMeshData->m_nIndexCount;
		GLvoid* pIndices = pRenderable->m_pIndexBuffer->GetData();
		GLvoid*	pVertex = pRenderable->m_pVertexBuffers->GetData();

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


		if (pRenderable->m_pIndexBuffer)
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
		}

		if (bStencil)
		{
			GL_ASSERT( glClearStencil(s) );
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		GL_ASSERT( glClear(mask) );
	}


	void GLESRenderDevice::DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor)
	{
// 		if (m_pLineRender == NULL)
// 			return;
// 
// 		m_pLineRender->DrawLine(p0,p1,dwColor);
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



