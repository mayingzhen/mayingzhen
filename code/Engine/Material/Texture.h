#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__



namespace ma
{
	class ImageData;	

	class Texture : public Resource
	{
		
	public:
		Texture(); 

		Texture(int nWidth,int nHeight,PixelFormat eFormat,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage);
		
		virtual ~Texture();

		int				GetWidth() const {return m_nWidth;}

		int				GetHeight() const {return m_nHeight;}

		PixelFormat		GetFormat() const {return m_eFormat;}

		TEXTURE_USAGE	GetUsage() const {return m_eUsage;}

		virtual bool    InitRes();

		void			SetMipMap(bool b) {m_bMipMap = b;}
		bool			GetMipMap() const {return m_bMipMap;}
		
		void			SetSRGB(bool b) {m_bSRGB = b;}
		bool			GetSRGB() const {return m_bSRGB;}

		static bool		BuildImageData(const char* pszFile, void* pMemory, uint32 nNumBytes, OUT ImageData& imageData);
	
	protected:

		bool			LoadFromImagData(const ImageData& imageData);
		
		bool			RT_StreamComplete();  

		virtual	bool	RT_CreateTexture() = 0;	
		
		virtual bool	RT_CreateRenderTarget() = 0;

		virtual	bool	RT_CreateDepthStencil() = 0;	

		virtual	bool	SetLevelData(int level, const PixelBox& src) = 0;

		virtual bool	GenerateMipmaps() = 0;

	protected:
		int				m_nWidth;
		int				m_nHeight;
		UINT			m_nMipLevels;
		TEXTURE_USAGE	m_eUsage;
		PixelFormat		m_eFormat;
		TEXTURE_TYPE	m_eType;

		bool			m_bMipMap;
		bool			m_bSRGB;
		bool			m_bTypeLess;

		friend class	RenderThread;
		friend class	TextureManager;
	};

	RefPtr<Texture> CreateTexture(const char* pImagePath,bool bMipMap = true,bool bSRGB = true);
	RefPtr<Texture> CreateTexture();

	class SamplerState : public Serializable
	{
	public:
		SamplerState();

		DECL_OBJECT(SamplerState);

		static void		RegisterAttribute();

		void			SetTexture(Texture* pTexutre) {m_pTexture = pTexutre;}
		Texture*		GetTexture() const {return m_pTexture.get();}
		void			SetTexturePath(const char* pszPath);
		const char*		GetTexturePath() const;

		void			SetWrapMode(Wrap eWrap) {m_eWrap = eWrap;}
		Wrap			GetWrapMode() const {return m_eWrap;}

		void			SetWrapModeW(Wrap eWrapW) {m_eWrapW = eWrapW;}
		Wrap			GetWrapModeW() const {return m_eWrapW;}

		void			SetFilterMode(Filter eFilter) {m_eFilter = eFilter;}
		Filter			GetFilterMode() const {return m_eFilter;} 

		bool			GetSRGB() const {return m_bSRGB;}
		void			SetSRGB(bool enable) {m_bSRGB = enable;}

		float			GetLodBias() const {return m_fLodBias;}
		void			SetLodBias(float fBias) {m_fLodBias = fBias;}

		bool			operator < ( const SamplerState& other) const;

	private:
		RefPtr<Texture> m_pTexture;

		Wrap			m_eWrap;
		Wrap			m_eWrapW;
		Filter			m_eFilter;	
		bool			m_bSRGB;
		float			m_fLodBias;
	};

	RefPtr<SamplerState> CreateSamplerState(const char* pPath,Wrap eWrap = REPEAT, Filter eFilter = TFO_TRILINEAR,bool bSRGB = true);
	RefPtr<SamplerState> CreateSamplerState(Texture* pTexutre,Wrap eWrap = REPEAT, Filter eFilter = TFO_TRILINEAR,bool bSRGB = true);
}

#endif
