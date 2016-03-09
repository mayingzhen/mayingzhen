#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__



namespace ma
{
	class ImageData;	

	class Texture : public Resource
	{
		
	public:
		Texture(); 

		Texture(int nWidth,int nHeight,PixelFormat eFormat,TEXTURE_USAGE eUsage); // For Render Texture
		
		virtual ~Texture();

		int				GetWidth() const {return m_nWidth;}

		int				GetHeight() const {return m_nHeight;}

		PixelFormat		GetFormat() const {return m_eFormat;}

		TEXTURE_USAGE	GetUsage() const {return m_eUsage;}

		Wrap			GetWrapMode() const {return m_eWrap;}

		Filter			GetFilterMode() const {return m_eFilter;} 

		bool			GetSRGB() const {return m_bSRGB;}

		void			SetSRGB(bool enable) {m_bSRGB = enable;}

		const char*		GetImagePath() const;

		void			Load(const char* pszPath,Wrap eWrap,Filter eFilter,bool bSRGB);

		virtual	bool	IsReady();

		static bool		BuildImageData(const char* pszFile, void* pMemory, uint32 nNumBytes, OUT ImageData& imageData);

		static RefPtr<Texture>	Improt(rapidxml::xml_node<>* pXmlElem);
		static void		Export(Texture* pTexture,rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	
	
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
		TEXTURE_USAGE	m_eUsage;
		PixelFormat		m_eFormat;
		TEXTURE_TYPE	m_eType;

		Wrap			m_eWrap;
		Filter			m_eFilter;	
		RefPtr<Resource> m_pImageRes;
		std::string		m_strImagePath;

		bool			m_bSRGB;

		friend class	RenderThread;
	};

	RefPtr<Texture> CreateTexture(const char* pImagePath,Wrap eWrap = REPEAT, Filter eFilter = TFO_TRILINEAR,bool bSRGB = true);
	RefPtr<Texture> CreateTexture();
}

#endif
