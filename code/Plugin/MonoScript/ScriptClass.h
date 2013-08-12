#ifndef __MonoScriptClass_h__
#define __MonoScriptClass_h__

namespace ma
{
	class ScriptClass
	{
	public:
		ScriptClass();

		ScriptObject*			CreateScriptObject();

		void					DeleteScriptObject(ScriptObject* pScriptObject);

		void					ParseMonoClass(MonoClass* pMonoClass);

		UINT					GetClassFieldNumber() {return m_arrFields.size();}
		
		ClassField*				GetClassFieldByIndex(UINT index) {return m_arrFields[index];}

		void					SetMonoClass(MonoClass* pMonoClass);

		const char*				GetClassName() {return m_strClassName.c_str();}

		UINT					GetObjectInsNumber() {return m_arrObjectInstance.size();}

		ScriptObject*			GetObjectInsByIndex(UINT index) {return m_arrObjectInstance[index];}

	private:
		std::string					m_strClassName;

		MonoClass*					m_pMonoClas;

		std::vector<ClassField*>	m_arrFields;


		std::vector<ScriptObject*>	m_arrObjectInstance;
	};

}

#endif // __MonoScriptClass_h__

