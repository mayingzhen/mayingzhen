#ifndef  _GLESRenderTarget__H__
#define  _GLESRenderTarget__H__



namespace ma
{
	class GLESRENDER_API GLESRenderTarget : public RenderTarget
	{
	public:
		GLESRenderTarget();

		~GLESRenderTarget();

		virtual bool		Create(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8);

		void				SetFrameBuffer(FrameBufferHandle handele) {m_handle = handele;}

		FrameBufferHandle	GetFrameBuffer() {return m_handle;}

	private:
		FrameBufferHandle	m_handle;
		
	};

}


#endif