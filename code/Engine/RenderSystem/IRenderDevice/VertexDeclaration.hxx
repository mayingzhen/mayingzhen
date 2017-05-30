#include "VertexDeclaration.h"

namespace ma
{

	VertexElement::VertexElement()
		: Stream(DECL_UNUSED),
		Offset(0),
		Type(DT_UNKNOWN),
		Usage(DU_UNKNOWN),
		UsageIndex(0),
		mHash(0)
	{
	}

	VertexElement::VertexElement(short stream, short offset, DECL_TYPE type, DECL_USAGE use, unsigned char index)
		: Stream(stream),
		Offset(offset),
		Type(type),
		Usage(use),
		UsageIndex(index),
		mHash(0)
	{
		BuildHash();
	}

	void VertexElement::RegisterAttribute()
	{
		ATTRIBUTE(VertexElement, "Stream", Stream, short, 0, AM_DEFAULT);
		ATTRIBUTE(VertexElement, "Offset", Offset, short, 0, AM_DEFAULT);
		ENUM_ATTRIBUTE(VertexElement, "Type", Type, DECL_TYPE, strDECL_TYPE, DT_UNKNOWN, AM_DEFAULT);
		ENUM_ATTRIBUTE(VertexElement, "Usage", Usage, DECL_USAGE, strDECL_USAGE, DU_UNKNOWN, AM_DEFAULT);
		ATTRIBUTE(VertexElement, "UsageIndex", UsageIndex, unsigned char, 0, AM_DEFAULT);
	}

	// ---------------------------------------------------------------------
	//  stream  offset  type  semantic index GLLocate
	//   4        12     4      4        4	    4
	//  0~15    0~4096-1 0~15   0~15    0~15   0~15
	// ---------------------------------------------------------------------
	void VertexElement::BuildHash()
	{
		mHash = 0;

		//ASSERT(mGLLocate <= 15);
		//UINT32 n = mGLLocate;
		//mHash += n;

		ASSERT(UsageIndex <= 15);
		UINT32 n = UsageIndex;
		mHash += n << 4;

		ASSERT(Usage <= 15);
		n = Usage;
		mHash += n << 8;

		ASSERT(Type <= 15);
		n = Type;
		mHash += n << 12;

		ASSERT(Offset <= 4096 - 1);
		n = Offset;
		mHash += n << 16;

		ASSERT(Stream <= 15);
		n = Stream;
		mHash += n << 28;
	}


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
		for (int i = 0; i < m_ElementCount; ++i)
		{
			VertexElement& e = m_Elements[i];
			e.BuildHash();
			nNum += e.GetHash();
		}
		return nNum;
	}

	bool VertexDeclaration::Import(rapidxml::xml_node<>* pXmlVD)
	{
		m_ElementCount = 0;
		rapidxml::xml_node<>* pXmlEment = pXmlVD->first_node("Element");
		while (pXmlEment)
		{
			m_Elements[m_ElementCount].Import(pXmlEment);

			m_nStreamStride = m_Elements[m_ElementCount].Offset + GetDeclTypeSize(m_Elements[m_ElementCount].Type);

			++m_ElementCount;

			pXmlEment = pXmlEment->next_sibling("Element");
		}

		return true;
	}

	bool VertexDeclaration::Export(rapidxml::xml_node<>* pXmlVD, rapidxml::xml_document<>& doc)
	{
		for (int i = 0; i < m_ElementCount; ++i)
		{
			rapidxml::xml_node<>* pXmlElement = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Element"));
			pXmlVD->append_node(pXmlElement);

			m_Elements[i].Export(pXmlElement, doc);
		}

		return true;
	}

	RefPtr<VertexDeclaration> CreateVertexDeclaration()
	{
		return GetRenderDevice()->CreateVertexDeclaration();
	}
}
