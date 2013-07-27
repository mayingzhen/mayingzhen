#include "ScriptClass.h"

namespace ma
{
	ScriptClass::ScriptClass()
	{
		m_pMonoClas = NULL;
	}

	ScriptObject* ScriptClass::CreateScriptObject()
	{
		ScriptObject* pScriptObject = new ScriptObject(this);
		m_arrObjectInstance.push_back(pScriptObject);

		//pScriptObject->InitWithScriptClass(this);
		return pScriptObject;
	}

	void ScriptClass::ParseMonoClass(MonoClass* pMonoClass)
	{
		//m_pMonoClas = pMonoClass;
		m_strClassName = mono_class_get_name(pMonoClass);

		ScriptDevice* pScriptDevice = (ScriptDevice*)GetScriptDevice();

		MonoObject* pMonoObj = mono_object_new(pScriptDevice->GetAppDomain(),pMonoClass);
		ASSERT(pMonoObj != NULL);
		if (pMonoObj == NULL)
			return;

		mono_runtime_object_init(pMonoObj);

		m_arrFields.clear();
		void* iter = NULL;
		MonoClassField *pMonoField = NULL;
		while ( ( pMonoField = mono_class_get_fields(pMonoClass, &iter) ) ) 
		{
			uint32_t flags = mono_field_get_flags(pMonoField); 
			if ( (flags & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) != MONO_FIELD_ATTR_PUBLIC) 
				continue;

			ClassField* pClaaField = new ClassField();
			m_arrFields.push_back(pClaaField);
			pClaaField->ParseMonoFiled(pMonoField,pMonoObj);
		}
	}

	void ScriptClass::SetMonoClass(MonoClass* pMonoClass)
	{
		m_pMonoClas = pMonoClass;

		for (UINT i = 0; i < m_arrObjectInstance.size(); ++i)
		{
			m_arrObjectInstance[i]->InitMonoObject(m_pMonoClas);
		}
	}

}


