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

	void XMLSaveVisitor::SaveToXML(TiXmlElement* pParent,Object* pObj,const char* propName)
	{
		m_pRootElem = pParent;
		m_pParentElem = pParent;
		VisitProperty(pObj,propName,RefMode_Composite);
	}

	void XMLSaveVisitor::BeginVisitGroup(const char* propName,const char* groupType)
	{
		TiXmlElement* pGroupElem = new TiXmlElement(propName.GetName());
		pGroupElem->SetAttribute("type",groupType.GetName());
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
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","bool");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(char& val,const char* propName,char defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","char");	
		char strVal[2] = {val,'\0'};
		pElem->SetAttribute("value",strVal);
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(short& val,const char* propName,short defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","short");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}

	void XMLSaveVisitor::VisitProperty(int& val,const char* propName,int defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","int");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(long& val,const char* propName,long defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","long");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned char& val,const char* propName,unsigned char defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","unsigned char");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}

	void XMLSaveVisitor::VisitProperty(unsigned short& val,const char* propName,unsigned short defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","unsigned short");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned int& val,const char* propName,unsigned int defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","unsigned int");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(unsigned long& val,const char* propName,unsigned long defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","unsigned long");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	void XMLSaveVisitor::VisitProperty(float& val,const char* propName, float defVal)
	{
		TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		pElem->SetAttribute("type","float");
		pElem->SetAttribute("value",StringConverter::ToString(val).c_str());
		m_pParentElem->LinkEndChild(pElem);
	}


	 void XMLSaveVisitor::VisitProperty(std::string& val,const char* propName, const char* defVal)
	 {
		 TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		 pElem->SetAttribute("type","string");
		 pElem->SetAttribute("value",val.c_str());
		 m_pParentElem->LinkEndChild(pElem);
	 }

	 void XMLSaveVisitor::VisitProperty(const char*& val,const char* propName,const char* defVal)
	 {
		 TiXmlElement* pElem = new TiXmlElement(propName.GetName());
		 pElem->SetAttribute("type","name");
		 pElem->SetAttribute("value",val.GetName());
		 m_pParentElem->LinkEndChild(pElem);
	 }

	 void XMLSaveVisitor::VisitProperty(Object*& val,const char* propName,PropertyRefMode refMode)
	 {
		 if (val != NULL)
		 {
			 TiXmlElement* pElem = new TiXmlElement(propName.GetName());
			 pElem->SetAttribute("type",val->GetClass()->GetName().GetName());
			 pElem->SetAttribute("aggregate",refMode == RefMode_Aggregate ? "true" : "false");

			 union 
			 {
				 RefID refID;
				 Uint nRefID;
			 };

			 nRefID = val->GetPropertyTag();
			 if (refID.nRefIndex != 0)
			 {
				 pElem->SetAttribute("ref_id",StringConverter::ToString(refID.nRefIndex).c_str());
			 }

			 m_pParentElem->LinkEndChild(pElem);

			 TiXmlElement* preParent = m_pParentElem;
			 m_pParentElem = pElem;

			 if (refMode == RefMode_Composite)
			 {
				 val->TravelProperty(this);
			 }

			 m_pParentElem = preParent; 
		 }
	 }
}
