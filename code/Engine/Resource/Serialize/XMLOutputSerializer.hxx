#include "XMLOutputSerializer.h"

namespace ma
{
	XMLOutputSerializer::XMLOutputSerializer()
	{
		m_pRootElem = new TiXmlElement("Root");	
		m_pCurElem = m_pRootElem;
	}

	XMLOutputSerializer::~XMLOutputSerializer()
	{
		SAFE_DELETE(m_pRootElem);
	}

	bool XMLOutputSerializer::IsReading() const
	{
		return false;
	}

	bool XMLOutputSerializer::Save(const char* pszFileName)
	{
		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszFileName;

		TiXmlDocument document;
		document.LinkEndChild( m_pRootElem->Clone() );
		bool bSaveOK = document.SaveFile(strFullName.c_str());
		ASSERT(bSaveOK);
		return bSaveOK;
	}

	void XMLOutputSerializer::BeginSection(const char* pszLable)
	{
		ASSERT(m_pRootElem);
		if (m_pRootElem == NULL)
		{
			m_pRootElem = new TiXmlElement(pszLable);
			m_pCurElem = m_pRootElem;
		}
		else
		{
			TiXmlElement* pEle = new TiXmlElement(pszLable);
			m_pCurElem->LinkEndChild(pEle);
			m_pCurElem = pEle;
		}
	}

	void XMLOutputSerializer::EndSection()
	{
		ASSERT(m_pCurElem);
		if (m_pCurElem == NULL)
			return;

		m_pCurElem = static_cast<TiXmlElement*>(m_pCurElem->Parent());
	}
}

