#include "XMLSerialize.h"

namespace ma
{
// 	void XMLSaveVisitor::SaveExternLink(TiXmlElement* pParent,ObjectVec& objVec)
// 	{
// 		TiXmlElement* pExternElem = new TiXmlElement("ExternLink");
// 		pParent->LinkEndChild(pExternElem);
// 
// 		for (Uint nCnt = 1; nCnt < objVec.size(); ++nCnt)
// 		{
// 			Object* pObj = objVec[nCnt];
// 			TiXmlElement* pLinkElem = new TiXmlElement("link");
// 			pLinkElem->SetAttribute("name",pObj->GetName().GetName());
// 			pLinkElem->SetAttribute("type",pObj->GetClass()->GetName().GetName());
// 			pExternElem->LinkEndChild(pLinkElem);
// 		}
// 	}

	bool XMLSaveVisitor::SaveToXMLFile(const char* pszFileName,Object* pObj)
	{
		if (pszFileName == NULL || pObj == NULL)
			return false;

		TiXmlDocument document;
		m_pRootElem = new TiXmlElement( pObj->GetName() );
		m_pParentElem = m_pRootElem;
		
		VisitProperty(pObj,"",RefMode_Composite);

		document.LinkEndChild(m_pRootElem);

		bool bSaveOK = document.SaveFile(pszFileName);

		return bSaveOK;
	}

	void XMLSaveVisitor::BeginVisitGroup(const char* propName,const char* groupType)
	{
		TiXmlElement* pGroupElem = new TiXmlElement(propName);
		pGroupElem->SetAttribute("type",groupType);
		m_pParentElem->LinkEndChild(pGroupElem);
		
		m_arrGroupStack.push_back(m_pParentElem);
		m_pParentElem = pGroupElem;
	}

	void XMLSaveVisitor::EndVisiteGroup(const char* propName)
	{
		m_pParentElem = m_arrGroupStack.back();
		m_arrGroupStack.pop_back();
	}

	void XMLSaveVisitor::VisitProperty(bool& val,const char* propName,bool defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(char& val,const char* propName,char defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		char strVal[2] = {val,'\0'};
		pElem->SetAttribute("value",strVal);
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(short& val,const char* propName,short defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}

	void XMLSaveVisitor::VisitProperty(int& val,const char* propName,int defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(long& val,const char* propName,long defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned char& val,const char* propName,unsigned char defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}

	void XMLSaveVisitor::VisitProperty(unsigned short& val,const char* propName,unsigned short defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned int& val,const char* propName,unsigned int defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned long& val,const char* propName,unsigned long defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(float& val,const char* propName, float defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName);
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	 void XMLSaveVisitor::VisitProperty(std::string& val,const char* propName, const char* defVal)
	 {
		 TiXmlElement* pElem = new TiXmlElement(propName);
		 pElem->SetAttribute("value",val.c_str());
		 m_pParentElem->LinkEndChild(pElem);
	 }

	 void XMLSaveVisitor::VisitProperty(const char*& val,const char* propName,const char* defVal)
	 {
		 TiXmlElement* pElem = new TiXmlElement(propName);
		 pElem->SetAttribute("value",val);
		 m_pParentElem->LinkEndChild(pElem);
	 }

	  void XMLSaveVisitor::VisitProperty(NodeTransform& val,const char* propName)
	  {
		  TiXmlElement* pElem = new TiXmlElement(propName);
		  {
				TiXmlElement* pEleSacle = new TiXmlElement("m_pos");
				pElem->SetAttribute("x");
		  }
		  m_pParentElem->LinkEndChild(pElem);
	  }

	 void XMLSaveVisitor::VisitProperty(Object*& val,const char* propName,PropertyRefMode refMode)
	 {
		 if (val == NULL)
			 return;

		 TiXmlElement* pElem = new TiXmlElement(propName);
		 pElem->SetAttribute("type",val->GetClass()->GetName());
		 if (refMode == RefMode_Composite)
		 {
			 TiXmlElement* preParent = m_pParentElem;
			 m_pParentElem = pElem;

			val->TravelProperty(this);
			
			m_pParentElem = preParent;
		 }
		 else
		 {

		 }

		 m_pParentElem->LinkEndChild(pElem);
	 }
}

