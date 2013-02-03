#ifndef __MonoScriptDevice_h__
#define __MonoScriptDevice_h__

#include <Framework/Script/IScriptDevice.h>


namespace ma
{

	class MonoScriptAPI MonoScriptDevice : public ScriptDevice
	{
		

		MonoDomain*		m_pDomain;
		MonoDomain*		m_pAppDomain;  


		// script dll
		std::vector<std::string> m_vDllName;
		std::vector<MonoAssembly*>	m_vAssembly; 
		std::vector<MonoClass*>	m_vClass;
		std::vector<TiXmlElement*> m_vEleClass;

	public:

		MonoScriptDevice();

		void	Init();

		void	Shutdown();

		bool	Start();

		void	Stop();

		ScriptObject* CreateScriptObject(const char* pszName);

		void		  DestoryScriptObject(ScriptObject* pScriptObject);

		//------------------------------------------------------------------------------
		//Script Object Data Access
		//------------------------------------------------------------------------------
// 		xmBool	GetBooleanField(ScriptObjectHandle hObj,S3AName fieldName);
// 
// 		void	SetBooleanField(ScriptObjectHandle hObj,S3AName fieldName,xmBool val);
// 
// 		xmInt	GetIntField(ScriptObjectHandle hObj,S3AName fieldName);
// 
// 		void	SetIntField(ScriptObjectHandle hObj,S3AName fieldName,xmInt val);
// 
// 		xmUint	GetUintField(ScriptObjectHandle hObj,S3AName fieldName);
// 
// 		void	SetUintField(ScriptObjectHandle hObj,S3AName fieldName,xmUint val);
// 
// 		xmFloat	GetFloatField(ScriptObjectHandle hObj,S3AName fieldName);
// 
// 		void	SetFloatField(ScriptObjectHandle hObj,S3AName fieldName,xmFloat val);
// 
// 		ScriptObjectHandle	GetObjectField(ScriptObjectHandle hObj,S3AName fieldName);
// 
// 		void				SetObjectField(ScriptObjectHandle hObj,S3AName field, ScriptObjectHandle val);
// 
// 		const char*			GetStringValue(ScriptObjectHandle hObj);
// 
// 
// 		//------------------------------------------------------------------------------
// 		//
// 		//------------------------------------------------------------------------------
// 		bool	Execute(ScriptObjectHandle hObj,S3AName funName);


		//------------------------------------------------------------------------------
		//Implementation
		//------------------------------------------------------------------------------

		void	LoadScriptDll(const char* pScriptDllName);

		void	ParseScriptDll(const char* pScriptDllName);

		void	UnParseScriptDll(const char* pScriptDllName);

		UINT	GetScriptNumber();

		const char* GetScriptNameByIndex(UINT index);

		//IScriptObject* GetScriptObjectByName(const char* pScriptName);

		MonoDomain*	GetAppDomain() {return m_pAppDomain;}

		MonoClass*	GetMonoClassByName(const char* pClassName);

		TiXmlElement* GetXmlClassByName(const char* pClassName);

		MonoClass* GetScriptBaseClass(MonoClass* pMonoClass); // return ScriptObject class

	};

}


#endif //__MonoScriptDevice_h__

