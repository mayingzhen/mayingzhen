#ifndef __MonoScriptObject_h__
#define __MonoScriptObject_h__

namespace ma
{
	class ScriptClass;
	class ClassField;

	class ScriptObject : public IScriptObject
	{
	public:
		ScriptObject(ScriptClass* pScriptClass);

		~ScriptObject();

		virtual	const char*		GetName();

		virtual float			GetFloat(const char* pszName);

		virtual void			SetFloat(const char* pszName,float fValue);

		virtual int				GetInt(const char* pszName);

		virtual void			SetInt(const char* pszName, int nValue);

		virtual Vector3			GetVector3(const char* pszName);

		virtual void			SetVector3(const char* pszName,const Vector3& vecotr3);	

		virtual void			InvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL);

		void					Serialize(Serializer& sl, const char* pszLable = "ScriptObject");

		void					InitMonoObject(MonoClass* pMonoClass); 

		MonoObject*				GetMonoObject() {return m_pMonoObj;}

		ScriptClass*			GetScriptClass() {return m_pScriptClass;}

		//GameObject*				GetGameObject() {return m_pGameObj;}

		//void					SetGameObject(GameObject* pGameObj) {m_pGameObj = pGameObj;}

	private:
		void					DestoryScript();

		ClassField*				GetClassField(const char* pName);

		void					InvokeMethod(MonoMethod* pMethod,void **params = NULL);

	private:
		ScriptClass*					m_pScriptClass;
		
		MonoClass*						m_pMonoClass;
		MonoObject*						m_pMonoObj;
		uint32_t						m_handle;

		std::vector<ClassField*>		m_arrFields;

		//GameObject*						m_pGameObj;
	};



}

#endif // __MonoScriptObject_h__

