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
		
		string strType = pszType ? pszType : "";
		string strValue = pszValue ? pszValue : "";

		m_anyValue = AnyFromString(strType,strValue);		
		
		if (m_anyValue.isEmpty())
		{
			if ( stricmp(pszType,"SamplerState") == 0 )
			{
				m_anyValue = Any( Texture::Improt(pXmlElem) );	
			}
		}
	}

	void Parameter::Export(TiXmlElement* pXmlElem)
	{
		pXmlElem->SetAttribute("Name",m_sName.c_str());

		string strType;
		string strValue;
		AnyGetString(m_anyValue,strType,strValue);

		if (!strValue.empty())
		{
			pXmlElem->SetAttribute("Type",strType.c_str());
			pXmlElem->SetAttribute("Value",strValue.c_str());
		}
		else
		{
			RefPtr<Texture> pTexuture = any_cast< RefPtr<Texture> >(m_anyValue);
			if (pTexuture != NULL)
			{
				pXmlElem->SetAttribute("Type","SamplerState");
				Texture::Export(pTexuture.get(), pXmlElem);
			}
		}
	}
}
