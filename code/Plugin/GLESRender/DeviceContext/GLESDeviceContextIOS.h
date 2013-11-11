#ifndef  _GLESDeveiveContextIOS__H__
#define  _GLESDeveiveContextIOS__H__



namespace ma
{
    
	class GLESDeviceContext
	{
	public:
		virtual bool		Init(HWND wndHandle);
        
        virtual void        MakeCurrent();

		virtual void		SwapBuffers(); 

	protected:
		GLuint				m_hDefaultFramebuffer;
		GLuint				m_hColorRenderbuffer;
		GLuint				m_hDepthRenderbuffer;

		GLint				m_nFramebufferWidth;
		GLint				m_nFramebufferHeight;
        
        void*               m_pDeviceGLRC;
	};
}

#endif
