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
		GetRenderDevice()->GetRenderWndSize(width,Heigh);
		Vector2 offset = Vector2(0.5f / width, 0.5f / Heigh);

		//   1    0 
		//   +----+
		//   | \  |  
		//   |  \ |
		//   +----+
		//   3    2
		Vertex	quadVerts[4];
		quadVerts[0].position = Vector3(1, 1, 0);
		quadVerts[1].position = Vector3(-1, +1, 0);
		quadVerts[2].position = Vector3(1, -1, 0);
		quadVerts[3].position = Vector3(-1, -1, 0);

		quadVerts[0].texCoords = Vector2(0.0f, 1.0f) + offset;
 		quadVerts[1].texCoords = Vector2(0.0f, 0.0f) + offset;
 		quadVerts[2].texCoords = Vector2(1.0f, 1.0f) + offset;
 		quadVerts[3].texCoords = Vector2(0.0f, 1.0f) + offset;

		unsigned short indices[4] = { 0, 2, 1, 3 };

		// Init Renderable
		_ScrenQuad = new Renderable();

		VertexDeclaration* pVertexDec = GetRenderDevice()->CreateVertexDeclaration(); 
		pVertexDec->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		pVertexDec->AddElement(0,12,DT_FLOAT2,DU_TEXCOORD0,0);
		
		VertexBuffer* pVertexs = GetRenderDevice()->CreateVertexBuffer(quadVerts,sizeof(quadVerts),sizeof(Vertex));
		IndexBuffer* pIndexs = GetRenderDevice()->CreateIndexBuffer(indices,sizeof(indices));

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

	void ScreenQuad::Render(Material* pMaterial)
	{
		_ScrenQuad->m_pMaterial = pMaterial;

// 		Matrix4x4 mInvProj;
// 		MatrixInverse(&mInvProj, NULL, &m_matProj);
// 		pMaterial->GetParameter("u_InvProjMatrix")->setMatrix(mInvProj);

		GetRenderDevice()->DrawRenderable(_ScrenQuad);
	}
}


