#ifndef __MonoScriptDevice_h__
#define __MonoScriptDevice_h__




namespace ma
{

	class  ScriptDevice : public IScriptDevice
	{
	public:

		ScriptDevice();

		void					Init();

		void					Shutdown();

		virtual bool			Start();

		virtual void			Stop();

		virtual IScriptObject*	CreateScriptObject(const char* pszName);

		virtual void			DestoryScriptObject(IScriptObject* pScriptObject);

		virtual	void			ParseScriptAll(const char* pDllName);

		void					LoadScriptDll(const char* pScriptDllName);

		MonoDomain*				GetAppDomain() {return m_pAppDomain;}

		ScriptClass*			GetScriptClass(const char* pClassName);

	
	private:
		MonoDomain*				m_pDomain;
		MonoDomain*				m_pAppDomain;  


		// script dll
 		std::vector<std::string> m_arrDllName;
// 		std::vector<MonoAssembly*>	m_vAssembly; 
// 		std::vector<MonoClass*>	m_vClass;
// 		std::vector<TiXmlElement*> m_vEleClass;'

		std::vector<ScriptClass*>	m_arrScriptClass;
	

	};

}


#endif //__MonoScriptDevice_h__

