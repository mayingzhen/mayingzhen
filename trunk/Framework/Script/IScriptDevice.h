#ifndef __ScriptDevice_h__
#define __ScriptDevice_h__



namespace ma
{
	typedef void* ScriptObjectHandle;
	typedef void* ScriptMethodHandle;

	class ScriptObject;

	class FRAMEWORK_API ScriptDevice 
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
// 		virtual xmBool	GetBooleanField(ScriptObjectHandle hObj,S3AName fieldName) = 0;
// 
// 		virtual void	SetBooleanField(ScriptObjectHandle hObj,S3AName fieldName,xmBool val) = 0;
// 
// 		virtual xmInt	GetIntField(ScriptObjectHandle hObj,S3AName fieldName) = 0;
// 
// 		virtual void	SetIntField(ScriptObjectHandle hObj,S3AName fieldName,xmInt val) = 0;
// 
// 		virtual xmFloat	GetFloatField(ScriptObjectHandle hObj,S3AName fieldName) = 0;
// 
// 		virtual void	SetFloatField(ScriptObjectHandle hObj,S3AName fieldName,xmFloat val) = 0;
// 
// 		virtual ScriptObjectHandle	GetObjectField(ScriptObjectHandle hObj,S3AName fieldName) = 0;
// 
// 		virtual void				SetObjectField(ScriptObjectHandle hObj,S3AName field, ScriptObjectHandle val) = 0;
// 
// 		virtual const char*			GetStringValue(ScriptObjectHandle hObj) = 0;


		//------------------------------------------------------------------------------
		//Function Invoke
		//------------------------------------------------------------------------------
//		virtual bool	Execute(ScriptObjectHandle hObj,S3AName funName) = 0;


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

	FRAMEWORK_API ScriptDevice* GetScriptDevice();

	FRAMEWORK_API void			SetScriptDevice(ScriptDevice* pDevice);

}

#endif //__ScriptDevice_h__