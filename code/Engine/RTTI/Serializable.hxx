
#include "AttributeManager.h"
#include "Serializable.h"
#include "Engine/Resource/XmlFile.h"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.h"

namespace ma
{

	Serializable::Serializable()
	{
		m_bLoadOver = false;
	}

	Serializable::~Serializable()
	{
	}

	void Serializable::SaveToXML(const char* pszXMLFile)
	{
		string strFullName = GetArchiveMananger()->GetSaveDir();
		strFullName += pszXMLFile;

		TiXmlDocument doc;
		TiXmlElement* pRoot = new TiXmlElement(this->GetClassName());
		doc.LinkEndChild(pRoot);
		this->Export(pRoot);
		bool bRes = doc.SaveFile(strFullName.c_str());
		ASSERT(bRes);
	}

	void Serializable::LoadFromXML(const char* pszXMlFile)
	{
		m_pXMlFile = CreateXmlFile(pszXMlFile);
		m_bLoadOver = false;
		IsReady();
	}

	bool Serializable::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if (m_pXMlFile == NULL || !m_pXMlFile->IsReady())
			return false;

		TiXmlElement* pXmlRoot = m_pXMlFile->GetDoc().FirstChildElement(this->GetClassName());
		ASSERT(pXmlRoot);
		this->Improt(pXmlRoot);

		m_bLoadOver = true;

		return true;
	}

}
