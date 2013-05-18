#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Engine/RenderSystem/IRenderItem.h"

namespace ma
{
	class MeshData;

	class VertexStream;

	class ENGINE_API RenderMesh : public IRenderItem
	{
	public:
		bool InitWithData(MeshData* pMeshData);
		

		//void Draw();

	public:
		
		MeshData*				m_pMesData;

		//VertexStream*			m_pVertexStream;

		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
		VertexBuffer*			m_pVertexBuffers;
		IndexBuffer*			m_pIndexBuffer;

	};
}

#endif
