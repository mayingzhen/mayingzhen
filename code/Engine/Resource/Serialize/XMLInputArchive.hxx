#include "XMLInputArchive.h"
#include "tinyxml/tinyxml.h"
#include "StringConverter.h"

namespace ma
{
	XMLInputArchive::XMLInputArchive()
	{
		m_pDoc = NULL;
		m_pRootElem = NULL;
		m_pCurElem = NULL;
	}

	XMLInputArchive::~XMLInputArchive()
	{
		
	}

	bool XMLInputArchive::Close()
	{
		SAFE_DELETE(m_pDoc);
		return true;
	}

	bool XMLInputArchive::Open(const char* pszFilename)
	{
		m_pDoc = new TiXmlDocument();
			
		bool bLoadOK = m_pDoc->LoadFile(pszFilename);
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return false;

		m_pRootElem = m_pDoc->RootElement();
		m_pCurElem = m_pRootElem;

		return true;
	}

	bool XMLInputArchive::Open(XMLOutputArchive& xmlOutAr)
	{
		ASSERT(m_pDoc == NULL);

		m_pRootElem = xmlOutAr.GetRootElement();
		m_pCurElem = m_pRootElem;

		return true;
	}

	bool XMLInputArchive::IsReading() const
	{
		return true;
	}

	void XMLInputArchive::BeginSection(const char* pszLable)
	{
		ASSERT(m_pCurElem);
		if (m_pCurElem == NULL)
			return;

		ASSERT(strcmp(pszLable,m_pCurElem->Value()) == 0);
		TiXmlElement* pChildEle = m_pCurElem->FirstChildElement();
		if (pChildEle != NULL)
			m_pCurElem = pChildEle;
	}

	void XMLInputArchive::EndSection()
	{
		TiXmlElement* pNextElem = m_pCurElem->NextSiblingElement();
		if (pNextElem == NULL)
			m_pCurElem = static_cast<TiXmlElement*>(m_pCurElem->Parent());	
		else
			m_pCurElem = pNextElem;
	}

	void XMLInputArchive::Serialize(bool& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(unsigned char& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(short& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(unsigned int& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(int& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(long &val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(Uint64& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(float& val,const char* pszLable)
	{
		BeginSection(pszLable);
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
		EndSection();
	}

	void XMLInputArchive::Serialize(std::string& val,const char* pszLable)
	{
// 		//ASSERT(m_nullChild == 0);
// 		ASSERT(strcmp(pszLable,m_pCurElem->Value()) == 0);
// 		val = NULL != m_pCurElem->GetText() ? m_pCurElem->GetText() : "\0";
// 		NextSiblingElement();
		BeginSection(pszLable);
		const char* pszValue = m_pCurElem->Attribute(pszLable);
		val = pszValue ? pszValue : "";
		EndSection();
	}	
}

