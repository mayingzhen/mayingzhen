#ifndef __MonoScriptObject_h__
#define __MonoScriptObject_h__

namespace ma
{
	class MonoScriptAPI MonoScriptObject : public ScriptObject
	{
	public:
		MonoScriptObject(const char* pClassName);

		~MonoScriptObject();

		virtual	const char* GetName();

		virtual UINT		GetFieldNumber();

		virtual const char*	GetFieldName(UINT index);

		virtual const char*	GetFieldTypeName(UINT index);

		virtual float		GetFloat(const char* pszName);

		virtual void		SetFloat(const char* pszName,float fValue);

		virtual int			GetInt(const char* pszName);

		virtual void		SetInt(const char* pszName, int nValue);

		virtual xmUint		GetUint(const char* pszName);

		virtual void		SetUint(const char* pszName, xmUint nValue);

		virtual D3DXVECTOR3	GetVector3(const char* pszName);

		virtual void		SetVector3(const char* pszName,const D3DXVECTOR3& vecotr3);

		virtual void		InvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL);

		virtual void		InvokeMethod(ScriptMethodHandle hMethod,int param_count = 0, void **params = NULL);

		virtual	ScriptMethodHandle GetScriptMethodHandle(const char* pMethodName,int param_count = 0);

		MonoObject*			GetMonoObject() {return m_pObj;}

	private:
		void DestoryScript();

		void SetupField();

		void InvokeMethod(MonoMethod* pMethod,void **params = NULL);

	private:
		MonoClass*		m_pClass;
		MonoObject*		m_pObj;
		uint32_t		m_handle;
		std::vector<MonoClassField*> m_vMonoFields;
	};
}

#endif

