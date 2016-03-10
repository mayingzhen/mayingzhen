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
		m_pMonoField = pMonoField;
		m_pMonoObject = pMonoObject;
		m_strName = mono_field_get_name(pMonoField);
		MonoType* pMonoType = mono_field_get_type(pMonoField);
		const char* pszTypeName  = mono_type_get_name(pMonoType);
		int nMonoType = mono_type_get_type(pMonoType);

		if (nMonoType == MONO_TYPE_I4)
		{
			m_anyValue = Any( GetInt() );
		}
		else if (nMonoType == MONO_TYPE_R4)
		{
			m_anyValue = Any( GetFloat() );
		}
		else if ( strcmp(pszTypeName, "Vector3") == 0 )
		{
			m_anyValue = Any( GetVector3() );
		}

		m_pMonoField = NULL;
		pMonoObject = NULL;
	}

	void ClassField::SetMonoObjecField(MonoClassField* pMonoField,MonoObject* pMonoObject)
	{
		m_pMonoField = pMonoField;
		m_pMonoObject = pMonoObject;

		//std::string strType = GetStrTypeAny(m_anyValue);
		//void AnyGetString(const Any& varValue, string& strVarType,string& strVarValue)
		//Any AnyFromString(const string& strVarType,const string& strVarValue)

// 		if (strType == "int")
// 		{
// 			SetInt(any_cast<int>(m_anyValue));
// 		}
// 		else if (strType == "float")
// 		{
// 			SetFloat(any_cast<float>(m_anyValue));
// 		}
// 		else if (strType == "Vector3")
// 		{
// 			SetVector3(any_cast<Vector3>(m_anyValue));
// 		}
	}

	float ClassField::GetFloat()
	{
		if (m_pMonoField)
		{	
			float fValue;
			mono_field_get_value(m_pMonoObject,m_pMonoField,&fValue);
			return fValue;
		}
		else
		{
			return any_cast<float>(m_anyValue);
		}
	}

	void ClassField::SetFloat(float fValue)
	{
		m_anyValue = Any(fValue);

		if (m_pMonoField)
		{
			mono_field_set_value(m_pMonoObject,m_pMonoField,&fValue);
		}
	}

	int	ClassField::GetInt()
	{
		if (m_pMonoField)
		{	
			int iValue;
			mono_field_get_value(m_pMonoObject,m_pMonoField,&iValue);
			return iValue;
		}
		else
		{
			return any_cast<int>(m_anyValue);
		}
		
	}

	void ClassField::SetInt(int nValue)
	{
		m_anyValue = Any(nValue);

		if (m_pMonoField)
		{
			mono_field_set_value(m_pMonoObject,m_pMonoField,&nValue);
		}
	}

	Vector3 ClassField::GetVector3()
	{
		if (m_pMonoField == NULL)
			return any_cast<Vector3>(m_anyValue);

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
		m_anyValue = Any(vecotr3);

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

	ClassField* ClassField::Clone()
	{
		ClassField* pClone = new ClassField();
		
		pClone->m_strName = this->m_strName;
		pClone->m_anyValue = this->m_anyValue;

		return pClone;
	}
}

			



