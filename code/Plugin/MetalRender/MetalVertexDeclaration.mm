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
        
        m_descriptor = [[MTLVertexDescriptor alloc] init];
        
        for (int i = 0; i < this->GetElementCount(); ++i)
        {
            const VertexElement& element = this->GetElement(i);
            
            MTLVertexAttributeDescriptor* attr = m_descriptor.attributes[i];
            attr.offset = element.Offset;
            attr.bufferIndex = 0;
            attr.format = MetalMapping::GetDeclType(element.Type);
            
        }
        
        m_descriptor.layouts[0].stride = this->GetStreanmStride();
        m_descriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        
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
