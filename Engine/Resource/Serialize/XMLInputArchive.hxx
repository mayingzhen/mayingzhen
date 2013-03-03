#include "XMLInputArchive.h"
#include "tinyxml/tinyxml.h"
#include "StringConverter.h"

namespace ma
{
	XMLInputArchive::XMLInputArchive()
		:m_pDoc(NULL)
		,m_pCurElem(NULL)
		,m_nullChild(0)
	{
		m_pDoc = new TiXmlDocument();
	}

	XMLInputArchive::~XMLInputArchive()
	{
		SAFE_DELETE(m_pDoc);
		m_pCurElem = NULL;

	}


	bool XMLInputArchive::IsReading() const
	{
		return true;
	}

	void XMLInputArchive::Release()
	{
		delete this;
	}

	bool XMLInputArchive::Open(const char* pszFilename)
	{

		Close();
		m_strFilename = pszFilename;


		bool bLoadOK = m_pDoc->LoadFile(pszFilename);

		if (bLoadOK)
		{
			m_pCurElem = m_pDoc->RootElement();
			m_pCurElem = reinterpret_cast<TiXmlElement*>(m_pCurElem->FirstChild());
		}

		return bLoadOK;

	}

	bool XMLInputArchive::OpenFromMemory(const char* pszData)
	{
		Close();
		m_strFilename = "Memory";
		m_pDoc->Parse(pszData);

		bool bLoadOK = !m_pDoc->Error();

		if (bLoadOK)
		{
			m_pCurElem = m_pDoc->RootElement();
			m_pCurElem = reinterpret_cast<TiXmlElement*>(m_pCurElem->FirstChild());
		}

		return bLoadOK;
	}

	void XMLInputArchive::Close()
	{
		m_pCurElem = NULL;

		if (m_strFilename.size() > 0)
		{

			bool bSaveOK = m_pDoc->SaveFile(m_strFilename.c_str());
			(void)bSaveOK;

			m_strFilename = "";

			assert(bSaveOK && "Fail to Save File");
		}
	}

	TiXmlDocument* XMLInputArchive::GetXMLDocument()
	{
		return m_pDoc;
	}

	void XMLInputArchive::BeginSection(const char* pszLable)
	{
		assert(_stricmp(pszLable,m_pCurElem->Value()) == 0);
		TiXmlElement* pFirstChild = reinterpret_cast<TiXmlElement*>(m_pCurElem->FirstChild());
		m_nullChild += (pFirstChild == NULL);
		m_pCurElem = m_nullChild ? m_pCurElem : pFirstChild;

	}

	void XMLInputArchive::EndSection()
	{
		m_pCurElem = m_nullChild ? m_pCurElem : reinterpret_cast<TiXmlElement*>(m_pCurElem->Parent());
		m_nullChild -= m_nullChild > 0 ? 1 : 0;
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(bool& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(unsigned char& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(short& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void XMLInputArchive::Serialize(unsigned int& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(int& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(long &val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void XMLInputArchive::Serialize(xmUint64& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void XMLInputArchive::Serialize(float& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		StringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void XMLInputArchive::Serialize(std::string& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		val = NULL != m_pCurElem->GetText() ? m_pCurElem->GetText() : "\0";
		NextSiblingElement();
	}

// 	void XMLInputArchive::Serialize(IStringWrapper* val,const char* pszLable)
// 	{
// 		assert(m_nullChild == 0);
// 		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
// 		val->SetValue(m_pCurElem->GetText());
// 		NextSiblingElement();
// 	}

	void XMLInputArchive::NextSiblingElement()
	{
		TiXmlElement* pNextElem = m_pCurElem->NextSiblingElement();
		m_pCurElem = pNextElem ? pNextElem : m_pCurElem;
	}

	SerializeListener* XMLInputArchive::GetSerializeListener()
	{
		return this;
	}	
}

