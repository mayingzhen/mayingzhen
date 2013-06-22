#ifndef  _Rendererable__H__
#define  _Rendererable__H__


namespace ma
{
	class Material;

	struct ENGINE_API Renderable
	{
	public:
		Renderable()
		{
			m_ePrimitiveType = PRIM_TRIANGLESTRIP;
			m_pDeclaration = NULL;
			m_pVertexBuffers = NULL;
			m_pIndexBuffer = NULL;
			m_nVertexStart = 0;
			m_nVertexCount = 0;
			m_nIndexStart = 0;
			m_nIndexCount = 0;

			m_pMaterial = NULL;

			MatrixIdentity(&m_matWorld);
		}
		
		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
		VertexBuffer*			m_pVertexBuffers;
		IndexBuffer*			m_pIndexBuffer;
		UINT					m_nVertexStart;
		UINT					m_nVertexCount;
		UINT					m_nIndexStart;
		UINT					m_nIndexCount;

		std::vector<BoneIndex>	m_arrBonePalette;
		std::vector<Matrix4x4>  m_arrSkinMatrix;

		Material*               m_pMaterial;

		Matrix4x4				m_matWorld;
	};
}

#endif

