#include "XmlFile.h"

namespace ma
{
	XmlFile::XmlFile()
	{
	}

	XmlFile::~XmlFile()
	{

	}


	bool XmlFile::InitRes()
	{
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
		
		Import(xmlRoot);

		return true;
	}

	bool XmlFile::SaveToFile(const char* pszFile)
	{
		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszFile;
		strFullName = StringUtil::standardiseDir(strFullName);

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string(this->GetClassName()));
		this->Export(pRoot,doc);
		doc.append_node(pRoot);
		std::ofstream out(strFullName.c_str(), ios::out|ios::binary);
		out << doc;

		return true;
	}

	bool XmlFile::Import(rapidxml::xml_node<>* pXmlElem)
	{
		//ASSERT(false);

		return false;
	}

	bool XmlFile::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		//ASSERT(false);

		return false;
	}

	ResourceSystem<XmlFile>* g_pXmlFileManager = NULL;

	RefPtr<XmlFile> CreateXmlFile(const char* pszFile)
	{
		return g_pXmlFileManager->CreateResource(pszFile);
	}

	
}