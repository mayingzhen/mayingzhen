#include "ScreenQuad.h"

namespace ma
{
	static Renderable* _ScrenQuad;

	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
	};

	void ScreenQuad::Init()
	{

		// Construct full screen quad
		int width, Heigh;
		Platform::GetInstance().GetWindowSize(width,Heigh);
		Vector2 offset = Vector2(0.5f / width, 0.5f / Heigh);

		//   1    0 
		//   +----+
		//   | \  |  
		//   |  \ |
		//   +----+
		//   3    2
		Vertex*	quadVerts = new Vertex[4];
		quadVerts[0].position = Vector3(1, 1, 0);
		quadVerts[1].position = Vector3(-1, +1, 0);
		quadVerts[2].position = Vector3(1, -1, 0);
		quadVerts[3].position = Vector3(-1, -1, 0);

		quadVerts[0].texCoords = Vector2(0.0f, 1.0f) + offset;
 		quadVerts[1].texCoords = Vector2(0.0f, 0.0f) + offset;
 		quadVerts[2].texCoords = Vector2(1.0f, 1.0f) + offset;
 		quadVerts[3].texCoords = Vector2(0.0f, 1.0f) + offset;

		unsigned short* indices = new unsigned short[4];
		indices[0]= 0;
		indices[1]= 2;
		indices[2]= 1;
		indices[3]= 3;

		// Init Renderable
		_ScrenQuad = new Renderable();

		VertexDeclaration* pVertexDec = GetRenderDevice()->CreateVertexDeclaration(); 
		pVertexDec->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		pVertexDec->AddElement(0,12,DT_FLOAT2,DU_TEXCOORD0,0);
		//pVertexDec->Active();
		
		VertexBuffer* pVertexs = GetRenderDevice()->CreateVertexBuffer(quadVerts,sizeof(Vertex) * 4,sizeof(Vertex));

		//pVertexs->Active();
		IndexBuffer* pIndexs = GetRenderDevice()->CreateIndexBuffer(indices,sizeof(unsigned short) * 4);
		//pIndexs->Active();

		_ScrenQuad->m_pSubMeshData = new SubMeshData();

		_ScrenQuad->m_pDeclaration = pVertexDec;
		_ScrenQuad->m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		_ScrenQuad->m_pIndexBuffer = pIndexs;
		_ScrenQuad->m_pVertexBuffers = pVertexs;
		_ScrenQuad->m_pSubMeshData->m_nVertexStart = 0;
		_ScrenQuad->m_pSubMeshData->m_nVertexCount = 4;
		_ScrenQuad->m_pSubMeshData->m_nIndexStart = 0;
		_ScrenQuad->m_pSubMeshData->m_nIndexCount = 4;
	}

	void ScreenQuad::ShoutDown()
	{
		SAFE_DELETE(_ScrenQuad);
	}

	void ScreenQuad::Render(Technique* pTechnique)
	{
		GetRenderSystem()->DrawRenderable(_ScrenQuad,pTechnique);
	}
}


