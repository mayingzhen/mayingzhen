#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

#include "Common/Resource.h"

namespace ma
{
	class ENGINE_API Texture : public Resource
	{
	public:
		virtual bool Create(int nWidth,int nHeight,int nMipLevel,FORMAT Format = FMT_X8R8G8B8,USAGE Usage = USAGE_STATIC) = 0;

		virtual bool Load(const char* pszPath = NULL) = 0;

	protected:
		int				mWidth;
		int				mHeight;
		int				mDepth;
		int				mMipLevels;
		USAGE			mUsage;
		FORMAT			mFormat;
		TEXTURE_TYPE	mType;
	};

}

#endif
