#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__


namespace ma
{
	class ENGINE_API Texture : public Resource
	{
		DECL_OBJECT(Texture)

	public:
		Texture(const char* pszPath = NULL);

		Texture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8);
			
		virtual void LoadSync();

		virtual bool CreateRT() = 0;

		virtual bool Load(const char* pszPath = NULL, bool generateMipmaps = false) = 0;

		virtual bool Load(DataStream* pDataStream, bool generateMipmaps = false) = 0;

		virtual bool CreateFromMemeory();

		virtual bool LoadFromData(Uint8* data,UINT size, bool generateMipmaps = false) = 0;

		int getWidth() {return m_nWidth;}

		int getHeight() {return m_nHeight;}

	protected:
		int				m_nWidth;
		int				m_nHeight;
		int				m_nMipLevels;
		USAGE			m_eUsage;
		FORMAT			m_eFormat;
		TEXTURE_TYPE	m_eType;
	};
}

#endif
