#include "ClassField.h"

namespace ma
{
	ClassField::ClassField()
	{
		m_pMonoField = NULL;
		m_pMonoObject = NULL;
	}

	void ClassField::ParseMonoFiled(MonoClassField* pMonoField,MonoObject* pMonoObject)
	{
		m_strName = mono_field_get_name(pMonoField);
		m_eType = ToEType( mono_field_get_type(pMonoField) );

		m_pMonoField = pMonoField;
		m_pMonoObject = pMonoObject;

		if (m_eType == INT_TYPE)
		{
			m_value.m_iInt = GetInt();
		}
		else if (m_eType == FLOAT_TYPE)
		{
			m_value.m_fFloat = GetFloat();
		}
		else if (m_eType == VECTOR3_TYPE)
		{
			Vector3 vector = GetVector3();
			m_value.m_vVector3[0] = vector[0];
			m_value.m_vVector3[1] = vector[1];
			m_value.m_vVector3[2] = vector[2];
		}

		m_pMonoField = NULL;
		pMonoObject = NULL;
	}

	void ClassField::SetMonoObjecField(MonoClassField* pMonoField,MonoObject* pMonoObject)
	{
		m_pMonoField = pMonoField;
		m_pMonoObject = pMonoObject;

		if (m_eType == INT_TYPE)
		{
			SetInt(m_value.m_iInt);
		}
		else if (m_eType == FLOAT_TYPE)
		{
			SetFloat(m_value.m_fFloat);
		}
		else if (m_eType == VECTOR3_TYPE)
		{
			SetVector3(m_value.m_vVector3);
		}
	}

	float ClassField::GetFloat()
	{
		ASSERT(m_eType == FLOAT_TYPE);
		if (m_eType != FLOAT_TYPE)
			return 0;
		
		if (m_pMonoField)
		{	
			float fValue;
			mono_field_get_value(m_pMonoObject,m_pMonoField,&fValue);
			return fValue;
		}
		else
		{
			return m_value.m_fFloat;
		}
	}

	void ClassField::SetFloat(float fValue)
	{
		ASSERT(m_eType == FLOAT_TYPE);
		if (m_eType != FLOAT_TYPE)
			return ;

		m_value.m_fFloat = fValue;

		if (m_pMonoField)
		{
			mono_field_set_value(m_pMonoObject,m_pMonoField,&fValue);
		}
	}

	int	ClassField::GetInt()
	{
		ASSERT(m_eType == INT_TYPE);
		if (m_eType != INT_TYPE)
			return 0;

		if (m_pMonoField)
		{	
			int iValue;
			mono_field_get_value(m_pMonoObject,m_pMonoField,&iValue);
			return iValue;
		}
		else
		{
			return m_value.m_iInt;
		}
		
	}

	void ClassField::SetInt(int nValue)
	{
		ASSERT(m_eType == INT_TYPE);
		if (m_eType != INT_TYPE)
			return;

		m_value.m_iInt = nValue;

		if (m_pMonoField)
		{
			mono_field_set_value(m_pMonoObject,m_pMonoField,&nValue);
		}
	}

	Vector3 ClassField::GetVector3()
	{
		ASSERT(m_eType == VECTOR3_TYPE);
		if (m_eType != VECTOR3_TYPE)
			return Vector3(0,0,0);

		if (m_pMonoField == NULL)
			return m_value.m_vVector3;

		Vector3 value(0,0,0);

		ScriptSystem* pMonoDScriptevive = (ScriptSystem*)GetScriptSystem();
		if (pMonoDScriptevive == NULL)
			return value;

		MonoDomain* pDomain = pMonoDScriptevive->GetAppDomain();

		MonoObject* pMonoObject = mono_field_get_value_object(pDomain,m_pMonoField,m_pMonoObject);
		ASSERT(pMonoObject != NULL);
		if (pMonoObject == NULL)
			return value;

		MonoClass* pMonoClass = mono_object_get_class(pMonoObject);
		ASSERT(pMonoClass != NULL);
		if (pMonoClass == NULL)
			return value;

		MonoProperty *pProperty = NULL;
		void* pIter = NULL;

		while ( ( pProperty = mono_class_get_properties(pMonoClass, &pIter) ) ) 
		{	
			const char* pPropertyName = mono_property_get_name(pProperty);
			MonoObject* pResObject = mono_property_get_value(pProperty,pMonoObject,NULL,NULL);
			float fRes = *( (float*)mono_object_unbox(pResObject) );
			if ( strcmp(pPropertyName,"x") == 0 )
			{
				value.x = fRes;
			}
			else if ( strcmp(pPropertyName,"y") == 0 )
			{
				value.y = fRes;
			}
			else if ( strcmp(pPropertyName,"z") == 0 )
			{
				value.z = fRes;
			}
		}

		return value;
	}

	void ClassField::SetVector3(const Vector3& vecotr3)
	{
		ASSERT(m_eType == VECTOR3_TYPE);
		if (m_eType != VECTOR3_TYPE)
			return;	

		//m_value.m_vVector3 = vecotr3;
		m_value.m_vVector3[0] = vecotr3[0];
		m_value.m_vVector3[1] = vecotr3[1];
		m_value.m_vVector3[2] = vecotr3[2];

		if (m_pMonoField)
		{
			ScriptSystem* pMonoDScriptevive = (ScriptSystem*)GetScriptSystem();
			if (pMonoDScriptevive == NULL)
				return;

			MonoDomain* pDomain = pMonoDScriptevive->GetAppDomain();

			MonoObject* pMonoObject = mono_field_get_value_object(pDomain,m_pMonoField,m_pMonoObject);
			MonoClass* pMonoClass = mono_object_get_class(pMonoObject);
			if (!pMonoObject || !pMonoClass)
				return;

			MonoProperty *pProperty = NULL;
			void* pIter = NULL;

			while ( ( pProperty = mono_class_get_properties(pMonoClass, &pIter) ) ) 
			{	
				float fValue = 0;
				const char* pPropertyName = mono_property_get_name(pProperty);
				if ( strcmp(pPropertyName,"x") == 0 )
				{
					fValue = vecotr3.x;
				}
				else if ( strcmp(pPropertyName,"y") == 0 )
				{
					fValue = vecotr3.y;
				}
				else if ( strcmp(pPropertyName,"z") == 0 )
				{
					fValue = vecotr3.z;
				}
				void *pparams [1] = {&fValue};
				mono_property_set_value(pProperty,pMonoObject,pparams,NULL);
			}
		}
	}


	std::string ClassField::ToStringType(Type eType)
	{
		if (eType == FLOAT_TYPE)
			return "float";
		else if (eType == INT_TYPE)
			return "int";
		else if (eType == VECTOR3_TYPE)
			return "Vector3";
		else 
			return "";
	}

	ClassField::Type ClassField::ToEType(std::string stType)
	{
		if (stType == "float")
			return FLOAT_TYPE;
		else if (stType == "int")
			return INT_TYPE;
		else if (stType == "Vector3")
			return VECTOR3_TYPE;

		return NONE_TYPE;
	}

	ClassField::Type ClassField::ToEType(MonoType* pMonoType)
	{
		const char* pszTypeName  = mono_type_get_name(pMonoType);
		int nMonoType = mono_type_get_type(pMonoType);

		if (nMonoType == MONO_TYPE_I4)
		{
			return INT_TYPE;
		}
		else if (nMonoType == MONO_TYPE_R4)
		{
			return FLOAT_TYPE;
		}
		else 
		{
			if ( strcmp(pszTypeName, "Vector3") == 0 )
				return VECTOR3_TYPE;
		}
	
		ASSERT(false);
		return NONE_TYPE;
	}

	void ClassField::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_strName,"strName");

		if (sl.IsReading())
		{
			std::string strType;
			sl.Serialize(strType,"strType");
			m_eType = ToEType(strType);
		}
		else
		{
			std::string strType = ToStringType(m_eType);
			sl.Serialize(strType,"strType");
		}

		if (m_eType == INT_TYPE)
		{
			sl.Serialize(m_value.m_iInt,"value");
		}
		else if (m_eType == FLOAT_TYPE)
		{
			sl.Serialize(m_value.m_fFloat,"value");
		}
		else if (m_eType == VECTOR3_TYPE)
		{
			Vector3 vector(m_value.m_vVector3);
			sl.Serialize(vector,"value");

			if (sl.IsReading())
			{
				m_value.m_vVector3[0] = vector[0];
				m_value.m_vVector3[1] = vector[1];
				m_value.m_vVector3[2] = vector[2];
			}
		}

		sl.EndSection();
	}

	ClassField* ClassField::Clone()
	{
		ClassField* pClone = new ClassField();
		
		pClone->m_strName = this->m_strName;
		pClone->m_eType = this->m_eType;
		pClone->m_value = this->m_value;

		return pClone;

// 		XMLOutputArchive xmlout;
// 		this->Serialize(xmlout);
// 
// 		XMLInputArchive xmlin;
// 		xmlin.Open(xmlout);
// 
// 		ClassField* pClone = new ClassField();
// 		pClone->Serialize(xmlin);
// 
// 		return pClone;
	}
}

			



