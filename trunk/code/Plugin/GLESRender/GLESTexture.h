#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESRENDER_API GLESTexture : public Texture
	{
	public:
		GLESTexture();

		~GLESTexture();

		virtual bool Create(int nWidth,int nHeight,int nMipLevel = 1,FORMAT Format = FMT_X8R8G8B8,USAGE Usage = USAGE_STATIC);

		virtual bool Load(const char* pszPath = NULL, bool generateMipmaps = false);

		//void	Bind();

		GLuint GetTexture() {return m_pTex;}

		void generateMipmaps();
		
	private:
		GLuint		m_pTex;		// ��ͼ��ַ
		GLuint	    m_PixelFormat;// �ڲ����ظ�ʽ
		GLint		m_uvAddressMode;
		bool        m_bAddBorderPvr;//pvrͼ����ӱ߿�ģʽ
		int			m_nMemSize;
		bool		m_bAutoGenerateMipmap;

		bool		_mipmapped;
	};

}



#endif