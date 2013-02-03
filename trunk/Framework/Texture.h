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

	private:
		IRendTexture* m_pRendTexure;
	};
}


#endif