#ifndef __EdScriptComponent_H__
#define __EdScriptComponent_H__

#include "IScriptObject.h"

namespace ma
{
	class ENGINE_API EdScriptObject : public ScriptObject
	{
	public:
		EdScriptObject(TiXmlElement* pEleClass);

		virtual	const char* GetName();

		virtual UINT		GetFieldNumber();

		virtual const char* GetFieldName(UINT index);

		virtual const char*	GetFieldTypeName(UINT index);

		virtual float		GetFloat(const char* pszName);

		virtual void		SetFloat(const char* pszName,float fValue);

		virtual int			GetInt(const char* pszName);

		virtual void		SetInt(const char* pszName, int nValue);

		virtual UINT		GetUint(const char* pszName);

		virtual void		SetUint(const char* pszName, UINT nValue);

		virtual D3DXVECTOR3	GetVector3(const char* pszName);

		virtual void		SetVector3(const char* pszName,const D3DXVECTOR3& vecotr3);	

		virtual void		InvokeMethod(const char* pszMethod,int param_count = 0, void **params = NULL) {}

		virtual void		InvokeMethod(ScriptMethodHandle hMethod,int param_count = 0, void **params = NULL) {}

		virtual	ScriptMethodHandle GetScriptMethodHandle(const char* pMethodName,int param_count = 0) {return 0;}

		// xml
		TiXmlElement*		GetTiXmlElement() {return m_pXmlClass;}
		
		UINT				GetXmlFieldNumber() {return m_vXmlFields.size();}

		TiXmlElement*		GetXmlFieldByIndex(UINT index) {return m_vXmlFields[index];}

	private:
		TiXmlElement*				 m_pXmlClass;
		std::vector<TiXmlElement*>   m_vXmlFields; // ·½±ã±éÀú
	};
}

#endif
