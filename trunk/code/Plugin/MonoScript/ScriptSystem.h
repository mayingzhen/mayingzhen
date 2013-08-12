#ifndef __MonoScriptSystem_h__
#define __MonoScriptSystem_h__




namespace ma
{

	class  ScriptSystem: public IScriptSystem
	{
	public:

		ScriptSystem();

		virtual void			Init();

		virtual void			Shutdown();

		virtual bool			Start();

		virtual void			Stop();

		virtual void			Update();

		virtual IScriptObject*	CreateScriptObject(const char* pszName/*,GameObject* pGameObj*/);

		virtual void			DeleteScriptObject(IScriptObject* pScriptObject);

		virtual	void			ParseScriptAll(const char* pDllName);

		void					LoadScriptDll(const char* pScriptDllName);

		MonoDomain*				GetAppDomain() {return m_pAppDomain;}

		ScriptClass*			GetScriptClass(const char* pClassName);

	private:
		//void					AllScriptObjInvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL);
	
	private:
		MonoDomain*				m_pDomain;
		MonoDomain*				m_pAppDomain;  

		// script dll
 		std::vector<std::string>	m_arrDllName;
		std::vector<ScriptClass*>	m_arrScriptClass;
		
	};

}


#endif //__MonoScriptSystem_h__

