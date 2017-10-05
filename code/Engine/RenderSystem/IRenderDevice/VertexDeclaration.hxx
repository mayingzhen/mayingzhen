#include "VertexDeclaration.h"

namespace ma
{

	VertexElement::VertexElement()
		: Stream(-1),
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
		ATTRIBUTE(VertexElement, "UsageIndex", UsageIndex, short, 0, AM_DEFAULT);
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
	}

	VertexDeclaration::~VertexDeclaration()
	{
	}

	void VertexDeclaration::AddElement(const VertexElement& element)
	{
		UINT nStream = element.Stream;

		m_arrStreamElement.resize(nStream + 1);
		m_arrStreamStride.resize(nStream + 1);

		m_arrStreamElement[nStream].push_back(element);

		m_arrStreamStride[nStream] = element.Offset + GetDeclTypeSize(element.Type);
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
		for (UINT i = 0; i < m_arrStreamElement.size(); ++i)
		{
			for (UINT j = 0; j < m_arrStreamElement[i].size(); ++j)
			{
				VertexElement& e = m_arrStreamElement[i][j];
				e.BuildHash();
				nNum += e.GetHash();
			}
		}

		return nNum;
	}

	bool VertexDeclaration::Import(rapidxml::xml_node<>* pXmlVD)
	{
		rapidxml::xml_node<>* pXmlEment = pXmlVD->first_node("Element");
		while (pXmlEment)
		{
			VertexElement element;
			element.Import(pXmlEment);

			AddElement(element);

			pXmlEment = pXmlEment->next_sibling("Element");
		}

		return true;
	}

	bool VertexDeclaration::Export(rapidxml::xml_node<>* pXmlVD, rapidxml::xml_document<>& doc)
	{
		for (UINT i = 0; i < m_arrStreamElement.size(); ++i)
		{
			for (UINT j = 0; j < m_arrStreamElement[i].size(); ++j)
			{
				rapidxml::xml_node<>* pXmlElement = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Element"));
				pXmlVD->append_node(pXmlElement);

				VertexElement& ele = m_arrStreamElement[i][j];
				ele.Export(pXmlElement, doc);
			}
		}

		return true;
	}

	RefPtr<VertexDeclaration> CreateVertexDeclaration()
	{
		return GetRenderDevice()->CreateVertexDeclaration();
	}
}
