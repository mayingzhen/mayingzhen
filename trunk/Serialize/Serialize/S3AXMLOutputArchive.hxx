//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#include <Animation/Common/Serialize/S3AXMLOutputArchive.h>
#include <Animation/Common/xml/S3TinyXML.h>
#include <Animation/Common/S3AStringConverter.h>

S3AXMLOutputArchive::S3AXMLOutputArchive()
:m_pDoc(NULL)
,m_pParentElem(NULL)
{
	m_pDoc = new TiXmlDocument();
}

S3AXMLOutputArchive::~S3AXMLOutputArchive()
{
	Close();
	xmSafeDelete(m_pDoc);
	m_pParentElem = NULL;

}

bool S3AXMLOutputArchive::IsReading() const
{
	return false;
}

void S3AXMLOutputArchive::Release()
{
	delete this;
}


bool S3AXMLOutputArchive::Open(const char* pszFilename)
{
	Close();
	m_strFilename = pszFilename;
	m_pDoc->Clear();
	TiXmlElement rootElem("Root");
	m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pDoc->InsertEndChild(rootElem));
	return true;

}

void S3AXMLOutputArchive::Close()
{
	if (m_strFilename.size() > 0)
	{

		bool bSaveOK = m_pDoc->SaveFile(m_strFilename.c_str());
        (void)bSaveOK;
        
		m_strFilename = "";

		S3ASSERT(bSaveOK && "Fail to Save File");
		m_pParentElem = NULL;
	}

}

bool S3AXMLOutputArchive::OpenToMemory()
{
	return Open("\0");
}

bool S3AXMLOutputArchive::SaveToMemory(char** ppData,xmUint* pDataSize,IS3AAllocator* pAlloc)
{
	TiXmlPrinter printer;
	printer.SetIndent( "\t" );
	m_pDoc->Accept( &printer );
	xmUint nSize = printer.Size();
	*ppData = (char*)pAlloc->Malloc(nSize+1);
	memcpy(*ppData,printer.CStr(),nSize);
	(*ppData)[nSize] = '\0';
	return true;
}

void S3AXMLOutputArchive::BeginSection(const char* pszLable)
{
	
	TiXmlElement elem(pszLable);
	m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pParentElem->InsertEndChild(elem));
}

void S3AXMLOutputArchive::EndSection()
{
	m_pParentElem = reinterpret_cast<TiXmlElement*>(m_pParentElem->Parent());
}

void S3AXMLOutputArchive::SerializeBasicType(bool& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(char& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(short& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(unsigned short& val, const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(unsigned int& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(int& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(unsigned long &val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(long &val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(xmUint64& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(float& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(unsigned char& val, const char* pszLabel)
{
	TiXmlElement elem(pszLabel);
	TiXmlText text(S3AStringConverter::ToString(val).c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(std::string& val,const char* pszLable)
{
	TiXmlElement elem(pszLable);
	TiXmlText text(val.c_str());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

void S3AXMLOutputArchive::SerializeBasicType(IS3AStringWrapper* val,const char* pszLable )
{
	S3ASSERT(NULL != val);
	TiXmlElement elem(pszLable);
	TiXmlText text(val->GetValue());
	elem.InsertEndChild(text);
	m_pParentElem->InsertEndChild(elem);
}

SerializeListener* S3AXMLOutputArchive::GetSerializeListener()
{
	return this;
}