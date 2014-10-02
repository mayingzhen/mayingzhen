#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__


namespace ma
{
	class RENDER_API Texture : public Resource
	{
		DECL_OBJECT(Texture)

	public:
		Texture(const char* pszPath = NULL);

		Texture(int nWidth,int nHeight,FORMAT eFormat,bool bDepthStencil); // RenderTarget
			
		virtual void	LoadSync();

		virtual bool	CreateRT() = 0;

		virtual bool	Load(MemoryStream* pDataStream, bool generateMipmaps = false) = 0;

		virtual bool	CreateFromMemeory();

		int				GetWidth() {return m_nWidth;}

		int				GetHeight() {return m_nHeight;}

		FORMAT			GetFormat() {return m_eFormat;}

	protected:
		int				m_nWidth;
		int				m_nHeight;
		int				m_nMipLevels;
		USAGE			m_eUsage;
		FORMAT			m_eFormat;
		TEXTURE_TYPE	m_eType;
	};

	DeclareRefPtr(Texture);
}

#endif
