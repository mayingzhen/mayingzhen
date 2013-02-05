#ifndef  _Texture__H__
#define  _Texture__H__

#include "Framework/Resource.h"

namespace ma
{
	class IRendTexture;

	class FRAMEWORK_API Texture : public Resource
	{
	public:
		Texture(const char* pTexturePath);

		bool Load();

		IRendTexture* GetRendTexture() {return m_pRendTexure;}

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "TextureRes");

	private:
		IRendTexture* m_pRendTexure;
	};
}


#endif