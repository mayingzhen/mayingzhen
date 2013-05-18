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

	VertexBuffer*	GLESRenderDevice::CreateVertexBuffer()
	{
		return new GLESVertexBuffer();
	}

	IndexBuffer*	GLESRenderDevice::CreateIndexBuffer()
	{
		return new GLESIndexBuffer();
	}

	Technique*		GLESRenderDevice::CreateTechnique()
	{
		return new GLESTechnique();
	}

	void GLESRenderDevice::Init(HWND wndhandle)
	{

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

		
	}

	void GLESRenderDevice::BeginRender()
	{
		BOOL bOK = wglMakeCurrent(m_hDC,m_hGLRC);

// 		glClearColor(m_clearCol.r, m_clearCol.g, m_clearCol.b, m_clearCol.a);
// 		glClearDepthf(1.0);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Color clearColor(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
		ClearBuffer(true,true,true,clearColor,1.0f,0);

		//m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
		
		//m_pD3DDevice->BeginScene();
	}

	void GLESRenderDevice::EndRender()
	{
		//FlushRenderQueue();

		//FlushLine();

		//m_pD3DDevice->EndScene();
		//m_pD3DDevice->Present(NULL,NULL,NULL,NULL);

		glFlush();

		SwapBuffers(m_hDC);
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


	void GLESRenderDevice::GetRenderWndSize(int& Width,int& Heigh)
	{
// 		RECT rect;
// 		GetWindowRect(m_hWnd,&rect);
// 		Width = rect.right - rect.left;
// 		Heigh = rect.bottom - rect.top;
	}


	#define BUFFER_OFFSET(offset) ((Int8 *) NULL + offset)

	void GLESRenderDevice::DrawRenderMesh(RenderMesh* pRenderMesh,Technique* pTech)
	{
		//GL_CHECK_ERROR;

		//glDisable(GL_CULL_FACE);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);

		if (pRenderMesh == NULL)
			return;

		// Bind index buffer
		GLESIndexBuffer* pIndexBuffer = (GLESIndexBuffer*)pRenderMesh->m_pIndexBuffer;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetIndexBuffer() );
		//GL_CHECK_ERROR;

		// Bind Vertex Buffer
		GLESVertexBuffer* pVertexBuffer = (GLESVertexBuffer*)pRenderMesh->m_pVertexBuffers;
		glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->GetVertexBuffer() );
		//GL_CHECK_ERROR;

		// Vertex Declar
		GLESVertexDeclaration* pVertexDeclar = (GLESVertexDeclaration*)pRenderMesh->m_pDeclaration;

 		GLenum ePrimType = GLESMapping::GetGLESPrimitiveType(pRenderMesh->m_ePrimitiveType);
		GLenum eIndexType = GLESMapping::GetGLESIndexType(pIndexBuffer->GetIndexType());
		for (UINT i = 0; i < pRenderMesh->m_pMesData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pRenderMesh->m_pMesData->GetSubMeshByIndex(i);
			if (pSubMesh == NULL)
				continue;

			GLESTechnique* glesTech = (GLESTechnique*)pTech;
			pVertexDeclar->Active(pSubMesh->m_nVertexStart,glesTech->GetCurEffect());
			//GL_CHECK_ERROR;

			UINT nPrimCount = 0;
			if (ePrimType == GL_TRIANGLES)
			{
				nPrimCount = pSubMesh->m_nIndexCount / 3;
			}
			else if (ePrimType == GL_TRIANGLE_STRIP)
			{
				nPrimCount = pSubMesh->m_nIndexCount - 2;
			}

			int indexStride = pIndexBuffer->GetStride();
			void* pBufferData = BUFFER_OFFSET(indexStride * pSubMesh->m_nIndexStart);

			glDrawElements(ePrimType, pSubMesh->m_nIndexCount, eIndexType, pBufferData);
			//GL_CHECK_ERROR;

		}	
	}

	void GLESRenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
	{
		GLbitfield mask = 0;
		if (bColor)
		{
			glClearColor(c.r, c.g, c.b, c.a);
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if (bDepth)
		{
			glClearDepthf(z);
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (bStencil)
		{
			glClearStencil(s);
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(mask);
	}


	void GLESRenderDevice::DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor)
	{
// 		if (m_pLineRender == NULL)
// 			return;
// 
// 		m_pLineRender->DrawLine(p0,p1,dwColor);
	}

	Matrix4x4 GLESRenderDevice::MakeProjectionMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
	{
		 MatrixPerspectiveFovGL_LH(pOut,fovy,Aspect,zn,zf);
		 return *pOut;
	}


}



