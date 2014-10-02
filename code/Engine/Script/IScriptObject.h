#ifndef __ScriptObject_h__
#define __ScriptObject_h__

namespace ma
{
	class ENGINE_API IScriptObject : public Component
	{
		DECL_OBJECT(IScriptObject);

	public:
		IScriptObject(SceneNode* pGameObj);

		virtual	const char* GetName() = 0;

		virtual	void		SetName(const char* pName) = 0;

		virtual float		GetFloat(const char* pszName) = 0;

		virtual void		SetFloat(const char* pszName,float fValue) = 0;

		virtual int			GetInt(const char* pszName) = 0;

		virtual void		SetInt(const char* pszName, int nValue) = 0;

		virtual Vector3		GetVector3(const char* pszName) = 0;

		virtual void		SetVector3(const char* pszName,const Vector3& vecotr3) = 0;	

		virtual void		InvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL) = 0;

		virtual void		Serialize(Serializer& sl, const char* pszLable = "ScriptObject") = 0;
	};

	DeclareRefPtr(IScriptObject);
}

#endif
