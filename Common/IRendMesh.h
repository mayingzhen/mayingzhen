#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

namespace ma
{
	class COMMON_API IRendMesh
	{
	public:
		virtual bool Load(const char* resPath) = 0;
	};
}

#endif
