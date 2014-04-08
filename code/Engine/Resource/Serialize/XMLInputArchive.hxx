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
		Close();
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

	bool XMLInputArchive::Open(MemoryStream* pDataStream)
	{
		ASSERT(pDataStream);

		m_pDoc = new TiXmlDocument();
		bool bLoadOK = m_pDoc->Parse( (const char*)pDataStream->GetPtr() );
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return false;

		m_pRootElem = m_pDoc->RootElement();
		m_pCurElem = NULL;

		return true;
	}

	bool XMLInputArchive::IsReading() const
	{
		return true;
	}

	void XMLInputArchive::BeginSection(const char* pszLable)
	{
		TiXmlElement* pCurElem = m_pCurElem ? m_pCurElem : m_pRootElem;
		m_pCurElem = pCurElem->FirstChildElement(pszLable);
		ASSERT(m_pCurElem);
	}

	void XMLInputArchive::EndSection()
	{
		ASSERT(m_pCurElem);
		m_pCurElem = static_cast<TiXmlElement*>(m_pCurElem->Parent());
		ASSERT(m_pCurElem);
	}

	void XMLInputArchive::Serialize(bool& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(unsigned char& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(short& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(UINT& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(int& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(long &val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(Uint64& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(float& val,const char* pszLable)
	{
		StringConverter::ToValue(val,m_pCurElem->Attribute(pszLable));
	}

	void XMLInputArchive::Serialize(std::string& val,const char* pszLable)
	{
		const char* pszValue = m_pCurElem->Attribute(pszLable);
		val = pszValue ? pszValue : "";
	}	
}

