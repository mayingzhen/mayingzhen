#include "GLESDeviceContextWindows.h"

namespace ma
{
	void GLESDeviceContext::Init(HWND wndhandle)
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

	void GLESDeviceContext::SwapBuffers()
	{
		GL_ASSERT( ::SwapBuffers(m_hDC) );
	}
}



