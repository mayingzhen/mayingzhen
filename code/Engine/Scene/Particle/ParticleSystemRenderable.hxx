#include "ParticleSystemRenderable.h"
#include "ParticleSystem.h"

namespace ma
{

	ParticleSystemRenderable::ParticleSystemRenderable()
	{
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

	bool ParticleSystemRenderable::AllocVertices( int nAllocVerts, int nAllocInds )
	{
		ParallHardWareBuffer* pBuffer = GetRenderSystem()->GetParticleBuffer();
		ASSERT(pBuffer);
		if (pBuffer == NULL)
			return false;

		m_subAllocVB = pBuffer->AllocVertexBuffer(nAllocVerts);
		m_subAllocIB = pBuffer->AllocIndexBuffer(nAllocInds);

		m_pSubMeshData->m_nIndexStart = m_subAllocIB.m_nFirstIndex;
		m_pSubMeshData->m_nIndexCount = m_subAllocIB.m_nAllocInds;

		ASSERT(m_subAllocVB.m_nAllocVerts == nAllocVerts && m_subAllocIB.m_nAllocInds == nAllocInds);
		if (m_subAllocVB.m_nAllocVerts != nAllocVerts || m_subAllocIB.m_nAllocInds != nAllocInds)
			return false;

		return true;
	}

	void ParticleSystemRenderable::Render(Technique* pTech)
	{
		if (m_pSubMeshData->m_nIndexCount == 0)
		{
			return;
		}

		ParallHardWareBuffer* pBuffer = GetRenderSystem()->GetRTParticleBuffer();
		ASSERT(pBuffer);
		if (pBuffer == NULL)
			return;

		m_pVertexBuffer = pBuffer->GetVertexBuffer();
		m_pIndexBuffer = pBuffer->GetIndexBuffer();

		m_pSubMeshData->m_nVertexStart = 0;
		m_pSubMeshData->m_nVertexCount = m_pVertexBuffer->GetNumber();

		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		GetRenderSystem()->DrawRenderable(this,pTech);
	}


}