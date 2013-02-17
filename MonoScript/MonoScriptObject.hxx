#include "MonoScript/MonoScriptObject.h"
#include "MonoScript/MonoUtil.h"

namespace ma
{
	MonoScriptObject::MonoScriptObject(const char* pClassName)
	{
		MonoScriptDevice* pScriptDevice = (MonoScriptDevice*)GetScriptDevice();

		if (pClassName == NULL)
			return;

		MonoClass* pMonoClass = pScriptDevice->GetMonoClassByName(pClassName);
		if (pMonoClass == m_pClass)
			return;

		if (pMonoClass == NULL)
		{
			DestoryScript();
			return;
		}

		m_pClass = pMonoClass;

		if (m_pObj != NULL)
		{
			mono_gchandle_free(m_handle); 
		}

		m_pObj = mono_object_new(pScriptDevice->GetAppDomain(),m_pClass);
		assert(m_pObj != NULL);
		if (m_pObj == NULL)
			return;

		m_handle = mono_gchandle_new(m_pObj,false); // ∑¿÷πm_pObj±ªgcµÙ

		mono_runtime_object_init(m_pObj);

		//SetupMethod();

		SetupField();
	}

	MonoScriptObject::~MonoScriptObject()
	{
		DestoryScript();
	}

	const char* MonoScriptObject::GetName()
	{
		return mono_class_get_name(m_pClass);
	}

	UINT MonoScriptObject::GetFieldNumber()
	{
		return m_vMonoFields.size();
	}

	const char* MonoScriptObject::GetFieldName(UINT index)
	{
		if (index < 0 || index >= GetFieldNumber() )
			return 0;
	
		MonoClassField* pField = m_vMonoFields[index];
		if (pField == NULL)
			return NULL;

		return mono_field_get_name(pField);
	}

	const char* MonoScriptObject::GetFieldTypeName(UINT index)
	{
		if (index < 0 || index >= GetFieldNumber() )
			return 0;

		MonoClassField* pField = m_vMonoFields[index];
		if (pField == NULL)
			return NULL;

		const char* pszTypeName = mono_type_get_name( mono_field_get_type(pField) );
		
		return pszTypeName;
		//return NULL != pszTypeName ? MonoUtil::MapTypeMonoToCRT(pszTypeName) : NULL;
	}

// 	int MonoScriptObject::GetFieldMonoType(UINT index)
// 	{
// 		if (index < 0 || index >= GetFieldNumber() )
// 			return 0;
// 
// 		MonoClassField* pField = m_vMonoFields[index];
// 		if (pField == NULL)
// 			return 0;
// 
// 		return mono_type_get_type( mono_field_get_type(pField) );
// 	}

	float MonoScriptObject::GetFloat(const char* pszName)
	{
		if (pszName == NULL)
			return 0;

		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			int nType = mono_type_get_type( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 && nType == MONO_TYPE_R4)
			{
				float fValue;
				mono_field_get_value(m_pObj,pClassField,&fValue);
				return fValue;
			}
		}

		return 0;
	}

	void MonoScriptObject::SetFloat(const char* pszName,float fValue)
	{
		if (pszName == NULL)
			return;

		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			int nType = mono_type_get_type( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 && nType == MONO_TYPE_R4)
			{
				mono_field_set_value(m_pObj,pClassField,&fValue);
			}
		}
	}


	int MonoScriptObject::GetInt(const char* pszName)
	{
		if (pszName == NULL)
			return 0;

		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			int nType = mono_type_get_type( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 && nType == MONO_TYPE_I4)
			{
				int iValue;
				mono_field_get_value(m_pObj,pClassField,&iValue);
				return iValue;
			}
		}	

		return 0;
	}

	void MonoScriptObject::SetInt(const char* pszName,int iValue)
	{
		if (pszName == NULL)
			return;

		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			int nType = mono_type_get_type( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 && nType == MONO_TYPE_I4)
			{
				mono_field_set_value(m_pObj,pClassField,&iValue);
			}
		}
	}


	xmUint		MonoScriptObject::GetUint(const char* pszName)
	{
		xmUint nRet = 0;
		if (pszName != NULL)
		{
			//MonoUtil::GetField(&nRet,m_pObj,pszName);
		}		
		return nRet;
	}

	void		MonoScriptObject::SetUint(const char* pszName, xmUint nValue)
	{
		if (pszName != NULL)
		{
			//MonoUtil::SetField(m_pObj,pszName,&nValue);
		}		
	}

	D3DXVECTOR3 MonoScriptObject::GetVector3(const char* pszName)
	{
		if (pszName == NULL)
			return D3DXVECTOR3(0,0,0);
		
		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			const char* pszTypeName  = mono_type_get_name( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 &&  strcmp(pszTypeName,"D3DXVECTOR3") == 0 )
			{
				return MonoUtil::MonoVector3ValueToxmValue(pClassField,m_pObj);
			}
		}

		return D3DXVECTOR3(0,0,0);
	}

	void MonoScriptObject::SetVector3(const char* pszName,const D3DXVECTOR3& vecotr3)
	{
		if (pszName == NULL)
			return ;

		for (UINT i = 0; i < m_vMonoFields.size(); ++i)
		{
			MonoClassField* pClassField = m_vMonoFields[i];
			if (pClassField == NULL)
				continue;

			const char* pszFieldName = mono_field_get_name(pClassField);
			const char* pszTypeName  = mono_type_get_name( mono_field_get_type(pClassField) );
			if ( strcmp(pszFieldName,pszName) == 0 &&  strcmp(pszTypeName,"D3DXVECTOR3") == 0 )
			{
				MonoUtil::D3DXVECTOR3ValueToMonoValue(vecotr3,pClassField,m_pObj);	
			}
		}
	}	

	void		MonoScriptObject::InvokeMethod(const char* pszMethod,int param_count,void **params)
	{
		if (pszMethod == NULL)
			return;

		MonoMethod* pMethod = mono_class_get_method_from_name(m_pClass,pszMethod,param_count);
		if (pMethod == NULL)
		{
			MonoScriptDevice* pScriptDevice = (MonoScriptDevice*)GetScriptDevice();
			MonoClass* pParentClass = pScriptDevice->GetScriptBaseClass(m_pClass);
			pMethod = mono_class_get_method_from_name(pParentClass,pszMethod,param_count);
		}
		//SSERT_MSG(pMethod != NULL,"%s:%s Not Find",GetName(),pszMethod);

		InvokeMethod(pMethod,params);
	}

	void MonoScriptObject::DestoryScript()
	{
		mono_gchandle_free(m_handle);
		m_pClass = NULL;
		m_pObj = NULL;

		m_vMonoFields.clear();
	}

	void MonoScriptObject::InvokeMethod(MonoMethod* pMethod,void **params)
	{
		if (m_pObj == NULL || pMethod == NULL)
			return;

		MonoObject* pException = NULL;
		mono_runtime_invoke(pMethod,m_pObj,params,&pException);
		if (pException)
		{
			//LogError(_ERR_INVALID_CALL, "Fail to invoke %s script %s Method", 
			//	GetName(), mono_method_get_name(pMethod) );
		}
	}

	void MonoScriptObject::InvokeMethod(ScriptMethodHandle hMethod,int param_count, void **params)
	{
		MonoMethod* pMonoMethod = (MonoMethod*)hMethod;
		InvokeMethod(pMonoMethod,params);
	}

	ScriptMethodHandle MonoScriptObject::GetScriptMethodHandle(const char* pMethodName,int param_count)
	{
		MonoMethod* pMethod = mono_class_get_method_from_name(m_pClass,pMethodName,param_count);
		if (pMethod == NULL)
		{
			MonoScriptDevice* pScriptDevice = (MonoScriptDevice*)GetScriptDevice();
			MonoClass* pParentClass = pScriptDevice->GetScriptBaseClass(m_pClass);
			pMethod = mono_class_get_method_from_name(pParentClass,pMethodName,param_count);
		}
		return pMethod;
	}

	void MonoScriptObject::SetupField()
	{
		m_vMonoFields.clear();

		MonoClass* klass = m_pClass;

		MonoClassField *field;
		void* iter = NULL;

		while ( ( field = mono_class_get_fields(klass, &iter) ) ) 
		{
			uint32_t flags = mono_field_get_flags(field); 
			if ( (flags & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) != MONO_FIELD_ATTR_PUBLIC) 
				continue;

			m_vMonoFields.push_back(field);
		}
	}
}


