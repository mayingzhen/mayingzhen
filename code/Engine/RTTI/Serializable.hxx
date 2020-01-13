
#include "AttributeManager.h"
#include "Serializable.h"
#include "Engine/Resource/XmlFile.h"

namespace ma
{

	Serializable::Serializable()
	{
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

	void Serializable::LoadFromXML(const char* pszXMlFile, const RES_CALL_BACK& call_back)
	{
		m_strPath = pszXMlFile;
		CreateXmlFile(pszXMlFile, [this,call_back](Resource* res) {
			m_pXMlFile = (XmlFile*)(res);
			rapidxml::xml_document<>& doc = m_pXMlFile->GetDoc();
			rapidxml::xml_node<>* pXmlRoot = doc.first_node(/*this->GetTypeName()*/);
			ASSERT(pXmlRoot);
			this->Import(pXmlRoot);
			if (call_back)
			{
				call_back(res);
			}
		});
	}

	void Serializable::LoadFromXMLSync(const char* pszXMlFile)
	{
		m_strPath = pszXMlFile;
		m_pXMlFile = CreateXmlFileSync(pszXMlFile);
		
		rapidxml::xml_document<>& doc = m_pXMlFile->GetDoc();
		rapidxml::xml_node<>* pXmlRoot = doc.first_node(/*this->GetTypeName()*/);
		ASSERT(pXmlRoot);
		this->Import(pXmlRoot);
	}

	bool Serializable::IsReady()
	{
		return m_pXMlFile != nullptr;
	}

}
