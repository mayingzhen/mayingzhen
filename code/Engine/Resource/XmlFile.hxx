#include "XmlFile.h"

namespace ma
{
	XmlFile::XmlFile()
	{
		m_eCreateType = XML_RES;
	}

	XmlFile::XmlFile(const char* pszPath)
		:Resource(pszPath)
	{
		m_eCreateType = XML_RES;
	}

	XmlFile::~XmlFile()
	{

	}


	bool XmlFile::CreateFromMemeory()
	{
		ASSERT(m_eResState == ResLoaded);

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		bool bLoadOK = m_doc.Parse( (const char*)m_pDataStream->GetPtr() ) != NULL;
		ASSERT(bLoadOK);
		if (!bLoadOK)
			return false;
		
		m_pDataStream = NULL;

		m_eResState = ResInited;

		IsReady();

		return true;
	}

	RefPtr<XmlFile> CreateXmlFile(const char* pszFile)
	{
		string strKey = pszFile;
		StringUtil::toLowerCase(strKey);
		RefPtr<XmlFile> pXmlFile = FindResource<XmlFile>( strKey.c_str() );
		if (pXmlFile)
			return pXmlFile;

		pXmlFile = new XmlFile(pszFile);
		pXmlFile->Load();
		GetResourceSystem()->AddResource(strKey.c_str(),pXmlFile.get());
		return pXmlFile;
	}
}