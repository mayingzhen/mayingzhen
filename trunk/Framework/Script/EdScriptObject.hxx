#include "Framework/Script/EdScriptObject.h"


namespace ma
{
	EdScriptObject::EdScriptObject(TiXmlElement* pEleClass):m_pXmlClass(NULL)
	{
		if (pEleClass == NULL)
			return;

		m_pXmlClass = (TiXmlElement*)pEleClass->Clone();
		TiXmlElement* pEleField = m_pXmlClass->FirstChildElement();
		while(pEleField)
		{
			m_vXmlFields.push_back(pEleField);

			pEleField = pEleField->NextSiblingElement();
		}
	}

	const char* EdScriptObject::GetName()
	{
		return m_pXmlClass->Attribute("className");
	}

	UINT EdScriptObject::GetFieldNumber()
	{
		return m_vXmlFields.size(); 
	}

	const char* EdScriptObject::GetFieldName(UINT index)
	{
		if (index < 0 || index >= GetFieldNumber() )
			return NULL;

		TiXmlElement* pEleField = m_vXmlFields[index];
		if (pEleField == NULL)
			return NULL;

		return pEleField->Attribute("name");
	}

	const char* EdScriptObject::GetFieldTypeName(UINT index)
	{
		if (index < 0 || index >= GetFieldNumber() )
			return 0;

		TiXmlElement* pEleField = m_vXmlFields[index];
		if (pEleField == NULL)
			return NULL;

		return pEleField->Attribute("sTypeName");
	}

	float EdScriptObject::GetFloat(const char* pszName)
	{
		if (pszName == NULL)
			return 0;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"float") == 0 )
			{
				const char* pszValue = pEleField->Attribute("value");
				return (float)atof(pszValue);
			}
		}

		return 0;
	}

	void EdScriptObject::SetFloat(const char* pszName,float fValue)
	{
		if (pszName == NULL)
			return;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"float") == 0 )
			{
				pEleField->SetDoubleAttribute("value",fValue);
			}
		}
	}

	int EdScriptObject::GetInt(const char* pszName)
	{
		if (pszName == NULL)
			return 0;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
					continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"int") == 0)
			{
				const char* pszValue = pEleField->Attribute("value");
				return atoi(pszValue);
			}
		}

		return 0;
	}

	void EdScriptObject::SetInt(const char* pszName,int iValue)
	{
		if (pszName == NULL)
			return;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"int") == 0)
			{
				pEleField->SetAttribute("value",iValue);
			}
		}
	}

	xmUint			EdScriptObject::GetUint(const char* pszName)
	{
		if (pszName == NULL)
			return 0;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"uint") == 0)
			{
				const char* pszValue = pEleField->Attribute("value");
				int iVal = atoi(pszValue);
				return (xmUint)iVal;
			}
		}

		return 0;
	}

	void		EdScriptObject::SetUint(const char* pszName, xmUint nValue)
	{
		if (pszName == NULL)
			return;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"uint") == 0)
			{
				pEleField->SetAttribute("value",(int)nValue);
			}
		}
	}

	D3DXVECTOR3 EdScriptObject::GetVector3(const char* pszName)
	{
		if (pszName == NULL)
			return D3DXVECTOR3();

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"D3DXVECTOR3") == 0 )
			{
				TiXmlElement* pEleValue = pEleField->FirstChildElement("D3DXVECTOR3");
				//return ParseD3DXVECTOR3(pEleValue);
			}
		}	

		return D3DXVECTOR3();
	}

	void EdScriptObject::SetVector3(const char* pszName,const D3DXVECTOR3& vecotr3)
	{
		if (pszName == NULL)
			return ;

		for (UINT i = 0; i < m_vXmlFields.size(); ++i)
		{
			TiXmlElement* pEleField = m_vXmlFields[i];
			if (pEleField == NULL)
				continue;

			const char* pszEleName = pEleField->Attribute("name");
			const char* pszTypeName = pEleField->Attribute("sTypeName");
			if ( strcmp(pszEleName,pszName) == 0 && strcmp(pszTypeName,"D3DXVECTOR3") == 0 )
			{
				TiXmlElement* pEleValue =  pEleField->FirstChildElement("D3DXVECTOR3");
				if (pEleValue != NULL)
				{
					pEleValue->SetDoubleAttribute("x",vecotr3.x);
					pEleValue->SetDoubleAttribute("y",vecotr3.y);
					pEleValue->SetDoubleAttribute("z",vecotr3.z);
				}
			}
		}
	}	
}



