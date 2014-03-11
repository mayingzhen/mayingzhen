#include "XMLOutputArchive.h"
#include "tinyxml/TinyXML.h"
#include "StringConverter.h"

namespace ma
{
	XMLOutputArchive::XMLOutputArchive()
	{
		//m_pRootElem = NULL;
		//m_pCurElem = NULL;
		m_pRootElem = new TiXmlElement("Root");	
		m_pCurElem = m_pRootElem;
	}

	XMLOutputArchive::~XMLOutputArchive()
	{
		//Close();
		SAFE_DELETE(m_pRootElem);
	}

	bool XMLOutputArchive::IsReading() const
	{
		return false;
	}


	bool XMLOutputArchive::Open(const char* pszFilename)
	{
		m_strFilename = pszFilename;

// 		m_pRootElem = new TiXmlElement("Root");	
// 		m_pCurElem = m_pRootElem;

		return true;
	}

	void XMLOutputArchive::Save(const char* pszFileName)
	{
		TiXmlDocument document;
		document.LinkEndChild( m_pRootElem->Clone() );
		bool bSaveOK = document.SaveFile(pszFileName);
		ASSERT(bSaveOK);
	}

	void XMLOutputArchive::Close()
	{
		TiXmlDocument document;
		document.LinkEndChild(m_pRootElem);
		bool bSaveOK = document.SaveFile(m_strFilename.c_str());
		ASSERT(bSaveOK);
		m_pRootElem = NULL;
	}

	void XMLOutputArchive::BeginSection(const char* pszLable)
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

	void XMLOutputArchive::EndSection()
	{
		ASSERT(m_pCurElem);
		if (m_pCurElem == NULL)
			return;

		m_pCurElem = static_cast<TiXmlElement*>(m_pCurElem->Parent());
	}

	void XMLOutputArchive::Serialize(bool& val,const char* pszLable)
	{
		//TiXmlElement elem(pszLable);
		//TiXmlText text(StringConverter::ToString(val).c_str());
		//elem.InsertEndChild(text);
		//m_pParentElem->InsertEndChild(elem);
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(char& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(short& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned int& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(int& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(long &val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(Uint64& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(float& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned char& val, const char* pszLabel)
	{
		//BeginSection(pszLabel);
		m_pCurElem->SetAttribute(pszLabel,StringConverter::ToString(val).c_str());
		//EndSection();
	}

	void XMLOutputArchive::Serialize(std::string& val,const char* pszLable)
	{
		//BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,val.c_str());
		//EndSection();
	}
}

