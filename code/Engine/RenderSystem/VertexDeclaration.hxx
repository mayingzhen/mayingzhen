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

	void VertexDeclaration::Init(UINT nVertexType)
	{
		int voffset = 0;
		if (nVertexType & DUM_POSITION)
		{
			AddElement(0, voffset, DT_FLOAT3, DU_POSITION, 0);
			voffset += 12;
		}
		if (nVertexType & DUM_TEXCOORD)
		{
			AddElement(0, voffset, DT_FLOAT2, DU_TEXCOORD, 0);
			voffset += 8;
		}
		if (nVertexType & DUM_NORMAL)
		{
			AddElement(0, voffset, DT_FLOAT3, DU_NORMAL, 0);
			voffset += 12;
		}
		if (nVertexType & DUM_TANGENT)
		{
			AddElement(0, voffset, DT_FLOAT3, DU_TANGENT, 0);
			voffset += 12;
		}
		if (nVertexType & DUM_BLENDINDICES)
		{
			AddElement(0, voffset, DT_UBYTE4, DU_BLENDINDICES, 0);
			voffset += 4;
		}
		if (nVertexType & DUM_BLENDWEIGHT)
		{
			AddElement(0, voffset, DT_UBYTE4N, DU_BLENDWEIGHT, 0);
			voffset += 4;
		}
		
		m_nStreamStride = voffset;
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

}
