
#include "AttributeManager.h"
#include "Serializable.h"
#include "Engine/Resource/XmlFile.h"

namespace ma
{

	Serializable::Serializable()
	{
		m_bXMLLoadOver = false;
	}

	Serializable::~Serializable()
	{
	}

	void Serializable::SaveToXML(const char* pszXMLFile)
	{
		m_strPath = pszXMLFile;

		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszXMLFile;
		strFullName = StringUtil::standardiseDir(strFullName);

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string(this->GetTypeName()));
		this->Export(pRoot,doc);
		doc.append_node(pRoot);
		std::ofstream out(strFullName.c_str(), ios::out|ios::binary);
		out << doc;
	}

	void Serializable::LoadFromXML(const char* pszXMlFile)
	{
		m_strPath = pszXMlFile;
		m_pXMlFile = CreateXmlFile(pszXMlFile);
		IsReady();
	}

	bool Serializable::IsReady()
	{
		if (m_bXMLLoadOver)
			return true;

		if (m_pXMlFile == NULL || !m_pXMlFile->IsReady())
			return false;
		
		rapidxml::xml_document<>& doc = m_pXMlFile->GetDoc();
		rapidxml::xml_node<>* pXmlRoot = doc.first_node(/*this->GetTypeName()*/);
		ASSERT(pXmlRoot);
		this->Import(pXmlRoot);

		m_bXMLLoadOver = true;

		return true;
	}

}
