#ifndef __ScriptSystem_h__
#define __ScriptSystem_h__



namespace ma
{
	class IScriptObject;
	class SceneNode;

	class ENGINE_API IScriptSystem
	{
	public:
		virtual void 			Init() = 0;

		virtual void			Shutdown() = 0;

		virtual bool			Start() = 0;

		virtual void			Stop() = 0;

		virtual void			Update() = 0;

		virtual IScriptObject*	CreateScriptObject(SceneNode* pGameObj) = 0;

		virtual void			DeleteScriptObject(IScriptObject* pScriptObject) = 0;

		virtual	void			ParseScriptAll(const char* pDllName) = 0;
	};

	ENGINE_API IScriptSystem*	GetScriptSystem();

	ENGINE_API void				SetScriptSystem(IScriptSystem* pScriptSystem);

}

#endif //__ScriptSystem_h__