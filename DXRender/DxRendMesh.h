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

	private:
		ID3DXMesh**	m_ppD3DMesh;
		IS3AMeshData* m_pMeshData;
	};
}


#endif
