#ifndef  _TextureData__H__
#define  _TextureData__H__


namespace ma
{
	class Texture;

	class ENGINE_API TextureData : public Resource
	{
	public:
		virtual bool Load(const char* pszPath = NULL);

		virtual bool Save(const char* pszPath = NULL);

		Texture*	GetRenderTexture() {return m_pTexture;}

	private:
		Texture* m_pTexture;
	};
}

#endif
