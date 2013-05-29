#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

#include "Common/Resource.h"

namespace ma
{
	class ENGINE_API Texture : public Resource
	{
	public:
		//static Texture* create(const char* path,bool generateMipmaps = false);

		virtual bool Create(int nWidth,int nHeight,int nMipLevel,FORMAT Format = FMT_X8R8G8B8,USAGE Usage = USAGE_STATIC) = 0;

		virtual bool Load(const char* pszPath = NULL, bool generateMipmaps = false) = 0;

		int getWidth() {return mWidth;}

		int getHeight() {return mHeight;}

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
