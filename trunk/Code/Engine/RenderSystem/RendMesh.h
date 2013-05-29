#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

//#include "Engine/RenderSystem/IRenderItem.h"
#include "Renderer.h"

namespace ma
{
	class MeshData;

	class ENGINE_API RenderMesh 
	{
	public:
		bool InitWithData(MeshData* pMeshData);
		

		void Draw(Technique* pTechnique);

	public:
		
 		MeshData*				m_pMesData;
 
		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
		VertexBuffer*			m_pVertexBuffers;
		IndexBuffer*			m_pIndexBuffer;

		//std::vector<Renderable*>	m_arrRenderable;

	};
}

#endif
