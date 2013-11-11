#ifndef  _GLESDeveiveContextAndroid__H__
#define  _GLESDeveiveContextAndroid__H__

namespace ma
{
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
		};

	}

}

#endif