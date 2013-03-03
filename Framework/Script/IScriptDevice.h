#ifndef __ScriptDevice_h__
#define __ScriptDevice_h__



namespace ma
{
	typedef void* ScriptObjectHandle;
	typedef void* ScriptMethodHandle;

	class ScriptObject;

	class FRAMEWORK_API IScriptDevice 
	{
		//DefaultScriptEventFactory m_defaultScriptFactory;

	public:

		virtual bool			Start() = 0;

		virtual void			Stop() = 0;

		virtual ScriptObject*	CreateScriptObject(const char* pszName) = 0;

		virtual void			DestoryScriptObject(ScriptObject* pScriptObject) = 0;

		virtual UINT			GetScriptNumber() = 0;

		virtual const char*		GetScriptNameByIndex(UINT index) = 0;

		virtual	TiXmlElement*	GetXmlClassByName(const char* pszName) = 0;

		//------------------------------------------------------------------------------
		//Script Object Data Access
		//------------------------------------------------------------------------------
// 		virtual bool	GetBooleanField(ScriptObjectHandle hObj,Name fieldName) = 0;
// 
// 		virtual void	SetBooleanField(ScriptObjectHandle hObj,Name fieldName,bool val) = 0;
// 
// 		virtual xmInt	GetIntField(ScriptObjectHandle hObj,Name fieldName) = 0;
// 
// 		virtual void	SetIntField(ScriptObjectHandle hObj,Name fieldName,xmInt val) = 0;
// 
// 		virtual float	GetFloatField(ScriptObjectHandle hObj,Name fieldName) = 0;
// 
// 		virtual void	SetFloatField(ScriptObjectHandle hObj,Name fieldName,float val) = 0;
// 
// 		virtual ScriptObjectHandle	GetObjectField(ScriptObjectHandle hObj,Name fieldName) = 0;
// 
// 		virtual void				SetObjectField(ScriptObjectHandle hObj,Name field, ScriptObjectHandle val) = 0;
// 
// 		virtual const char*			GetStringValue(ScriptObjectHandle hObj) = 0;


		//------------------------------------------------------------------------------
		//Function Invoke
		//------------------------------------------------------------------------------
//		virtual bool	Execute(ScriptObjectHandle hObj,Name funName) = 0;


		//------------------------------------------------------------------------------
		//Event Binding
		//------------------------------------------------------------------------------
// 		virtual void	RegisterScriptEventFactory(ScriptEventFactory* pFactory);
// 
// 		virtual	void	UnRegisterScriptEventFactory(ScriptEventFactory* pFactory);
// 
// 		virtual DefaultScriptEventFactory*	GetDefaultScriptEventFactory();
// 
// 		virtual ScriptEventBinder*			CreateScriptEventBinder(const ScriptSignalKey& key);
		

	};

	FRAMEWORK_API IScriptDevice* GetScriptDevice();

	FRAMEWORK_API void			SetScriptDevice(IScriptDevice* pDevice);

}

#endif //__ScriptDevice_h__