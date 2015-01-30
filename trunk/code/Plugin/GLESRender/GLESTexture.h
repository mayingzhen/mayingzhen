#ifndef  _GLESRendTexture__H__
#define  _GLESRendTexture__H__



namespace ma
{
	class GLESTexture : public Texture
	{
	public:
		GLESTexture();

		GLESTexture(int nWidth,int nHeight,PixelFormat format = PF_A8R8G8B8,USAGE eUsage = USAGE_STATIC);

		~GLESTexture();

		GLuint				GetTexture() {return m_pTex;}
		void				SetTexture(GLuint hTex) {m_pTex = hTex;}

		void				SetFrameBuffer(FrameBufferHandle handele) {m_hFB = handele;}
		FrameBufferHandle	GetFrameBuffer() {return m_hFB;}

		void				SetTextureWrap(Wrap eWrap);
		Wrap				GetTextureWrap() const {return m_eWrap;}

 		void				SetTextureFilter(FilterOptions eFilter);
		FilterOptions		GetTextureFilter() const {return m_eFilter;}

	private:
		virtual	bool		RT_CreateTexture(bool bMinMap);	

		virtual	bool		RT_CreateDepthStencil();	

		virtual	bool		SetLevelData(int level, const PixelBox& src);

		virtual bool		GenerateMipmaps();

		PixelFormat			GetNativeFormat(PixelFormat format);
		
	private:
		GLuint				m_pTex;			// ��ͼ��ַ
		GLuint				m_PixelFormat;	// �ڲ����ظ�ʽ
		GLuint				m_DataType;

		FrameBufferHandle	m_hFB;
	};

}



#endif