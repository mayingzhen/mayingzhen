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

		m_doc.parse<0>((char*)m_pDataStream->GetPtr());
		rapidxml::xml_node<>* xmlRoot = m_doc.first_node();
		if (xmlRoot == NULL)
		{
			LogError("Failed to load as first_node is NULL,%s",this->GetResPath());
			return false;
		}
		
		//m_pDataStream = NULL;

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