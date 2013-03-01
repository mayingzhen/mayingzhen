#include "Common/RenderSystem/Texture.h"

namespace ma
{
    Texture::Texture()
        : mName("")
        , mHeight(0)
        , mWidth(0)
        , mDepth(0)
        , mMipLevels(0)
        , mUsage(USAGE_STATIC)
        , mFormat(FMT_UNKNOWN)
        , mType(TEXTYPE_2D)

    {
    }

    Texture::~Texture()
    {
    }

	const char* Texture::GetName() const
	{ 
		return mName.c_str();
	}

	int Texture::GetWidth() const
	{ 
		if (mWidth != -1)
			return mWidth;
		else
			return 0;//Engine::Instance()->GetDeviceProperty()->Width;
	}

	int Texture::GetHeight() const
	{
		if (mHeight != -1)
			return mHeight;
		else
			return 0;//Engine::Instance()->GetDeviceProperty()->Height;
	}

	int Texture::GetDepth() const
	{ 
		return mDepth;
	}

	int Texture::GetMipLevels() const
	{ 
		return mMipLevels;
	}

	USAGE Texture::GetUsage() const
	{ 
		return mUsage;
	}

	FORMAT Texture::GetFormat() const
	{
		return mFormat;
	}

	TEXTURE_TYPE Texture::GetTextureType() const
	{ 
		return mType;
	}
}
