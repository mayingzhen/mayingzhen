#ifndef __ScriptDevice_h__
#define __ScriptDevice_h__



namespace ma
{
	class IScriptObject;

	class ENGINE_API IScriptDevice 
	{
	public:

		virtual bool			Start() = 0;

		virtual void			Stop() = 0;

		virtual IScriptObject*	CreateScriptObject(const char* pszName) = 0;

		virtual void			DestoryScriptObject(IScriptObject* pScriptObject) = 0;

		virtual	void			ParseScriptAll(const char* pDllName) = 0;
	};

	ENGINE_API IScriptDevice*	GetScriptDevice();

	ENGINE_API void				SetScriptDevice(IScriptDevice* pDevice);

}

#endif //__ScriptDevice_h__