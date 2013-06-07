#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

//#include "Engine/RenderSystem/IRenderItem.h"
//#include "Renderable.h"

namespace ma
{
	class MeshData;
	class Material;

	class ENGINE_API RenderMesh 
	{
	public:
		bool InitWithData(MeshData* pMeshData);
		

		void Draw();

		//void SetTexture(const Texture* pTexture);

		void SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		//void SetMatrix(const Matrix4x4& matWorld, const Matrix4x4& matView, const Matrix4x4& matProj);

		void SetMaterial(Material* pMaterial);

	public:
		
 		MeshData*				m_pMesData;
 
// 		PRIMITIVE_TYPE			m_ePrimitiveType;
// 		VertexDeclaration*		m_pDeclaration;
// 		VertexBuffer*			m_pVertexBuffers;
// 		IndexBuffer*			m_pIndexBuffer;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

	};
}

#endif
