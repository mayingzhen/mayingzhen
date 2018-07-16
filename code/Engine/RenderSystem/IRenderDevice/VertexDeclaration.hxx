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

	// ---------------------------------------------------------------------
	//  stream  offset  type  semantic index GLLocate
	//   4        12     4      4        4	    4
	//  0~15    0~4096-1 0~15   0~15    0~15   0~15
	// ---------------------------------------------------------------------
	void VertexElement::BuildHash()
	{
		mHash = 0;

		//ASSERT(mGLLocate <= 15);
		//uint32_t n = mGLLocate;
		//mHash += n;

		ASSERT(UsageIndex <= 15);
		uint32_t n = UsageIndex;
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
    
    uint32_t VertexDeclaration::GetStreanmStride(uint32_t nStream) const
    {
        if (nStream >= m_arrStreamStride.size())
            return 0;
        
        return m_arrStreamStride[nStream];
    }
    
    uint32_t VertexDeclaration::GetElementCount(uint32_t nStream)
    {
        if (nStream >= m_arrStreamElement.size())
            return 0;
        
        return (uint32_t)m_arrStreamElement[nStream].size();
    }
    
    const VertexElement& VertexDeclaration::GetElement(uint32_t nStream, uint32_t index)
    {
        static VertexElement ve;
        if (nStream >= m_arrStreamElement.size())
            return ve;
        
        return m_arrStreamElement[nStream][index];
    }

	void VertexDeclaration::AddElement(const VertexElement& element)
	{
		uint32_t nStream = element.Stream;

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

	uint64_t VertexDeclaration::GetHash()
	{
		uint64_t nNum = 0;
		for (uint32_t i = 0; i < m_arrStreamElement.size(); ++i)
		{
			for (uint32_t j = 0; j < m_arrStreamElement[i].size(); ++j)
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
			const char* buf = pXmlEment->findAttribute("Value");
			ASSERT(buf);

			std::vector<std::string> vec = StringUtil::split(buf, ",");
			ASSERT(vec.size() == 5);

			VertexElement ele;
			ele.Stream = StringConverter::parseInt(vec[0]);
			ele.Offset = StringConverter::parseInt(vec[1]);
			ele.Type = StringUtil::StringToEnum<DECL_TYPE>(vec[2], strDECL_TYPE);
			ele.Usage = StringUtil::StringToEnum<DECL_USAGE>(vec[3], strDECL_USAGE);
			ele.UsageIndex = StringConverter::parseInt(vec[4]);

			AddElement(ele);

			pXmlEment = pXmlEment->next_sibling("Element");
		}

		return true;
	}

	bool VertexDeclaration::Export(rapidxml::xml_node<>* pXmlVD, rapidxml::xml_document<>& doc)
	{
		for (uint32_t i = 0; i < m_arrStreamElement.size(); ++i)
		{
			for (uint32_t j = 0; j < m_arrStreamElement[i].size(); ++j)
			{
				rapidxml::xml_node<>* pXmlElement = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Element"));
				pXmlVD->append_node(pXmlElement);

				VertexElement& ele = m_arrStreamElement[i][j];

				char buf[MAX_PATH] = { 0 };
				sprintf(buf, "%d,%d,%s,%s,%d", 
					ele.Stream, 
					ele.Offset, 
					strDECL_TYPE[ele.Type], 
					strDECL_USAGE[ele.Usage],
					ele.UsageIndex);

				pXmlElement->append_attribute(doc.allocate_attribute(doc.allocate_string("Value"), doc.allocate_string(buf)));
			}
		}

		return true;
	}

	RefPtr<VertexDeclaration> CreateVertexDeclaration()
	{
		return GetRenderDevice()->CreateVertexDeclaration();
	}
}
