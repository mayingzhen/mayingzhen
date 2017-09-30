#include "MetalRenderCommand.h"
#include "MetalIndexBuffer.h"
#include "MetalVertexBuffer.h"
#include "MetalMapping.h"
#include "MetalRenderPass.h"

namespace ma
{

	MetalRenderCommand::MetalRenderCommand()
	{

	}

	MetalRenderCommand::~MetalRenderCommand()
	{

	}


	void MetalRenderCommand::Begin()
	{
        ASSERT(m_pRenderPass);
        if (m_pRenderPass == NULL)
            return;
        
        m_encoder = [m_pRenderPass->m_parallelRCE renderCommandEncoder];
	}

	void MetalRenderCommand::End()
	{
        [m_encoder endEncoding];
	}

	void MetalRenderCommand::SetIndexBuffer(IndexBuffer* pIB)
	{

	}

	void MetalRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
        MetalVertexBuffer* pMetalVertexBuffer = (MetalVertexBuffer*)(pVB);
        id<MTLBuffer> vb = pMetalVertexBuffer->GetMetalVertexBuffer();
        
        [m_encoder setVertexBuffer:vb offset:0 atIndex:0];
	}

	void MetalRenderCommand::DrawRenderable(const Renderable* pRenderable, Technique* pTech)
	{
        MTLPrimitiveType ePrimitiveType = MetalMapping::GetPrimitiveType(pRenderable->m_ePrimitiveType);
        
        const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;
        
        UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();

        MetalIndexBuffer* pMetalIndexBuffer = (MetalIndexBuffer*)(pRenderable->m_pIndexBuffer.get());
        id<MTLBuffer> ib = pMetalIndexBuffer->GetMetalIndexBuffer();
        
        MTLIndexType ibType = pRenderable->m_pIndexBuffer->GetIndexType() == INDEX_TYPE_U16 ? MTLIndexTypeUInt16 : MTLIndexTypeUInt32;
        
        [m_encoder drawIndexedPrimitives:ePrimitiveType indexCount:nIndexCount indexType:ibType indexBuffer:ib indexBufferOffset:0];
	}

}


