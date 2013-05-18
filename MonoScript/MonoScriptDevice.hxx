#include <MonoScript/MonoScriptDevice.h>
#include <MonoScript/MonoScriptIntenal.h>
#include <MonoScript/MonoScriptObject.h>
#include <MonoScript/MonoScriptIntenal.h>
#include <MonoScript/MonoUtil.h>

namespace ma
{

	MonoScriptDevice::MonoScriptDevice()
	{
		m_pDomain = NULL;
		m_pAppDomain = NULL;
	}

	void	MonoScriptDevice::Init()
	{
		mono_set_dirs("../Data/mono-2.11.3/lib/", "../Data/mono-2.11.3/etc/");

		int port = (int)(56000 + (GetCurrentProcessId() % 1000));
		char options[255];
		sprintf_s(options,255,"--debugger-agent=transport=dt_socket,address=127.0.0.1:%d,server=y,suspend=n",port);
		//sprintf_s(options,255,"--debugger-agent=transport=dt_socket,address=127.0.0.1:%d,server=y",port);
		char* dbgArgs = options;

		mono_jit_parse_options(1,&dbgArgs);
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);

		m_pDomain = mono_jit_init_version("m_pDomain","v2.11.30727");

		MonoInternalInit();
		ASSERT(m_pDomain != NULL);
	}

	void	MonoScriptDevice::Shutdown()
	{
		mono_jit_cleanup(m_pDomain);

	}

	bool	MonoScriptDevice::Start()
	{
		if (m_pAppDomain != NULL)
			return false;

		m_pAppDomain = mono_domain_create_appdomain("m_pAppDomain",NULL);
		ASSERT(m_pAppDomain !=  NULL);
		if (m_pAppDomain == NULL)
			return false;

		mono_domain_set(m_pAppDomain,0);

		for (UINT i = 0; i < m_vDllName.size(); ++i)
		{
			LoadScriptDll(m_vDllName[i].c_str());
		}

		return true;
	}

	void	MonoScriptDevice::Stop()
	{
		mono_domain_set(m_pDomain,10000);

		if (NULL != m_pAppDomain)
		{
			mono_domain_unload(m_pAppDomain);

			m_pAppDomain = NULL;
			m_vAssembly.clear();
			m_vClass.clear();
		}

	}

	ScriptObject* MonoScriptDevice::CreateScriptObject(const char* pszName)
	{
		return new MonoScriptObject(pszName);
	}

	void		  MonoScriptDevice::DestoryScriptObject(ScriptObject* pScriptObject)
	{
		SAFE_DELETE(pScriptObject);
	}

// 	bool	MonoScriptDevice::GetBooleanField(ScriptObjectHandle hObj,Name fieldName)
// 	{
// 		bool ret = false;
// 		MonoUtil::GetField(&ret,(MonoObject*)hObj,fieldName);
// 		return ret;
// 	}
// 
// 
// 	void	MonoScriptDevice::SetBooleanField(ScriptObjectHandle hObj,Name fieldName,bool val)
// 	{
// 		MonoUtil::SetField((MonoObject*)hObj,fieldName,&val);
// 	}
// 
// 
// 	int	MonoScriptDevice::GetIntField(ScriptObjectHandle hObj,Name fieldName)
// 	{
// 		int ret = 0;
// 		MonoUtil::GetField(&ret,(MonoObject*)hObj,fieldName);
// 		return ret;
// 	}
// 
// 
// 	void	MonoScriptDevice::SetIntField(ScriptObjectHandle hObj,Name fieldName,int val)
// 	{
// 		MonoUtil::SetField((MonoObject*)hObj,fieldName,&val);
// 	}
// 
// 	UINT	MonoScriptDevice::GetUintField(ScriptObjectHandle hObj,Name fieldName)
// 	{
// 		UINT ret = 0;
// 		MonoUtil::GetField(&ret,(MonoObject*)hObj,fieldName);
// 		return ret;
// 	}
// 
// 
// 	void	MonoScriptDevice::SetUintField(ScriptObjectHandle hObj,Name fieldName,UINT val)
// 	{
// 		MonoUtil::SetField((MonoObject*)hObj,fieldName,&val);
// 	}
// 
// 
// 
// 
// 	float	MonoScriptDevice::GetFloatField(ScriptObjectHandle hObj,Name fieldName)
// 	{
// 		float ret = 0.0f;
// 		MonoUtil::GetField(&ret,(MonoObject*)hObj,fieldName);
// 		return ret;
// 	}
// 
// 
// 	void	MonoScriptDevice::SetFloatField(ScriptObjectHandle hObj,Name fieldName,float val)
// 	{
// 		MonoUtil::SetField((MonoObject*)hObj,fieldName,&val);
// 	}
// 
// 
// 	ScriptObjectHandle	MonoScriptDevice::GetObjectField(ScriptObjectHandle hObj,Name fieldName)
// 	{
// 		return 0;
// 	}
// 

// 	void				MonoScriptDevice::SetObjectField(ScriptObjectHandle hObj,Name field, ScriptObjectHandle val)
// 	{
// 
// 	}

// 	const char*		MonoScriptDevice::GetStringValue(ScriptObjectHandle hObj)
// 	{
// 		MonoString* pMonoString = (MonoString*)hObj;
// 		return mono_string_to_utf8(pMonoString);
// 
// 	}


// 	bool	MonoScriptDevice::Execute(ScriptObjectHandle hObj,Name funName)
// 	{
// 		
// 		MonoObject* pException = NULL;
// 
// 		MonoObject* pObjM	= (MonoObject*)(hObj);
// 		MonoClass* pClsM	= NULL != pObjM ? mono_object_get_class(pObjM) : NULL;
// 		const char* pszFunName = funName.GetName();
// 		MonoMethod* pMethod = NULL != pClsM ? mono_class_get_method_from_name(pClsM,pszFunName,0) : NULL;
// 
// 		if (NULL != pMethod)
// 		{
// 			mono_runtime_invoke(pMethod,pObjM,NULL,&pException);
// 
// 			return true;
// 		}
// 
// 		return false;
// 	}

	void	MonoScriptDevice::ParseScriptDll(const char* pScriptDllName)
	{
		if (pScriptDllName == NULL)
			return;

		if (m_pAppDomain == NULL)
		{
			m_pAppDomain = mono_domain_create_appdomain("m_pAppDomain",NULL);
			ASSERT(m_pAppDomain !=  NULL);
			if (m_pAppDomain == NULL)
				return;

			mono_domain_set(m_pAppDomain,0);
		}

		MonoAssembly* pAssembly = mono_domain_assembly_open (m_pAppDomain,pScriptDllName);
		ASSERT(pAssembly != NULL);
		if (pAssembly == NULL)
			return;

		MonoImage* pImage = mono_assembly_get_image(pAssembly);
		ASSERT(pImage != NULL);
		if (pImage == NULL)
			return;

		m_vDllName.push_back(pScriptDllName);

		const MonoTableInfo* pTableInfo = mono_image_get_table_info (pImage, MONO_TABLE_TYPEDEF);
		int nTableRow = mono_table_info_get_rows(pTableInfo);
		for (int i = 1; i < nTableRow; ++i)  // i = 0 ==> <Module> type
		{
			MonoClass* pClass = mono_class_get (pImage, MONO_TOKEN_TYPE_DEF | (i + 1));
			if (pClass == NULL)
				continue;

			MonoClass* pParentClass = GetScriptBaseClass(pClass);
			if (pParentClass == NULL)
				continue;

			const char* pszClassName = mono_class_get_name(pClass);

			if ( GetXmlClassByName(pszClassName) ) 
				continue;

			MonoObject* pObj = mono_object_new(m_pAppDomain,pClass);
			if (pObj == NULL)
				continue;

			mono_runtime_object_init(pObj);

			TiXmlElement* pEleClass = new TiXmlElement(pszClassName);
			pEleClass->SetAttribute("className",pszClassName);
			pEleClass->SetAttribute("dllName",pScriptDllName);
			m_vEleClass.push_back(pEleClass);

			MonoClassField *field;
			void* iter = NULL;

			while ( ( field = mono_class_get_fields(pClass, &iter) ) ) 
			{
				uint32_t flags = mono_field_get_flags(field); 
				if ( (flags & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) != MONO_FIELD_ATTR_PUBLIC) 
					continue;

				// to l
				const char* pName = mono_field_get_name(field);
				MonoType* pType = mono_field_get_type(field);
				int nType = mono_type_get_type(pType);
				const char* pTypeName = mono_type_get_name(pType);

				TiXmlElement* pEleField = new TiXmlElement("EdField");
				pEleField->SetAttribute("name",pName);
				//pEleField->SetAttribute("nType",nType);
				pEleField->SetAttribute("sTypeName",pTypeName);

				if (nType == MONO_TYPE_R4)
				{
					float fValue;
					mono_field_get_value(pObj,field,&fValue);
					pEleField->SetDoubleAttribute("value",fValue);
					pEleField->SetAttribute("sTypeName","float");
				}
				else if (nType == MONO_TYPE_I4)
				{
					int nValue;
					mono_field_get_value(pObj,field,&nValue);
					pEleField->SetAttribute("value",nValue);
					pEleField->SetAttribute("sTypeName","int");
				}
				else if (nType == MONO_TYPE_U4)
				{
					UINT nValue;
					mono_field_get_value(pObj,field,&nValue);
					pEleField->SetAttribute("value",nValue);
					pEleField->SetAttribute("sTypeName","uint");
				}
				else 
				{
// 					if ( strcmp(pTypeName,"Vector3") == 0 )
// 					{
// 						Vector3 vector3 = MonoUtil::MonoVector3ValueToValue(field,pObj);
// 						TiXmlElement* pEleVECTOR3 = SaveVECTOR3(vector3,"Vector3");
// 						pEleField->LinkEndChild(pEleVECTOR3);
// 					}
				}

				pEleClass->LinkEndChild(pEleField);
			}

		}

		mono_domain_set(m_pDomain,10000);
		mono_domain_unload(m_pAppDomain);
		m_pAppDomain = NULL;


	}


	void	MonoScriptDevice::UnParseScriptDll(const char* pScriptDllName)
	{
		if (pScriptDllName == NULL)
			return;

		std::vector<TiXmlElement*>::iterator it = m_vEleClass.begin();
		while (it != m_vEleClass.end())
		{
			TiXmlElement* pEleClass = *it;
			if (pEleClass == NULL)
				continue;

			if (strcmp(pScriptDllName,pEleClass->Attribute("dllName")) == 0)
			{
				it = m_vEleClass.erase(it);
			}
			else
			{
				++it;
			}
		}
	}


	void	MonoScriptDevice::LoadScriptDll(const char* pScriptDllName)
	{
		if (pScriptDllName == NULL)
			return;

		ASSERT(m_pAppDomain != NULL);
		if (m_pAppDomain == NULL)
			return;

		MonoAssembly* pAssembly = mono_domain_assembly_open(m_pAppDomain,pScriptDllName);
		ASSERT(pAssembly != NULL);
		if (pAssembly == NULL)
			return;

		MonoImage* pImage = mono_assembly_get_image(pAssembly);
		ASSERT(pImage != NULL);
		if (pImage == NULL)
			return;

		const MonoTableInfo* pTableInfo = mono_image_get_table_info (pImage, MONO_TABLE_TYPEDEF);
		int nTableRow = mono_table_info_get_rows(pTableInfo);
		for (int i = 1; i < nTableRow; ++i)  // i = 0 ==> <Module> type
		{
			MonoClass* pClass = mono_class_get(pImage, MONO_TOKEN_TYPE_DEF | (i + 1));
			if (pClass == NULL)
				continue;

			MonoClass* pParentClass = GetScriptBaseClass(pClass);
			if (pParentClass == NULL)
				continue;

			const char* pszClassName = mono_class_get_name(pClass);
			if ( GetMonoClassByName(pszClassName) )
				continue;

			m_vClass.push_back(pClass);
		}

		ASSERT(m_vClass.size() == m_vEleClass.size());
	}

	UINT	MonoScriptDevice::GetScriptNumber()
	{
		if (m_pAppDomain == NULL)
		{
			return m_vEleClass.size();
		}
		else 
		{
			return m_vClass.size();
		}
	}

	const char* MonoScriptDevice::GetScriptNameByIndex(UINT index)
	{
		if ( index < 0 || index >= GetScriptNumber() )
			return NULL;

		if (m_pAppDomain == NULL)
		{
			TiXmlElement* pEleClass = m_vEleClass[index];
			return pEleClass->Attribute("className");
		}
		else 
		{
			MonoClass* pMonoClass = m_vClass[index];
			return mono_class_get_name(pMonoClass);
		}

		return NULL;
	}

	MonoClass*	MonoScriptDevice::GetMonoClassByName(const char* pClassName)
	{
		for (UINT i = 0; i < m_vClass.size(); ++i)
		{
			if (m_vClass[i] == NULL)
				continue;

			const char* pszClassName = mono_class_get_name(m_vClass[i]);
			if (strcmp(pszClassName, pClassName) == 0)
				return m_vClass[i];
		}

		return NULL;
	}

	TiXmlElement* MonoScriptDevice::GetXmlClassByName(const char* pClassName)
	{
		if (pClassName == NULL)
			return NULL;

		for (UINT i = 0; i < m_vEleClass.size(); ++i)
		{
			if (m_vEleClass[i] == NULL)
				continue;

			const char* pszClassName = m_vEleClass[i]->Attribute("className");
			if (pszClassName == NULL)
				continue;

			if (strcmp(pszClassName, pClassName) == 0)
				return m_vEleClass[i];
		}

		return NULL;
	}


	MonoClass* MonoScriptDevice::GetScriptBaseClass(MonoClass* pMonoClass)
	{
		if (pMonoClass == NULL)
			return NULL;

		// base class
		MonoClass* pParentClass = mono_class_get_parent(pMonoClass);
		ASSERT(pParentClass != NULL);
		if (pParentClass == NULL)
			return NULL;

		const char* pParenClassName = mono_class_get_name(pParentClass);
		bool bScriptObject = strcmp(pParenClassName, "ScriptObject") == 0;
		if (!bScriptObject)
			return NULL;

		return pParentClass;
	}

}

