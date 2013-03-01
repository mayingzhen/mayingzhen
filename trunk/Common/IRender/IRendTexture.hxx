#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

#include "Common/Resource.h"

namespace ma
{
	class COMMON_API IRendTexture : public Resource
	{
	public:
		virtual bool Load(const char* resPath) = 0;
	};
}

#endif