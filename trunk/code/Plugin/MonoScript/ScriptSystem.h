#ifndef __MonoScriptSystem_h__
#define __MonoScriptSystem_h__




namespace ma
{

	class MonoScriptAPI ScriptSystem
	{
	public:

		ScriptSystem();

		virtual void			Init();

		virtual void			Shutdown();

		virtual bool			Start();

		virtual void			Stop();

		virtual void			Update();

		//virtual void			DeleteScriptObject(ScriptObject* pScriptObject);

		virtual	void			ParseScriptAll(const char* pDllName);

		void					LoadScriptDll(const char* pScriptDllName);

		MonoDomain*				GetAppDomain() {return m_pAppDomain;}

		ScriptClass*			GetScriptClass(const char* pClassName);
	
	private:
		MonoDomain*				m_pDomain;
		MonoDomain*				m_pAppDomain;  

		// script dll
 		std::vector<std::string>	m_arrDllName;
		std::vector<ScriptClass*>	m_arrScriptClass;
		
	};

	MonoScriptAPI ScriptSystem*	GetScriptSystem();

	MonoScriptAPI void	SetScriptSystem(ScriptSystem* pScriptSystem);

}


#endif //__MonoScriptSystem_h__

