#ifndef  _Rendererable__H__
#define  _Rendererable__H__


namespace ma
{
	class Material;
	struct SubMeshData;

	struct Renderable
	{
	public:
		Renderable()
		{
			m_ePrimitiveType = PRIM_TRIANGLESTRIP;
			m_pDeclaration = NULL;
			m_pVertexBuffers = NULL;
			m_pIndexBuffer = NULL;
			m_pSubMeshData = NULL;

			MatrixIdentity(&m_matWorld[0]);
			MatrixIdentity(&m_matWorld[1]);
		}
		
		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
		VertexBuffer*			m_pVertexBuffers;
		IndexBuffer*			m_pIndexBuffer;
		SubMeshData*			m_pSubMeshData;	

		std::vector<Matrix4x4>  m_arrSkinMatrix[2];

		Matrix4x4				m_matWorld[2];
	};
}

#endif

