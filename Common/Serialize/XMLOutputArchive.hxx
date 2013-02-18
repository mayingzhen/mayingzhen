#include <Common/Serialize/XMLOutputArchive.h>
#include <Common/tinyxml/TinyXML.h>
#include <Common/Serialize/StringConverter.h>

namespace ma
{
	XMLOutputArchive::XMLOutputArchive()
		:m_pDoc(NULL)
		,m_pParentElem(NULL)
	{
		m_pDoc = new TiXmlDocument();
	}

	XMLOutputArchive::~XMLOutputArchive()
	{
		Close();
		SAFE_DELETE(m_pDoc);
		m_pParentElem = NULL;

	}

	bool XMLOutputArchive::IsReading() const
	{
		return false;
	}

	void XMLOutputArchive::Release()
	{
		delete this;
	}


	bool XMLOutputArchive::Open(const char* pszFilename)
	{
		Close();
		m_strFilename = pszFilename;
		m_pDoc->Clear();
		TiXmlElement rootElem("Root");
		m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pDoc->InsertEndChild(rootElem));
		return true;

	}

	void XMLOutputArchive::Close()
	{
		if (m_strFilename.size() > 0)
		{

			bool bSaveOK = m_pDoc->SaveFile(m_strFilename.c_str());
			(void)bSaveOK;

			m_strFilename = "";

			assert(bSaveOK && "Fail to Save File");
			m_pParentElem = NULL;
		}

	}

	bool XMLOutputArchive::OpenToMemory()
	{
		return Open("\0");
	}

// 	bool XMLOutputArchive::SaveToMemory(char** ppData,xmUint* pDataSize,IAllocator* pAlloc)
// 	{
// 		TiXmlPrinter printer;
// 		printer.SetIndent( "\t" );
// 		m_pDoc->Accept( &printer );
// 		xmUint nSize = printer.Size();
// 		*ppData = (char*)pAlloc->Malloc(nSize+1);
// 		memcpy(*ppData,printer.CStr(),nSize);
// 		(*ppData)[nSize] = '\0';
// 		return true;
// 	}

	void XMLOutputArchive::BeginSection(const char* pszLable)
	{

		TiXmlElement elem(pszLable);
		m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pParentElem->InsertEndChild(elem));
	}

	void XMLOutputArchive::EndSection()
	{
		m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pParentElem->Parent());
	}

	void XMLOutputArchive::Serialize(bool& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(char& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(short& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(unsigned short& val, const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(unsigned int& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(int& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(unsigned long &val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(long &val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(xmUint64& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(float& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(unsigned char& val, const char* pszLabel)
	{
		TiXmlElement elem(pszLabel);
		TiXmlText text(StringConverter::ToString(val).c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

	void XMLOutputArchive::Serialize(std::string& val,const char* pszLable)
	{
		TiXmlElement elem(pszLable);
		TiXmlText text(val.c_str());
		elem.InsertEndChild(text);
		m_pParentElem->InsertEndChild(elem);
	}

// 	void XMLOutputArchive::Serialize(IStringWrapper* val,const char* pszLable )
// 	{
// 		assert(NULL != val);
// 		TiXmlElement elem(pszLable);
// 		TiXmlText text(val->GetValue());
// 		elem.InsertEndChild(text);
// 		m_pParentElem->InsertEndChild(elem);
// 	}

	SerializeListener* XMLOutputArchive::GetSerializeListener()
	{
		return this;
	}
}

