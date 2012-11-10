#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__

namespace ma
{
	class COMMON_API IRendTexture
	{
	public:
		virtual bool Load(const char* resPath) = 0;
	};
}

#endif