#include <Framework/Script/IScriptDevice.h>
//#include <Framework/Script/ScriptEventBinder.hxx>

namespace ma
{

	namespace Script
	{
		ScriptDevice* g_pDevice = NULL;
	}


// 	void	ScriptDevice::RegisterScriptEventFactory(ScriptEventFactory* pFactory)
// 	{
// 
// 	}
// 
// 	void	ScriptDevice::UnRegisterScriptEventFactory(ScriptEventFactory* pFactory)
// 	{
// 
// 	}
// 
// 	DefaultScriptEventFactory*	ScriptDevice::GetDefaultScriptEventFactory()
// 	{
// 		return &m_defaultScriptFactory;
// 	}
// 
// 	ScriptEventBinder*	ScriptDevice::CreateScriptEventBinder(const ScriptSignalKey& key)
// 	{
// 		return m_defaultScriptFactory.CreateScriptEventBinder(key);
// 	}

	FRAMEWORK_API ScriptDevice* GetScriptDevice()
	{
		return Script::g_pDevice;
	}

	FRAMEWORK_API void SetScriptDevice(ScriptDevice* pDevice)
	{
		Script::g_pDevice = pDevice;
	}



}