#ifndef  _Texture__H__
#define  _Texture__H__

#include "Common/RenderSystem/RenderDefine.h"
#include "Common/Resource.h"


namespace ma
{
	class COMMON_API Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

		//virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags) = 0;
		//virtual void Unlock(int iLevel) = 0;

		const char* GetName() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetDepth() const;
		int GetMipLevels() const;
		USAGE GetUsage() const;
		FORMAT GetFormat() const;
		TEXTURE_TYPE GetTextureType() const;

	protected:
		std::string mName;
		int mWidth;
		int mHeight;
		int mDepth;
		int mMipLevels;
		USAGE mUsage;
		FORMAT mFormat;
		TEXTURE_TYPE mType;
	};

	//DeclareSmartPtr(Texture);

}

#endif
