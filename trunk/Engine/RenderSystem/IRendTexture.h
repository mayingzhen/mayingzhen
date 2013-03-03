#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

#include "Common/Resource.h"

namespace ma
{
	class ENGINE_API IRendTexture : public Resource
	{
	public:
		virtual bool Load(const char* pszPath = NULL) = 0;
	};
}

#endif