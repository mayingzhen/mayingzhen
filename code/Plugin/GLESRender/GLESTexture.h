#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESTexture : public Texture
	{
	public:
		GLESTexture();

		GLESTexture(int nWidth,int nHeight,UINT nMipMap,PixelFormat eFormat,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType);

		~GLESTexture();

		GLuint				GetTexture() {return m_pTex;}
		void				SetTexture(GLuint hTex) {m_pTex = hTex;}

		void				SetFrameBuffer(FrameBufferHandle handele) {m_hFB = handele;}
		FrameBufferHandle	GetFrameBuffer() {return m_hFB;}
		
		virtual void		CopyTo(Texture* pDesc,int nFace,int level) {}

	private:
		virtual	bool		RT_CreateCubeTexture();	

		virtual	bool		RT_CreateTexture();	

		virtual	bool		SetLevelData(int level, int face, const PixelBox& src);

		virtual bool		GenerateMipmaps();

		PixelFormat			GetNativeFormat(PixelFormat format);
		
	private:
		GLuint				m_pTex;			// 贴图地址
		GLuint				m_PixelFormat;	// 内部像素格式
		GLuint				m_DataType;

		FrameBufferHandle	m_hFB;
	};

}



#endif