#include "XMLInputSerializer.h"

namespace ma
{
	XMLInputSerializer::XMLInputSerializer()
	{
		m_pDoc = NULL;
		m_pRootElem = NULL;
		m_pCurElem = NULL;
	}

	XMLInputSerializer::~XMLInputSerializer()
	{
		Close();
	}

	bool XMLInputSerializer::Close()
	{
		SAFE_DELETE(m_pDoc);
		return true;
	}

	bool XMLInputSerializer::Open(XMLOutputSerializer& xmlOutAr)
	{
		ASSERT(m_pDoc == NULL);

		m_pRootElem = xmlOutAr.GetRootElement();
		m_pCurElem = m_pRootElem;

		return true;
	}

	bool XMLInputSerializer::Open(MemoryStream* pDataStream)
	{
		ASSERT(pDataStream);

		m_pDoc = new TiXmlDocument();
		bool bLoadOK = m_pDoc->Parse( (const char*)pDataStream->GetPtr() ) != NULL;
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return false;

		m_pRootElem = m_pDoc->RootElement();
		m_pCurElem = NULL;

		return true;
	}

	bool XMLInputSerializer::IsReading() const
	{
		return true;
	}

	void XMLInputSerializer::BeginSection(const char* pszLable)
	{
		TiXmlElement* pCurElem = m_pCurElem ? m_pCurElem : m_pRootElem;
		m_pCurElem = pCurElem->FirstChildElement(pszLable);
		ASSERT(m_pCurElem);
	}

	void XMLInputSerializer::EndSection()
	{
		ASSERT(m_pCurElem);
		m_pCurElem = static_cast<TiXmlElement*>(m_pCurElem->Parent());
		ASSERT(m_pCurElem);
	}
}

