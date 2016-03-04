#include "ParticleSystemRenderable.h"
#include "ParticleSystem.h"

namespace ma
{

ParticleSystemRenderable::ParticleSystemRenderable(ParticleSystem* pParent)
{
	m_pParent = pParent;
	m_nNumIndices = 0;
	m_nNumVertices = 0;

	m_pSubMeshData = CreateSubMeshData();

	m_ePrimitiveType = PRIM_TRIANGLELIST;

	m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer();
	m_pVertexBuffer = GetRenderDevice()->CreateVertexBuffer();


	VertexElement element[5];
	element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
	element[1] = VertexElement(0,12,DT_FLOAT2,DU_TEXCOORD,0);
	element[2] = VertexElement(0,20,DT_FLOAT2,DU_TEXCOORD,1);
	element[3] = VertexElement(0,28,DT_COLOR,DU_BLENDWEIGHT,0);
	element[4] = VertexElement(0,32,DT_COLOR,DU_COLOR,1);
	m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,5);
}

ParticleSystemRenderable::~ParticleSystemRenderable(void)
{
}

void ParticleSystemRenderable::Render(Technique* pTech)
{
	if (m_nNumIndices == 0)
	{
		return;
	}

	ASSERT(m_nNumVertices <= vertices.size());
	ASSERT(m_nNumIndices <= indices.size());

	m_pIndexBuffer->SetData((uint8*)&indices[0],indices.size() * sizeof(UINT16),sizeof(UINT16),USAGE_STATIC,false);
	m_pVertexBuffer->SetData((uint8*)&vertices[0],vertices.size() * sizeof(VERTEX),sizeof(VERTEX),USAGE_STATIC,false);
	m_pSubMeshData->m_nIndexCount = m_nNumIndices;
	m_pSubMeshData->m_nVertexCount = m_nNumVertices;

	GetRenderContext()->SetCurRenderObj(this);

	pTech->Bind();

	GetRenderSystem()->DrawDyRenderable(this,pTech);
}


}