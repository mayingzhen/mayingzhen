#include "ScreenQuad.h"

namespace ma
{
	static Renderable* gpScrenQuad;

	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
	};

	void ScreenQuad::Init()
	{
		gpScrenQuad = new Renderable();

		VertexElement element[2];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		element[1] = VertexElement(0,12,DT_FLOAT2,DU_TEXCOORD,0);
		RefPtr<VertexDeclaration> pVertexDec = GetRenderSystem()->CreateVertexDeclaration(element,2);

		gpScrenQuad->m_pDeclaration = pVertexDec;
		gpScrenQuad->m_ePrimitiveType = PRIM_TRIANGLESTRIP;

		// Construct full screen quad
		Rectangle rect = GetRenderSystem()->GetViewPort();

		Reset((uint32)rect.width(),(uint32)rect.height());
	}

	void ScreenQuad::Reset(uint32 nWidth,uint32 nHeight)
	{
		// Construct full screen quad
		Vector2 offset = Vector2(0.5f / (float)nWidth, 0.5f / (float)nHeight);

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

		quadVerts[0].texCoords = Vector2(1.0f, 0.0f) + offset;
		quadVerts[1].texCoords = Vector2(0.0f, 0.0f) + offset;
		quadVerts[2].texCoords = Vector2(1.0f, 1.0f) + offset;
		quadVerts[3].texCoords = Vector2(0.0f, 1.0f) + offset;

		uint16* indices = new uint16[4];
		indices[0]= 0;
		indices[1]= 1;
		indices[2]= 2;
		indices[3]= 3;

		RefPtr<VertexBuffer> pVertexs = GetRenderSystem()->CreateVertexBuffer((uint8*)quadVerts,sizeof(Vertex) * 4,sizeof(Vertex));

		RefPtr<IndexBuffer> pIndexs = GetRenderSystem()->CreateIndexBuffer((uint8*)indices,sizeof(uint16) * 4,sizeof(uint16));

		gpScrenQuad->m_pIndexBuffer = pIndexs;
		gpScrenQuad->m_pVertexBuffer = pVertexs;

		SAFE_DELETE_ARRAY(quadVerts);
		SAFE_DELETE_ARRAY(indices);
	}

	void ScreenQuad::Shoutdown()
	{
		SAFE_DELETE(gpScrenQuad);
	}

	void ScreenQuad::Render(Technique* pTechnique)
	{
		gpScrenQuad->Render(pTechnique);
	}
}


