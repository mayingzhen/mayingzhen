#include "ScriptObject.h"
#include "ClassField.h"
#include "ScriptClass.h"
#include "ScriptSystem.h"

namespace ma
{
	ScriptObject::ScriptObject(GameObject* pGameObject)
		:IScriptObject(pGameObject)
	{
		m_pScriptClass = NULL;
		m_pMonoClass = NULL;
		m_pMonoObj = NULL;
		m_handle = -1;	
	}


	ScriptObject::~ScriptObject()
	{
		DestoryScript();

// 		if (m_pScriptClass)
// 			m_pScriptClass->RemoveScriptObjectIns(this);
	}

	void ScriptObject::Start()
	{
		void* params[1];
		params[0] = &m_pGameObject;
		InvokeMethod("SetGameObjPtr",1,params);
		 
		InvokeMethod("Start");
	}

	void ScriptObject::Stop()
	{
		InvokeMethod("Stop");
	}

	void ScriptObject::Update()
	{
		InvokeMethod("Update");
	}

	float ScriptObject::GetFloat(const char* pszName)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			return pField->GetFloat();

		return 0;
	}	

	void ScriptObject::SetFloat(const char* pszName,float fValue)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			pField->SetFloat(fValue);

	}

	int	ScriptObject::GetInt(const char* pszName)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			return pField->GetInt();

		return 0;
	}

	void ScriptObject::SetInt(const char* pszName, int nValue)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			pField->SetInt(nValue);
	}

	Vector3 ScriptObject::GetVector3(const char* pszName)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			return pField->GetVector3();

		return Vector3(0,0,0);
	}

	void ScriptObject::SetVector3(const char* pszName,const Vector3& vecotr3)
	{
		ClassField* pField = GetClassField(pszName);
		ASSERT(pField);
		if (pField)
			pField->SetVector3(vecotr3);
	}

	const char* ScriptObject::GetName()
	{
		ASSERT(m_pScriptClass);
		return m_pScriptClass->GetClassName();
	}

	void ScriptObject::SetName(const char* pName)
	{
		ASSERT(pName);
		if (pName == NULL)
			return;

		if (m_pScriptClass)
		{
			m_pScriptClass->RemoveScriptObjectIns(this);
		}

		ScriptSystem* pScriptSystem = (ScriptSystem*)GetScriptSystem();

		m_pScriptClass = pScriptSystem->GetScriptClass(pName);
		ASSERT(m_pScriptClass);
		if (m_pScriptClass == NULL)
			return;

		UINT nFieldNumber = m_pScriptClass->GetClassFieldNumber();
		for (UINT i = 0; i < nFieldNumber; ++i)
		{
			ClassField* pField = m_pScriptClass->GetClassFieldByIndex(i);
			ClassField* pFieldInstace = pField->Clone();
			m_arrFields.push_back(pFieldInstace);
		}

		m_pScriptClass->AddScriptObjectIns(this);
	}

	void ScriptObject::InvokeMethod(const char* pszMethod,int param_count,void **params)
	{
		if (pszMethod == NULL)
			return;

		MonoMethod* pMethod = mono_class_get_method_from_name(m_pMonoClass,pszMethod,param_count);
		if (pMethod == NULL)
		{
			MonoClass* pParentClass = mono_class_get_parent(m_pMonoClass);
			if (pParentClass == NULL)
				return;

			pMethod = mono_class_get_method_from_name(pParentClass,pszMethod,param_count);
		}

		if (pMethod == NULL)
			return;
		
		//SSERT_MSG(pMethod != NULL,"%s:%s Not Find",GetName(),pszMethod);

		InvokeMethod(pMethod,params);
	}

	void ScriptObject::DestoryScript()
	{
		mono_gchandle_free(m_handle);
		m_pMonoClass = NULL;
		m_pMonoObj = NULL;

		m_arrFields.clear();
	}

	void ScriptObject::InvokeMethod(MonoMethod* pMethod,void **params)
	{
		if (m_pMonoObj == NULL || pMethod == NULL)
			return;

		MonoObject* pException = NULL;
		mono_runtime_invoke(pMethod,m_pMonoObj,params,&pException);
		if (pException)
		{
			MonoObject *other = NULL;
			MonoString *str = mono_object_to_string (pException, &other);
			char *msg = mono_string_to_utf8 (str);
			ASSERT(false);
			mono_free(msg);
		}
	}

	ClassField*	ScriptObject::GetClassField(const char* pName)
	{
		for (UINT i = 0; i < m_arrFields.size(); ++i)
		{
			if ( strcmp( pName,m_arrFields[i]->GetFieldName() ) == 0 )
				return m_arrFields[i];
		}

		return NULL;
	}

	void ScriptObject::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_arrFields);

		sl.EndSection();
	}

	void ScriptObject::InitMonoObject(MonoClass* pMonoClass)
	{
		ScriptSystem* pScriptSystem= (ScriptSystem*)GetScriptSystem();

		if (pMonoClass == NULL)
		{
			DestoryScript();
			return;
		}

		m_pMonoClass = pMonoClass;

		ASSERT(m_pMonoObj == NULL);
		if (m_pMonoObj != NULL)
		{
			mono_gchandle_free(m_handle); 
		}

		m_pMonoObj = mono_object_new(pScriptSystem->GetAppDomain(),m_pMonoClass);
		ASSERT(m_pMonoObj != NULL);
		if (m_pMonoObj == NULL)
			return;

		m_handle = mono_gchandle_new(m_pMonoObj,false); // ∑¿÷πm_pObj±ªgcµÙ

		mono_runtime_object_init(m_pMonoObj);

		for (UINT i = 0; i < m_arrFields.size(); ++i)
		{
			ClassField* pField = m_arrFields[i];
			if (pField == NULL)
				continue;

			const char* pFieldName = pField->GetFieldName();
			MonoClassField* pMonoField = mono_class_get_field_from_name(m_pMonoClass,pFieldName);
			m_arrFields[i]->SetMonoObjecField(pMonoField, m_pMonoObj);
		}
	}

}


