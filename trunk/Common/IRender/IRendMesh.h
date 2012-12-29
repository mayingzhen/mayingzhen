#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

//class MeshData;

namespace ma
{
	class COMMON_API IRendMesh /*: public IRendItem*/
	{
	public:
		virtual bool Load(const char* resPath) = 0;

	//private:
	//	MeshData* m_pMeshData;
	};
}

#endif
