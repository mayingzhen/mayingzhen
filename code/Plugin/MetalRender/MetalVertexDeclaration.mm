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

#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
    
	void MetalVertexDeclaration::RT_StreamComplete()
	{
        if (m_descriptor != nil)
            return;
        
        m_descriptor = [[MTLVertexDescriptor alloc] init];
        
        for (int i = 0; i < this->GetElementCount(VERTEX_BUFFER_BIND_ID); ++i)
        {
            const VertexElement& element = this->GetElement(VERTEX_BUFFER_BIND_ID,i);
            
            MTLVertexAttributeDescriptor* attr = m_descriptor.attributes[i];
            attr.offset = element.Offset;
            attr.bufferIndex = VERTEX_BUFFER_BIND_ID;
            attr.format = MetalMapping::GetDeclType(element.Type);
        }
        
        m_descriptor.layouts[VERTEX_BUFFER_BIND_ID].stride = this->GetStreanmStride(VERTEX_BUFFER_BIND_ID);
        m_descriptor.layouts[VERTEX_BUFFER_BIND_ID].stepFunction = MTLVertexStepFunctionPerVertex;
        
        if (this->GetElementCount(INSTANCE_BUFFER_BIND_ID) > 0)
        {
            for (UINT i = 0; i < this->GetElementCount(INSTANCE_BUFFER_BIND_ID); ++i)
            {
                const VertexElement& element = this->GetElement(INSTANCE_BUFFER_BIND_ID,i);
                
                MTLVertexAttributeDescriptor* attr = m_descriptor.attributes[i + this->GetElementCount(VERTEX_BUFFER_BIND_ID)];
                attr.offset = element.Offset;
                attr.bufferIndex = INSTANCE_BUFFER_BIND_ID;
                attr.format = MetalMapping::GetDeclType(element.Type);
            }
        
            m_descriptor.layouts[INSTANCE_BUFFER_BIND_ID].stride = this->GetStreanmStride(INSTANCE_BUFFER_BIND_ID);
            m_descriptor.layouts[INSTANCE_BUFFER_BIND_ID].stepFunction = MTLVertexStepFunctionPerInstance;
        }
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
