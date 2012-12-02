#ifndef  _DXRENDMESH__H__
#define  _DXRENDMESH__H__

namespace ma
{
	class DXRENDER_API DxRendMesh : public IRendMesh
	{
	public:
		DxRendMesh();


		~DxRendMesh();

		bool Load(const char* pszPath);

		ID3DXMesh* GetD3DXMesh() {return m_ppD3DMesh[0];}

	private:
		ID3DXMesh**	m_ppD3DMesh;
		IMeshData* m_pMeshData;
	};
}


#endif
