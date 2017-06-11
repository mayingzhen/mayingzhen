#include "MetalVertexDeclaration.h"
#include "MetalShaderProgram.h"
#include "MetalMapping.h"

namespace ma
{
	std::map<uint64, MTLVertexDescriptor* > g_vertexDeclarationPool;

	MetalVertexDeclaration::MetalVertexDeclaration()
	{
		m_descriptor = nil;
	}

	MetalVertexDeclaration::~MetalVertexDeclaration()
	{
        if (m_descriptor != nil)
            [m_descriptor release];
        m_descriptor = nil;
	}

	void MetalVertexDeclaration::RT_StreamComplete()
	{
        if (m_descriptor != nil)
            return;
        
        //m_descriptor = nil;
        m_descriptor = [[MTLVertexDescriptor alloc] init];
        
        for (int i = 0; i < this->GetElementCount(); ++i)
        {
            const VertexElement& element = this->GetElement(i);
            
            MTLVertexAttributeDescriptor* attr = m_descriptor.attributes[i];
            attr.offset = element.Offset;
            // stream 0 is reserved for constant buffer
            //attr.bufferIndex = 0;//element.UsageIndex + 1;
            attr.format = MetalMapping::GetDeclType(element.Type);
            
        }
        
        m_descriptor.layouts[0].stride = this->GetStreanmStride();
        m_descriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        
        
        /*

		ASSERT(m_descriptor == nil);
        m_descriptor = [[MTLVertexDescriptor alloc] init];
        
		uint64 nHash = this->GetHash();

		auto it = g_vertexDeclarationPool.find(nHash);
		if (it != g_vertexDeclarationPool.end())
		{
			m_descriptor = it->second;
		}
		else
		{
            m_vertex_size = stream_assign.GetVertexSize();
            m_assign = StreamAssignmentPtr(new StreamAssignment(stream_assign));
            m_stream_count = 0;
            bool bit_vec[MAX_STREAMS];
            memset(bit_vec, 0, sizeof(bit_vec));
            nfd::Word stream = 0;
            nfd::Word offset = 0;
            for (nfd::Dword i = 0; i< stream_assign.GetLayout().GetCount(); ++i)
            {
                stream = stream_assign.GetAssign(i);
                offset = stream_assign.GetOffset(i);
                const VertexElement& layout = stream_assign.GetLayout()[i];
                auto u = layout.GetUsage();
                if (u == VertexElement::USAGE_POSITIONT)
                {
                    u = VertexElement::USAGE_POSITION;
                }
                MTLVertexAttributeDescriptor * attr = m_descriptor.attributes[u];
                attr.offset = offset;
                // stream 0 is reserved for constant buffer
                attr.bufferIndex = stream + 1;
                attr.format = GetDataFormat(layout);
                if (!bit_vec[stream])
                {
                    bit_vec[stream] = true;
                    m_stream_count++;
                }
            }
            for (nfd::Dword i = 0; i < MAX_STREAMS; ++i)
            {
                if (!bit_vec[i])
                {
                    continue;
                }
                m_descriptor.layouts[i + 1].stride = stream_assign.GetStreamStride(i);
                m_descriptor.layouts[i + 1].stepFunction =
                (stream_assign.IsStreamPerInstance(i) ? MTLVertexStepFunctionPerInstance : MTLVertexStepFunctionPerVertex);
            }

			g_vertexDeclarationPool.insert(std::make_pair(nHash, pMetalVertexDecl));

			m_descriptor = pMetalVertexDecl;
		}
         */
	}

	void MetalVertexDeclaration::Clear()
	{
		for (auto it = g_vertexDeclarationPool.begin(); it != g_vertexDeclarationPool.end(); ++it)
		{
			//SAFE_RELEASE(it->second);
		}
		g_vertexDeclarationPool.clear();
	}
}
