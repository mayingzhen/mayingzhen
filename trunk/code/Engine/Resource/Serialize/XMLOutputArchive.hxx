#include "XMLOutputArchive.h"
#include "tinyxml/TinyXML.h"
#include "StringConverter.h"

namespace ma
{
	XMLOutputArchive::XMLOutputArchive()
	{
		m_pRootElem = NULL;
		m_pCurElem = NULL;
	}

	XMLOutputArchive::~XMLOutputArchive()
	{
		//Close();
	}

	bool XMLOutputArchive::IsReading() const
	{
		return false;
	}


	bool XMLOutputArchive::Open(const char* pszFilename)
	{
		m_strFilename = pszFilename;
		return true;
	}

	void XMLOutputArchive::Close()
	{
		TiXmlDocument document;
		document.LinkEndChild(m_pRootElem);
		bool bSaveOK = document.SaveFile(m_strFilename.c_str());
		ASSERT(bSaveOK);
	}

	void XMLOutputArchive::BeginSection(const char* pszLable)
	{
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
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(char& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(short& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned int& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(int& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(long &val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(Uint64& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(float& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(unsigned char& val, const char* pszLabel)
	{
		BeginSection(pszLabel);
		m_pCurElem->SetAttribute(pszLabel,StringConverter::ToString(val).c_str());
		EndSection();
	}

	void XMLOutputArchive::Serialize(std::string& val,const char* pszLable)
	{
		BeginSection(pszLable);
		m_pCurElem->SetAttribute(pszLable,val.c_str());
		EndSection();
	}
}

