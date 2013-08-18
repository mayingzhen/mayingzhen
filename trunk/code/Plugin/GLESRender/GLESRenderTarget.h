#ifndef  _GLESRenderTarget__H__
#define  _GLESRenderTarget__H__



namespace ma
{
	class GLESRENDER_API GLESRenderTarget : public RenderTarget
	{
	public:
		GLESRenderTarget();

		~GLESRenderTarget();

		virtual Texture*	GetTexture();

		bool				Create(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8);

		void				SetFrameBuffer(FrameBufferHandle handele) {m_handle = handele;}

		FrameBufferHandle	GetFrameBuffer() {return m_handle;}

	private:
		GLESTexture*		m_pTexture;

		FrameBufferHandle	m_handle;
		
	};

}


#endif