#include "VertexDeclaration.h"

namespace ma 
{
	VertexDeclaration::VertexDeclaration()
	{
		m_ElementCount = 0;
		m_nStreamStride = 0;
	}

	VertexDeclaration::~VertexDeclaration()
	{
	}

	void VertexDeclaration::AddElement(short StreamNumber, short Offset,
		DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char UsageIndex)
	{
		ASSERT(StreamNumber < MAX_ELEMENT);

		int index = m_ElementCount;
		m_ElementCount++;

		m_Elements[index].Stream = StreamNumber;
		m_Elements[index].Offset = Offset;
		m_Elements[index].Type = DeclType;
		m_Elements[index].Usage = DeclUsage;
		m_Elements[index].UsageIndex = UsageIndex;

		m_nStreamStride = Offset + GetDeclTypeSize(DeclType);
	}

	void VertexDeclaration::AddElement(const VertexElement& element)
	{
		int index = m_ElementCount;
		m_ElementCount++;

		m_Elements[index] = element;

		m_nStreamStride = element.Offset + GetDeclTypeSize(element.Type);				
	}

	int VertexDeclaration::GetDeclTypeSize(DECL_TYPE type)
	{
		switch (type)
		{
		case DT_FLOAT1: return 4;
		case DT_FLOAT2: return 8;
		case DT_FLOAT3: return 12;
		case DT_FLOAT4: return 16;
		case DT_COLOR:  return 4;
		case DT_UBYTE4: return 4;
		case DT_UBYTE4N:return 4;
		case DT_SHORT2: return 4;
		case DT_SHORT2N:return 4;
		case DT_SHORT4: return 8;
		case DT_SHORT4N:return 8;
		}

		ASSERT(false);

		return 0;
	}

	uint64 VertexDeclaration::GetHash()
	{
		uint64 nNum = 0;
		for (int i = 0;i< m_ElementCount;++i)
		{
			VertexElement& e = m_Elements[i];
			e.BuildHash();
			nNum += e.GetHash();
		}
		return nNum;
	}
}
