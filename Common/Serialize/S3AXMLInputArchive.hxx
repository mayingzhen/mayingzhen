#include <Common/Serialize/XMLInputArchive.h>
#include <Common/tinyxml/TinyXML.h>
#include <Common/Serialize/S3AStringConverter.h>

namespace ma
{
	S3AXMLInputArchive::S3AXMLInputArchive()
		:m_pDoc(NULL)
		,m_pCurElem(NULL)
		,m_nullChild(0)
	{
		m_pDoc = new TiXmlDocument();
	}

	S3AXMLInputArchive::~S3AXMLInputArchive()
	{
		xmSafeDelete(m_pDoc);
		m_pCurElem = NULL;

	}


	bool S3AXMLInputArchive::IsReading() const
	{
		return true;
	}

	void S3AXMLInputArchive::Release()
	{
		delete this;
	}

	bool S3AXMLInputArchive::Open(const char* pszFilename)
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

	bool S3AXMLInputArchive::OpenFromMemory(const char* pszData)
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

	void S3AXMLInputArchive::Close()
	{
		m_pCurElem = NULL;
	}

	TiXmlDocument* S3AXMLInputArchive::GetXMLDocument()
	{
		return m_pDoc;
	}

	void S3AXMLInputArchive::BeginSection(const char* pszLable)
	{
		assert(_stricmp(pszLable,m_pCurElem->Value()) == 0);
		TiXmlElement* pFirstChild = reinterpret_cast<TiXmlElement*>(m_pCurElem->FirstChild());
		m_nullChild += (pFirstChild == NULL);
		m_pCurElem = m_nullChild ? m_pCurElem : pFirstChild;

	}

	void S3AXMLInputArchive::EndSection()
	{
		m_pCurElem = m_nullChild ? m_pCurElem : reinterpret_cast<TiXmlElement*>(m_pCurElem->Parent());
		m_nullChild -= m_nullChild > 0 ? 1 : 0;
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(bool& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(unsigned char& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(short& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(unsigned short& val, const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void S3AXMLInputArchive::SerializeBasicType(unsigned int& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(int& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(unsigned long &val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(long &val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void S3AXMLInputArchive::SerializeBasicType(xmUint64& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}


	void S3AXMLInputArchive::SerializeBasicType(float& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		S3AStringConverter::ToValue(val,m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(std::string& val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		val = NULL != m_pCurElem->GetText() ? m_pCurElem->GetText() : "\0";
		NextSiblingElement();
	}

	void S3AXMLInputArchive::SerializeBasicType(IS3AStringWrapper* val,const char* pszLable)
	{
		assert(m_nullChild == 0);
		assert(strcmp(pszLable,m_pCurElem->Value()) == 0);
		val->SetValue(m_pCurElem->GetText());
		NextSiblingElement();
	}

	void S3AXMLInputArchive::NextSiblingElement()
	{
		TiXmlElement* pNextElem = m_pCurElem->NextSiblingElement();
		m_pCurElem = pNextElem ? pNextElem : m_pCurElem;
	}

	SerializeListener* S3AXMLInputArchive::GetSerializeListener()
	{
		return this;
	}	
}

