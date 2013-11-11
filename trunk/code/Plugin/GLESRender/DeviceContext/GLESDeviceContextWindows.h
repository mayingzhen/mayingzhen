#ifndef  _GLESDeveiveContextWindows__H__
#define  _GLESDeveiveContextWindows__H__

namespace ma
{
	class GLESDeviceContext
	{
	public:
		virtual void		Init(HWND wndHandle);

		virtual void		SwapBuffers(); 

	protected:
		GLuint				m_hDefaultFrameBuffer;
		GLuint				m_hColorRenderbuffer;
		GLuint				m_hDepthRenderbuffer;

		GLint				m_nFramebufferWidth;
		GLint				m_nFramebufferHeight;

		HDC					m_hDC;

		HGLRC				m_hGLRC;
	};

}

#endif




