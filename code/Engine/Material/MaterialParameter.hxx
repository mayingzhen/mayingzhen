#include "MaterialParameter.h"


namespace ma
{
	Parameter::Parameter()
	{
	}

	Parameter::~Parameter()
	{	
	}

	const char* Parameter::GetName() const
	{
		return m_sName.c_str();
	}

	void Parameter::SetName(const char* pName)
	{
		m_sName = pName ? pName : "";
	}

	void Parameter::Improt(TiXmlElement* pXmlElem)
	{
		m_sName = pXmlElem->Attribute("Name");

		const char* pszType = pXmlElem->Attribute("Type");
		const char* pszValue = pXmlElem->Attribute("Value");

		m_anyValue = AnyFromString(pszType,pszValue);		
		
		if (m_anyValue.isEmpty())
		{
			if ( stricmp(pszType,"Texture") == 0 )
			{
				TiXmlElement* pXmlSamplerState = pXmlElem->FirstChildElement("SamplerState");
				m_anyValue = Any( Texture::Improt(pXmlSamplerState) );	
			}
		}
	}

	void Parameter::Export(TiXmlElement* pXmlElem)
	{
		pXmlElem->SetAttribute("Name",m_sName.c_str());

		string strType;
		string strValue;
		AnyGetString(m_anyValue,strType,strValue);

		pXmlElem->SetAttribute("Type",strType.c_str());
		pXmlElem->SetAttribute("Value",strValue.c_str());

		if (strValue.empty())
		{
			if (strType == "Texture")
			{
				TiXmlElement* pXmlSamplerState = new TiXmlElement("SamplerState");
				pXmlElem->LinkEndChild(pXmlElem);
				RefPtr<Texture> pTexture = any_cast< RefPtr<Texture> >(m_anyValue);
				Texture::Export(pTexture.get(), pXmlSamplerState);
			}
		}
	}
}
