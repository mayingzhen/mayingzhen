#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__



namespace ma
{
	class ImageData;	

	class Texture : public Resource
	{
		
	public:
		Texture(); 

		Texture(int nWidth,int nHeight,PixelFormat eFormat,USAGE eUsage); // For Render Texture
		
		virtual ~Texture();

		DECL_OBJECT(Texture)

		static void		RegisterAttribute();

		int				GetWidth() const {return m_nWidth;}

		int				GetHeight() const {return m_nHeight;}

		PixelFormat		GetFormat() const {return m_eFormat;}

		USAGE			GetUsage() const {return m_eUsage;}

		void			SetWrapMode(Wrap eWrap) {m_eWrap = eWrap;}
		Wrap			GetWrapMode() const {return m_eWrap;}

		void			SetFilterMode(FilterOptions ficationFilter) {m_eFilter = ficationFilter;}
		FilterOptions	GetFilterMode() const {return m_eFilter;} 

		void			SetImagePath(const char* pTexPath);
		const char*		GetImagePath() const;

		virtual	bool	IsReady();

		static bool		BuildImageData(const char* pszFile, void* pMemory, uint32 nNumBytes, OUT ImageData& imageData);
	
	protected:

		bool			LoadFromImagData(const ImageData& imageData);
		
		bool			RT_StreamComplete();  

		virtual	bool	RT_CreateTexture(bool bMinMap) = 0;	

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

		Wrap			m_eWrap;
		FilterOptions	m_eFilter;	
		RefPtr<Resource> m_pImageRes;
		std::string		m_strImagePath;

		friend class	RenderThread;
	};

	RefPtr<Texture> CreateSamplerState(const char* pImagePath,Wrap eWrap = REPEAT, FilterOptions eFilter = TFO_TRILINEAR);
	RefPtr<Texture> CreateSamplerState();
}

#endif
