#ifndef __MonoScriptSystem_h__
#define __MonoScriptSystem_h__




namespace ma
{

	class ScriptSystem
	{
	public:

		ScriptSystem();

		virtual void			Init();

		virtual void			ShutDown();

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

	ScriptSystem*	GetScriptSystem();

	void	SetScriptSystem(ScriptSystem* pScriptSystem);

}


#endif //__MonoScriptSystem_h__

