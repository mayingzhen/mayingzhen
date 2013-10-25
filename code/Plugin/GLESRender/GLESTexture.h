#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESRENDER_API GLESTexture : public Texture
	{
	public:
		GLESTexture(const char* pszPath);

		GLESTexture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);

		~GLESTexture();

		virtual bool		CreateRT();

		virtual bool		Load(const char* pszPath = NULL, bool generateMipmaps = false);

		virtual bool		Load(DataStream* pDataStream, bool generateMipmaps = false);

		virtual bool		LoadFromData(Uint8* data,UINT size, bool generateMipmaps = false);

		GLuint				GetTexture() {return m_pTex;}

		void				SetFrameBuffer(FrameBufferHandle handele) {m_handle = handele;}

		FrameBufferHandle	GetFrameBuffer() {return m_handle;}

		
	private:
		void				GenerateMipmaps();

		void				ConvertImageData(int pixelFormat,int nPixelCount,Uint8* pPixel);
		
	private:
		GLuint				m_pTex;			// 贴图地址
		GLuint				m_PixelFormat;	// 内部像素格式

		FrameBufferHandle	m_handle;
	};

}



#endif