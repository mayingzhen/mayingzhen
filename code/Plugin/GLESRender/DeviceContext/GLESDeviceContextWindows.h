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
		HDC					m_hDC;

		/// EGL display
		EGLDisplay			m_eglDisplay;

		/// EGL context
		EGLContext			m_eglContext;

		/// EGL surface
		EGLSurface			m_eglSurface;
	};

}

#endif




