#ifndef __MonoScriptDevice_h__
#define __MonoScriptDevice_h__

#include <Framework/Script/IScriptDevice.h>


namespace ma
{

	class MonoScriptAPI MonoScriptDevice : public IScriptDevice
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
// 		bool	GetBooleanField(ScriptObjectHandle hObj,Name fieldName);
// 
// 		void	SetBooleanField(ScriptObjectHandle hObj,Name fieldName,bool val);
// 
// 		int	GetIntField(ScriptObjectHandle hObj,Name fieldName);
// 
// 		void	SetIntField(ScriptObjectHandle hObj,Name fieldName,int val);
// 
// 		UINT	GetUintField(ScriptObjectHandle hObj,Name fieldName);
// 
// 		void	SetUintField(ScriptObjectHandle hObj,Name fieldName,UINT val);
// 
// 		float	GetFloatField(ScriptObjectHandle hObj,Name fieldName);
// 
// 		void	SetFloatField(ScriptObjectHandle hObj,Name fieldName,float val);
// 
// 		ScriptObjectHandle	GetObjectField(ScriptObjectHandle hObj,Name fieldName);
// 
// 		void				SetObjectField(ScriptObjectHandle hObj,Name field, ScriptObjectHandle val);
// 
// 		const char*			GetStringValue(ScriptObjectHandle hObj);
// 
// 
// 		//------------------------------------------------------------------------------
// 		//
// 		//------------------------------------------------------------------------------
// 		bool	Execute(ScriptObjectHandle hObj,Name funName);


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

