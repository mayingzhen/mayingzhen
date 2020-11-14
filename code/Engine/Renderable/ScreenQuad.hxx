#include "ScreenQuad.h"

namespace ma
{
	static Renderable* gpScrenQuad;

	struct Vertex
	{
		Vector2 position;
		Vector2 texCoords;
	};

	void ScreenQuad::Init()
	{
		gpScrenQuad = new Renderable();

		gpScrenQuad->m_ePrimitiveType = PRIM_TRIANGLELIST;

		/*
			0   2	
		    1   3
		*/
		Vertex quadVerts[4];
		quadVerts[0].position = Vector2(-1, 1);
		quadVerts[1].position = Vector2(-1, -1);
		quadVerts[2].position = Vector2(1, 1);
		quadVerts[3].position = Vector2(1, -1);

		quadVerts[0].texCoords = Vector2(0.0f, 0.0f);
		quadVerts[1].texCoords = Vector2(0.0f, 1.0f);
		quadVerts[2].texCoords = Vector2(1.0f, 0.0f);
		quadVerts[3].texCoords = Vector2(1.0f, 1.0f);

// 		RenderDeviceType type = GetRenderDevice()->GetRenderDeviceType();
// 		if (type == RenderDevice_VULKAN)
// 		{
// 			quadVerts[0].texCoords = Vector2(0.0f, 1.0f);
// 			quadVerts[1].texCoords = Vector2(0.0f, 0.0f);
// 			quadVerts[2].texCoords = Vector2(1.0f, 1.0f);
// 			quadVerts[3].texCoords = Vector2(1.0f, 0.0f);
// 		}

		uint16_t indices[6];
		indices[0] = 0;
		indices[1] = 2;
		indices[2] = 1;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;

		RefPtr<VertexBuffer> pVertexs = GetRenderSystem()->CreateVertexBuffer((uint8_t*)quadVerts,sizeof(Vertex) * 4,sizeof(Vertex));

		RefPtr<IndexBuffer> pIndexs = GetRenderSystem()->CreateIndexBuffer((uint8_t*)indices,sizeof(uint16_t) * 6,sizeof(uint16_t));

		gpScrenQuad->m_pIndexBuffer = pIndexs;
		gpScrenQuad->m_pVertexBuffer = pVertexs;
	}

	void ScreenQuad::Shoutdown()
	{
		SAFE_DELETE(gpScrenQuad);
	}

	void ScreenQuad::Render(Technique* pTechnique, RenderCommand* pCommand)
	{
		gpScrenQuad->PreRender(pTechnique);

		gpScrenQuad->Render(pTechnique, pCommand);
	}

	Renderable* ScreenQuad::GetRenderable()
	{
		return gpScrenQuad;
	}
}


