#include <MonoScript/MonoScriptResource.h>
#include <MonoScript/MonoScriptDevice.h>

namespace ma
{

	MonoScriptResource::MonoScriptResource(const char* pszPath):Resource(pszPath)
	{
		//m_bAutoLoad = true;
	}

	MonoScriptResource::~MonoScriptResource()
	{

	}

	bool MonoScriptResource::Load()
	{
		const char* pszFilePath = m_sResPath.c_str();

		MonoScriptDevice* pScriptDevice = (MonoScriptDevice*)GetScriptDevice();
		if (pScriptDevice == NULL)
			return false;

		pScriptDevice->ParseScriptDll(pszFilePath);

		return true;
	}

	void MonoScriptResource::Unload()
	{
		const char* pszFilePath = m_sResPath.c_str();

		MonoScriptDevice* pScriptDevice = (MonoScriptDevice*)GetScriptDevice();
		if (pScriptDevice == NULL)
			return;
	
		pScriptDevice->UnParseScriptDll(pszFilePath);
	}
}
