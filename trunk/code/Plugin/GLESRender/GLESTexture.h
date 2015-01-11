#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESTexture : public Texture
	{
	public:
		GLESTexture(const char* pszPath);

		GLESTexture(int nWidth,int nHeight,PixelFormat format = PF_A8R8G8B8,USAGE eUsage = USAGE_STATIC);

		~GLESTexture();

		GLuint				GetTexture() {return m_pTex;}

		void				SetFrameBuffer(FrameBufferHandle handele) {m_handle = handele;}

		FrameBufferHandle	GetFrameBuffer() {return m_handle;}

	private:
		virtual	bool		RT_Create();			

		virtual	bool		SetLevelData(int level, const PixelBox& src);

		virtual bool		GenerateMipmaps();

		PixelFormat			getNativeFormat(PixelFormat format);

		//void				ConvertImageData(int pixelFormat,int nPixelCount,uint8* pPixel);
		
	private:
		GLuint				m_pTex;			// 贴图地址
		GLuint				m_PixelFormat;	// 内部像素格式
		GLuint				m_DataType;

		FrameBufferHandle	m_handle;
	};

}



#endif