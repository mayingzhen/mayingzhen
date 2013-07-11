#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESRENDER_API GLESTexture : public Texture
	{
	public:
		GLESTexture();

		~GLESTexture();

		virtual bool CreateRT(int nWidth,int nHeight,FORMAT Format = FMT_A8R8G8B8);

		virtual bool Load(const char* pszPath = NULL, bool generateMipmaps = false);

		virtual bool Load(DataStream* pDataStream, bool generateMipmaps = false);

		virtual bool LoadFromData(FORMAT format,UINT width,UINT height,Uint8* data,UINT size, bool generateMipmaps = false);

		GLuint GetTexture() {return m_pTex;}

		
	private:
		void GenerateMipmaps();

		void ConvertImageData(int pixelFormat,int nPixelCount,Uint8* pPixel);
		
	private:
		GLuint		m_pTex;			// 贴图地址
		GLuint	    m_PixelFormat;	// 内部像素格式
	};

}



#endif