#ifndef __ScriptObject_h__
#define __ScriptObject_h__

namespace ma
{
	class ENGINE_API ScriptObject
	{
	public:
		virtual	const char* GetName() = 0;

		virtual UINT		GetFieldNumber() = 0;

		virtual const char* GetFieldName(UINT index) = 0;

		virtual const char*	GetFieldTypeName(UINT index) = 0;

		virtual float		GetFloat(const char* pszName) = 0;

		virtual void		SetFloat(const char* pszName,float fValue) = 0;

		virtual int			GetInt(const char* pszName) = 0;

		virtual void		SetInt(const char* pszName, int nValue) = 0;

		virtual UINT		GetUint(const char* pszName) = 0; 

		virtual void		SetUint(const char* pszName, UINT nValue) = 0;

		virtual D3DXVECTOR3	GetVector3(const char* pszName) = 0;

		virtual void		SetVector3(const char* pszName,const D3DXVECTOR3& vecotr3) = 0;	

		virtual void		InvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL) = 0;

		virtual void		InvokeMethod(ScriptMethodHandle hMethod,int param_count = 0, void **params = NULL) = 0;

		virtual	ScriptMethodHandle GetScriptMethodHandle(const char* pMethodName,int param_count = 0) = 0;
	};
}

#endif
