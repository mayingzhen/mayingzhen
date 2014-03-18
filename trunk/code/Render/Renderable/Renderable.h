#ifndef  _Rendererable__H__
#define  _Rendererable__H__


namespace ma
{
	class Material;
	class Technique;
	class SubMeshData;

	class Renderable : public Referenced
	{
	public:
		Renderable();

		virtual void					Render(Technique* pTech);

		void							SetWorldMatrix(const Matrix4x4& matWS);

		void							SetSkinMatrix(const Matrix4x4* arrMatrixs, Uint nCount);

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		ref_ptr<VertexDeclaration>		m_pDeclaration;
		ref_ptr<VertexBuffer>			m_pVertexBuffers;	
		ref_ptr<IndexBuffer>			m_pIndexBuffer;
// 		UINT							m_nIndexStart;
// 		UINT							m_nIndexCount;
// 		UINT							m_nVertexStart;
// 		UINT							m_nVertexCount;
		ref_ptr<SubMeshData>			m_pSubMeshData;
		ref_ptr<Material>				m_pMaterial;
		Matrix4x4						m_matWorld[2];
		std::vector<Matrix4x4>			m_arrSkinMatrix[2];
	};

	DeclareRefPtr(Renderable);


// 	class SkinRenderable : public Renderable
// 	{
// 	public:
// 		void					SetSkinMatrix(const Matrix4x4* arrMatrixs, Uint nCount);
// 		
// 	public:
// 
// 		std::vector<BoneIndex>	m_arrBonePalette;
// 
// 		std::vector<Matrix4x4>	m_arrSkinMatrix[2];
// 	};
// 
// 	DeclareRefPtr(SkinRenderable);
}

#endif

