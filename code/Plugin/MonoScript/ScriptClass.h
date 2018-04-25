#ifndef __MonoScriptClass_h__
#define __MonoScriptClass_h__

namespace ma
{
	class ScriptClass
	{
	public:
		ScriptClass();

		void					AddScriptObjectIns(ScriptObject* pScriptObj);

		void					RemoveScriptObjectIns(ScriptObject* pScriptObj);

		void					ParseMonoClass(MonoClass* pMonoClass);

		uint32_t				GetClassFieldNumber() {return m_arrFields.size();}
		
		ClassField*				GetClassFieldByIndex(uint32_t index) {return m_arrFields[index];}

		void					SetMonoClass(MonoClass* pMonoClass);

		const char*				GetTypeName() {return m_strClassName.c_str();}

		uint32_t				GetObjectInsNumber() {return m_arrObjectInstance.size();}

		ScriptObject*			GetObjectInsByIndex(uint32_t index) {return m_arrObjectInstance[index];}

	private:
		std::string					m_strClassName;

		MonoClass*					m_pMonoClas;

		std::vector<ClassField*>	m_arrFields;

		std::vector<ScriptObject*>	m_arrObjectInstance;
	};

}

#endif // __MonoScriptClass_h__

