#include "XmlFile.h"

namespace ma
{
	XmlFile::XmlFile()
	{
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

	ResourceSystem<XmlFile>* g_pXmlFileManager = NULL;

	RefPtr<XmlFile> CreateXmlFile(const char* pszFile)
	{
		return g_pXmlFileManager->CreateResource(pszFile);
	}

	
}