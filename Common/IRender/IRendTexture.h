#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

#include "Common/Resource.h"

namespace ma
{
	class COMMON_API IRendTexture : public Resource
	{
	public:
		IRendTexture(const char* resPath):Resource(resPath)
		{

		}

		virtual bool Load() = 0;
	};
}

#endif