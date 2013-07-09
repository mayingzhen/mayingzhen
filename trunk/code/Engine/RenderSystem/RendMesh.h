#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__


namespace ma
{
	class MeshData;
	class Material;

	class ENGINE_API RenderMesh 
	{
	public:
		RenderMesh();

		bool		InitWithData(MeshData* pMeshData);

		bool		Load(const char* pMeshPath,const char* pDiffueTexture);
		
		void		Draw();

		void		SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		void		SetWorldMatrix(const Matrix4x4& matWorld);

		void		SetMaterial(Material* pMaterial);

		Material*	GetMaterial() {return m_pMaterial;}

		UINT		GetRenderableNumber() {return m_arrRenderable.size();}

		Renderable*	GetRenderableByIndex(UINT index) {return m_arrRenderable[index];}

	public:
		
 		MeshData*				m_pMesData;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

		bool					m_bSkin;

	};
}

#endif
