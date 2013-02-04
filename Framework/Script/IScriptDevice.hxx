#include <Framework/Script/IScriptDevice.h>
//#include <Framework/Script/ScriptEventBinder.hxx>

namespace ma
{

	namespace Script
	{
		IScriptDevice* g_pDevice = NULL;
	}


// 	void	IScriptDevice::RegisterScriptEventFactory(ScriptEventFactory* pFactory)
// 	{
// 
// 	}
// 
// 	void	IScriptDevice::UnRegisterScriptEventFactory(ScriptEventFactory* pFactory)
// 	{
// 
// 	}
// 
// 	DefaultScriptEventFactory*	IScriptDevice::GetDefaultScriptEventFactory()
// 	{
// 		return &m_defaultScriptFactory;
// 	}
// 
// 	ScriptEventBinder*	IScriptDevice::CreateScriptEventBinder(const ScriptSignalKey& key)
// 	{
// 		return m_defaultScriptFactory.CreateScriptEventBinder(key);
// 	}

	FRAMEWORK_API IScriptDevice* GetScriptDevice()
	{
		return Script::g_pDevice;
	}

	FRAMEWORK_API void SetScriptDevice(IScriptDevice* pDevice)
	{
		Script::g_pDevice = pDevice;
	}



}