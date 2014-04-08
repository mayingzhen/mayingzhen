#include "ScriptSystem.h"
#include "IntenalCall.h"
#include "ScriptObject.h"
#include "ScriptClass.h"

namespace ma
{

	ScriptSystem::ScriptSystem()
	{
		m_pDomain = NULL;
		m_pAppDomain = NULL;
	}

	void ScriptSystem::Init()
	{
		std::string strLibDir = GetArchiveMananger()->GetFullPath("mono-2.11.3/lib"); 
		std::string strEtcDir = GetArchiveMananger()->GetFullPath("mono-2.11.3/etc");
		mono_set_dirs(strLibDir.c_str(), strEtcDir.c_str());

		int port = (int)(56000 + (GetCurrentProcessId() % 1000));
		char options[255];
		sprintf_s(options,255,"--debugger-agent=transport=dt_socket,address=127.0.0.1:%d,server=y,suspend=n",port);
		char* dbgArgs = options;
		mono_jit_parse_options(1,&dbgArgs);
		mono_debug_init(MONO_DEBUG_FORMAT_MONO);

		m_pDomain = mono_jit_init_version("m_pDomain","v2.11.30727");
		ASSERT(m_pDomain != NULL);

		MonoInternalInit();
	}

	void ScriptSystem::Shutdown()
	{
		mono_jit_cleanup(m_pDomain);

	}

	bool ScriptSystem::Start()
	{
		if (m_pAppDomain != NULL)
			return false;

		m_pAppDomain = mono_domain_create_appdomain("m_pAppDomain",NULL);
		ASSERT(m_pAppDomain !=  NULL);
		if (m_pAppDomain == NULL)
			return false;

		mono_domain_set(m_pAppDomain,0);

		for (UINT i = 0; i < m_arrDllName.size(); ++i)
		{
			LoadScriptDll(m_arrDllName[i].c_str());
		}


		for (UINT iClass = 0; iClass < m_arrScriptClass.size(); ++iClass)
		{
			ScriptClass* pScriptClass = m_arrScriptClass[iClass];
			if (pScriptClass == NULL)
				continue;
			
			UINT nScrpitObj = pScriptClass->GetObjectInsNumber();
			for (UINT iObj = 0; iObj < nScrpitObj; ++iObj)
			{
				ScriptObject* pScriptObject = pScriptClass->GetObjectInsByIndex(iObj);
				if (pScriptObject == NULL)
					continue;

				pScriptObject->Start();
			}
		}

		return true;
	}

	void ScriptSystem::Stop()
	{
		for (UINT iClass = 0; iClass < m_arrScriptClass.size(); ++iClass)
		{
			ScriptClass* pScriptClass = m_arrScriptClass[iClass];
			if (pScriptClass == NULL)
				continue;

			UINT nScrpitObj = pScriptClass->GetObjectInsNumber();
			for (UINT iObj = 0; iObj < nScrpitObj; ++iObj)
			{
				ScriptObject* pScriptObject = pScriptClass->GetObjectInsByIndex(iObj);
				if (pScriptObject == NULL)
					continue;

				pScriptObject->Stop();
			}
		}


		m_arrDllName.clear();

		for (UINT i = 0; i < m_arrScriptClass.size(); ++i)
		{
			SAFE_DELETE(m_arrScriptClass[i]);
		}
		m_arrScriptClass.clear();

		mono_domain_set(m_pDomain,10000);

		if (NULL != m_pAppDomain)
		{
			mono_domain_unload(m_pAppDomain);

			m_pAppDomain = NULL;
		}
	}

	void ScriptSystem::Update()
	{
		for (UINT iClass = 0; iClass < m_arrScriptClass.size(); ++iClass)
		{
			ScriptClass* pScriptClass = m_arrScriptClass[iClass];
			if (pScriptClass == NULL)
				continue;

			UINT nScrpitObj = pScriptClass->GetObjectInsNumber();
			for (UINT iObj = 0; iObj < nScrpitObj; ++iObj)
			{
				ScriptObject* pScriptObject = pScriptClass->GetObjectInsByIndex(iObj);
				if (pScriptObject == NULL)
					continue;

				pScriptObject->Update();
			}
		}

	}

	IScriptObject* ScriptSystem::CreateScriptObject(GameObject* pGameObj)
	{
		ScriptObject* pScriptObj = new ScriptObject(pGameObj);

		return pScriptObj;
	}

	void ScriptSystem::DeleteScriptObject(IScriptObject* pScriptObject)
	{
		ScriptObject* pTemp = (ScriptObject*)pScriptObject;

		ScriptClass* pScriptClass = pTemp->GetScriptClass();
		pScriptClass->RemoveScriptObjectIns(pTemp);
	}

	// Parse dll 
	void ScriptSystem::ParseScriptAll(const char* pScriptDllName)
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

		m_arrDllName.push_back(pScriptDllName);

		const MonoTableInfo* pTableInfo = mono_image_get_table_info (pImage, MONO_TABLE_TYPEDEF);
		int nTableRow = mono_table_info_get_rows(pTableInfo);
		for (int i = 1; i < nTableRow; ++i)  // i = 0 ==> <Module> type
		{
			MonoClass* pClass = mono_class_get (pImage, MONO_TOKEN_TYPE_DEF | (i + 1));
			if (pClass == NULL)
				continue;
			
			const char* pClassName = mono_class_get_name(pClass);

			MonoClass* pParentClass = mono_class_get_parent(pClass);
			ASSERT(pParentClass != NULL);
			if (pParentClass == NULL)
				continue;

			const char* pParenClassName = mono_class_get_name(pParentClass);
			bool bScriptObject = strcmp(pParenClassName, "ScriptBase") == 0;
			if (!bScriptObject)
				continue;			
			
			ScriptClass* pScriptClass = new ScriptClass();
			m_arrScriptClass.push_back(pScriptClass);
			pScriptClass->ParseMonoClass(pClass);
		}

		mono_domain_set(m_pDomain,10000);
		mono_domain_unload(m_pAppDomain);
		m_pAppDomain = NULL;
	}


	void ScriptSystem::LoadScriptDll(const char* pScriptDllName)
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

			const char* pClassName = mono_class_get_name(pClass);
			ScriptClass* pScriptClass = GetScriptClass(pClassName);
			if (pScriptClass)
			{
				pScriptClass->SetMonoClass(pClass);
			}
		}
	}

	ScriptClass* ScriptSystem::GetScriptClass(const char* pClassName)
	{
		ASSERT(pClassName);
		if (pClassName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrScriptClass.size(); ++i)
		{
			if ( strcmp( pClassName, m_arrScriptClass[i]->GetClassName() ) == 0 )
				return m_arrScriptClass[i];
		}

		return NULL;
	}

}

