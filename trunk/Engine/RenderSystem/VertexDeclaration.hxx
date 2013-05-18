#include "VertexDeclaration.h"

namespace ma 
{

// 	int VertexDeclaration::GetDeclTypeSize(DECL_TYPE type)
// 	{
// 		switch (type)
// 		{
// 		case DT_FLOAT1: return 4;
// 		case DT_FLOAT2: return 8;
// 		case DT_FLOAT3: return 12;
// 		case DT_FLOAT4: return 16;
// 		case DT_COLOR:  return 4;
// 		case DT_UBYTE4: return 4;
// 		case DT_SHORT2: return 4;
// 		case DT_SHORT4: return 8;
// 		}
// 
// 		ASSERT(false);
// 
// 		return 0;
// 	}

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

		Create();
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
	}

// 	int VertexDeclaration::GetElementStride(int index)
// 	{
// 
// 	}

// 	int VertexDeclaration::GetStreamSize(short number)
// 	{
// 		int size = 0;
// 		for (int i = 0; i < mElementCount; ++i)
// 		{
// 			if (mElements[i].Stream == number)
// 				size += GetDeclTypeSize(mElements[i].Type); 
// 		}
// 
// 		return size;
// 	}

// 	void VertexDeclaration::GetStreamItem(short number, VertexElement * ve, int & count)
// 	{
// 		count = 0;
// 
// 		for (int i = 0; i < mElementCount; ++i)
// 		{
// 			if (mElements[i].Stream == number)
// 			{
// 				ve[count] = mElements[i];
// 				++count;
// 			}
// 		}
// 	}

// 	void VertexDeclaration::GetAllStreamItem(VertexElement * ve, int & count)
// 	{
// 		count = 0;
// 
// 		for (int i = 0; i < mElementCount; ++i)
// 		{
// 			ve[count] = mElements[i];
// 
// 			++count;
// 		}
// 	}

// 	void VertexDeclaration::GetElement(VertexElement & ve)
// 	{
// 		bool flag = FALSE;
// 
// 		for (int i = 0; i < mElementCount; ++i)
// 		{
// 			VertexElement _ve;
// 			_ve = mElements[i];
// 
// 			if ((ve.Stream == DECL_UNUSED || ve.Stream == _ve.Stream) &&
// 				(ve.Usage == _ve.Usage) || (ve.Type == _ve.Type) &&
// 				(ve.UsageIndex == _ve.UsageIndex))
// 			{
// 				ve = _ve;
// 				flag = TRUE;
// 				break;
// 			}
// 		}
// 
// 		if (!flag)
// 			ve.Stream = DECL_UNUSED;
// 	}

// 	const VertexElement * VertexDeclaration::GetElementByUsage(DECL_USAGE usage)
// 	{
// 		for (int i = 0; i < mElementCount; ++i)
// 		{
// 			const VertexElement & ve = mElements[i];
// 
// 			if (ve.Usage == usage)
// 			{
// 				return &ve;
// 			}
// 		}
// 
// 		return NULL;
// 	}


}
