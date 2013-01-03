#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__


namespace ma
{
	struct MeshData;

	class COMMON_API IRendMesh 
	{
	public:
		virtual bool InitWithData(MeshData* pMeshData) = 0;
	};
}

#endif
