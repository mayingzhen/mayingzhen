#include "GLESRenderDevice.h"
//#include "GLESRendMesh.h"
#include "GLESTexture.h"





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

	ShaderProgram*			GLESRenderDevice::CreateShaderProgram()
	{
		return new GLESShaderProgram();
	}

	const char*			GLESRenderDevice::GetShaderPath()
	{
		return "/shader/gles/";
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
	}

	void GLESRenderDevice::BeginRender()
	{
		//BOOL bOK = wglMakeCurrent(m_hDC,m_hGLRC);

		Color clearColor(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
		GL_ASSERT( ClearBuffer(true,true,true,clearColor,1.0f,0) );

		GL_ASSERT( glEnable(GL_DEPTH_TEST) );
	}

	void GLESRenderDevice::EndRender()
	{
		GL_ASSERT( glFlush() );


#if PLATFORM_WIN == 1
		GL_ASSERT( SwapBuffers(m_hDC) );
#endif
	}

	void GLESRenderDevice::SetRenderTarget(int index,Texture* pTexture)
	{
// 		if (pTexture != NULL)
// 		{
// 			HRESULT hr = D3D_OK;
// 			GLESTexture* pGLESTexture = static_cast<GLESTexture*>(pTexture);
// 			IDirect3DSurface9* target = pGLESTexture->GetD3DSurface();
// 
// 			hr = m_pD3DDevice->SetRenderTarget(index, target);
// 			ASSERT_MSG(hr == D3D_OK, "set render target failed.");
// 		}
// 		else
// 		{
// 			m_pD3DDevice->SetRenderTarget(index, NULL);
// 		}
// 
// 		//mRenderTarget[index] = target;
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
// 		//GetD3D9DxDevive()->SetRenderState(D3DRS_CULLMODE, cullMode);
// 		//GetD3D9DxDevive()->SetRenderState(D3DRS_FILLMODE, state.fillMode);
// 
// 		//mD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, state.colorWrite);
// 
		GL_ASSERT( glDepthMask(state.m_bDepthWrite ? GL_TRUE : GL_FALSE) );
// 		
// 
// 		switch (state.m_eDepthCheckMode)
// 		{
// 		case DCM_LESS_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 
// 			//GetD3D9DxDevive()->SetRenderState(D3DRS_ZENABLE, TRUE);
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
// 			break;
// 
// 		case DCM_LESS:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
// 			break;
// 
// 		case DCM_GREATER_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
// 			break;
// 
// 		case DCM_GREATER:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
// 			break;
// 
// 		case DCM_EQUAL:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
// 			break;
// 
// 		case DCM_ALWAYS:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,true) );
// 			GetD3D9DxDevive()->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
// 			break;
// 
// 		case DCM_NONE:
// 			GL_ASSERT( glEnable(GL_DEPTH_TEST,false) );
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


		//m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		// 		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
		// 		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,true);
		// 		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		// 		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		// 		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//glEnable(GL_DEPTH_TEST);
		//glDepthMask(false);
		//glEnable(GL_BLEND);
		//glDisable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		Material* pMaterial = pRenderable->m_pMaterial;
		pMaterial->Bind();
		GLESShaderProgram* pProgram = (GLESShaderProgram*)pMaterial->GetShaderProgram();

		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderable->m_pIndexBuffer;
		GL_ASSERT( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() ) );

		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderable->m_pVertexBuffers;
		GL_ASSERT( glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() ) );

		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderable->m_pDeclaration;

		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderable->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		int vertexStartByte = pRenderable->m_nVertexStart * pVertexDeclar->GetStreanmStride();

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

			void* pVBufferData = BUFFER_OFFSET( vertexStartByte + ve.Offset );		

			GL_ASSERT( glVertexAttribPointer( attr,typeCount,type,normalized, pVertexDeclar->GetStreanmStride(), pVBufferData ) );

			GL_ASSERT( glEnableVertexAttribArray(attr) );
		}


		int indexStride = pIndexBuffer->GetStride();
		void* pIBufferData = BUFFER_OFFSET(indexStride * pRenderable->m_nIndexStart);

		GL_ASSERT( glDrawElements(ePrimType, pRenderable->m_nIndexCount, eIndexType, pIBufferData) );



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

			GL_ASSERT( glDisableVertexAttribArray(attr) );
		}

		pMaterial->UnBind();
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
			GL_ASSERT( glClearDepthf(z) );
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



