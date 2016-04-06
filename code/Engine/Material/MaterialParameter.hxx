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

	void Parameter::Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent)
	{
		m_sName = pXmlElem->findAttribute("Name");

		const char* pszType = pXmlElem->findAttribute("Type");
		const char* pszValue = pXmlElem->findAttribute("Value");
		
		string strType = pszType ? pszType : "";
		string strValue = pszValue ? pszValue : "";

		m_anyValue = AnyFromString(strType,strValue);		
		
		if (m_anyValue.isEmpty())
		{
			if ( stricmp(pszType,"SamplerState") == 0 )
			{
				RefPtr<Texture> pTexuture = Texture::Import(pXmlElem);
				m_anyValue = Any(pTexuture);
				if (pParent)
				{
					pParent->AddRes(pTexuture.get());
				}
			}
			else if (stricmp(pszType,"UniformAnimation"))
			{
				RefPtr<UniformAnimation> pUniformAnimation = new UniformAnimation();
				pUniformAnimation->Import(pXmlElem);
				m_anyValue = Any(pUniformAnimation);
			}
		}
	}

	void Parameter::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		pXmlElem->append_attribute(doc.allocate_attribute(doc.allocate_string("Name"),doc.allocate_string(m_sName.c_str())));

		string strType;
		string strValue;
		AnyGetString(m_anyValue,strType,strValue);

		if (!strValue.empty())
		{
			pXmlElem->append_attribute(doc.allocate_attribute(doc.allocate_string("Type"),doc.allocate_string(strType.c_str())));
			pXmlElem->append_attribute(doc.allocate_attribute(doc.allocate_string("Value"),doc.allocate_string(strValue.c_str())));
		}
		else
		{
			RefPtr<Texture> pTexuture = any_cast< RefPtr<Texture> >(m_anyValue);
			if (pTexuture != NULL)
			{
				pXmlElem->append_attribute(doc.allocate_attribute(doc.allocate_string("Type"),doc.allocate_string("SamplerState")));
				Texture::Export(pTexuture.get(), pXmlElem, doc);
				return;
			}

			RefPtr<UniformAnimation> pUniformAnimation = any_cast< RefPtr<UniformAnimation> >(m_anyValue);
			if (pUniformAnimation != NULL)
			{
				pXmlElem->append_attribute(doc.allocate_attribute(doc.allocate_string("Type"),doc.allocate_string("UniformAnimation")));
				pUniformAnimation->Export(pXmlElem, doc);
				return;
			}
		}
	}
}
