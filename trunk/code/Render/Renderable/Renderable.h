#ifndef  _Rendererable__H__
#define  _Rendererable__H__


namespace ma
{
	class Material;
	class Technique;
	class MeshData;
	class SubMeshData;

	class Renderable : public Referenced
	{
	public:
		Renderable();

		void							InitWithMeshData(const MeshData& meshData,int index);

		virtual void					Render(Technique* pTech);

		void							SetWorldMatrix(const Matrix4& matWS);

		void							SetSkinMatrix(const Matrix4* arrMatrixs, Uint nCount);

	public:
		PRIMITIVE_TYPE					m_ePrimitiveType;
		RefPtr<VertexDeclaration>		m_pDeclaration;
		RefPtr<VertexBuffer>			m_pVertexBuffers;	
		RefPtr<IndexBuffer>				m_pIndexBuffer;
		RefPtr<SubMeshData>				m_pSubMeshData;
		RefPtr<Material>				m_pMaterial;
		Matrix4							m_matWorld[2];
		std::vector<Matrix4>			m_arrSkinMatrix[2];
	};

	DeclareRefPtr(Renderable);

}

#endif

