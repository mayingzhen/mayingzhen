#ifndef  _DXRENDMESH__H__
#define  _DXRENDMESH__H__

class MeshData;

namespace ma
{
	class DXRENDER_API DxRendMesh : public IRendMesh
	{
	public:
		ID3DXMesh* GetD3DXMesh() {return m_ppD3DMesh[0];}
		
		MeshData* GetMeshData() {return m_pMeshData;}

		virtual	bool InitWithData(MeshData* pMeshData);

		virtual	void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax);

	private:
		ID3DXMesh**	m_ppD3DMesh;
		MeshData* m_pMeshData;
	};
}


#endif
