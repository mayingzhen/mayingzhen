#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__



namespace ma
{
	class ImageData;	

	class Texture : public Resource
	{
		DECL_OBJECT(Texture)

	public:
		Texture(const char* pszPath = NULL);

		Texture(int nWidth,int nHeight,PixelFormat eFormat,USAGE eUsage);
			
		virtual void	LoadSync();

		int				GetWidth() const {return m_nWidth;}

		int				GetHeight() const {return m_nHeight;}

		PixelFormat		GetFormat() const {return m_eFormat;}

		void			SetAutoMipMap(bool b) {m_bAutoMipMap = b;}

		static bool		BuildImageData(const char* pszFile, void* pMemory, uint32 nNumBytes, OUT ImageData& imageData);
	
	protected:
		bool			CreateFromMemeory();

		bool			RT_StreamComplete(); 

		bool			LoadFromImagData(const ImageData& imageData);

		virtual	bool	RT_CreateTexture() = 0;	

		virtual	bool	RT_CreateDepthStencil() = 0;	

		virtual	bool	SetLevelData(int level, const PixelBox& src) = 0;

		virtual bool	GenerateMipmaps() = 0;

	protected:
		int				m_nWidth;
		int				m_nHeight;
		UINT			m_nMipLevels;
		USAGE			m_eUsage;
		PixelFormat		m_eFormat;
		TEXTURE_TYPE	m_eType;

		bool			m_bAutoMipMap;

		friend class	RenderThread;
	};

	DeclareRefPtr(Texture);

	RefPtr<Texture> CreateTexture(const char* pPath,bool bMipmap = true);
}

#endif
