#include "WwiseSystem.h"
#include "WwiseComponent.h"
#include "WwiseBank.h"
#include "WwiseListener.h"

#include "IO/Win32/AkDefaultIOHookBlocking.h"

#include "AK/Comm/AkCommunication.h"
#include "AK/SoundEngine/Common/AkModule.h"
#include "AK/MusicEngine/Common/AkMusicEngine.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"
#include "AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "AK/Plugin/AllPluginsRegistrationHelpers.h"

#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 2*1024*1024

namespace ma
{

CAkDefaultIOHookBlocking g_pLowLevelIO;

WwiseSystem::WwiseSystem(void)
{
	
}

WwiseSystem::~WwiseSystem(void)
{
}

bool WwiseSystem::Init(HWND hwnd, const char* pszSoundBashPath,const char* pszLanguage)
{
	// MemoryMgr::Init
	{

		AkMemSettings memSettings;
		memSettings.uMaxNumPools = 20;
		AKRESULT res = AK::MemoryMgr::Init(&memSettings);
		if (res != AK_Success)
		{
			LogError("AK::MemoryMgr::Init() returned AKRESULT %d", res );
			return false;
		}
	}

	// StreamMgr::Creat
	{
		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings( stmSettings );
		if ( !AK::StreamMgr::Create(stmSettings) )
		{
			LogError("AK::StreamMgr::Create() failed");
			return false;
		}

		AKRESULT res = AK::StreamMgr::SetCurrentLanguage( StaticFunc::AnsiToUnicode(pszLanguage).c_str() );
		if (res != AK_Success)
		{
			LogError("SetCurrentLanguage failed");
			return false;
		}
	}

	// LowLevelIO.Init
	{
		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
		AKRESULT res = g_pLowLevelIO.Init(deviceSettings);
		if (res != AK_Success)
		{
			LogError("m_lowLevelIO.Init() returned AKRESULT %d", res);
			return false;
		}

		g_pLowLevelIO.SetBasePath( StaticFunc::AnsiToUnicode(pszSoundBashPath).c_str() );
	}

	// SoundEngine::Init
	{
		AkInitSettings initSettings;
		AK::SoundEngine::GetDefaultInitSettings( initSettings );
		initSettings.uDefaultPoolSize = DEMO_DEFAULT_POOL_SIZE;
		initSettings.bUseSoundBankMgrThread = false;
		// 
		// #if defined( INTEGRATIONDEMO_ASSERT_HOOK )
		// 		initSettings.pfnAssertHook = INTEGRATIONDEMO_ASSERT_HOOK;
		// #endif // defined( INTEGRATIONDEMO_ASSERT_HOOK )


		AkPlatformInitSettings platformInitSettings;
		AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );
		platformInitSettings.uLEngineDefaultPoolSize = DEMO_LENGINE_DEFAULT_POOL_SIZE;
		platformInitSettings.hWnd = hwnd;

		AKRESULT res = AK::SoundEngine::Init(&initSettings, &platformInitSettings);
		if ( res != AK_Success )
		{
			LogError("AK::SoundEngine::Init() returned AKRESULT %d", res);
			return false;
		}
	}

	// MusicEngine::Init
	{
		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);

		AKRESULT res = AK::MusicEngine::Init(&musicInit);
		if ( res != AK_Success )
		{
			LogError("AK::MusicEngine::Init() returned AKRESULT %d", res);
			return false;
		}
	}

	// Comm::Init
	{
#ifdef _DEBUG
		// Initialize communications (not in release build!)
		AkCommSettings commSettings;
		AK::Comm::GetDefaultInitSettings(commSettings);
		AKPLATFORM::SafeStrCpy(commSettings.szAppNetworkName, "Integration Demo", AK_COMM_SETTINGS_MAX_STRING_SIZE);
		AKRESULT res = AK::Comm::Init( commSettings );
		if ( res != AK_Success )
		{
			LogError("AK::Comm::Init() returned AKRESULT %d. Communication between the Wwise authoring application and the game will not be possible.", res );
		}
#endif 
	}

	//SoundEngine::RegisterAllPlugins
	{
		// Register plugins
		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		AKRESULT res = AK::SoundEngine::RegisterAllPlugins();
		if (res != AK_Success)
		{
			LogError("AK::SoundEngine::RegisterAllPlugins() returned AKRESULT %d.", res);
		}	
	}

	// Load the Init sound bank
	// NOTE: The Init sound bank must be the first bank loaded by Wwise!
	AkBankID bankID;
	if ( AK::SoundEngine::LoadBank( "Init.bnk", AK_DEFAULT_POOL_ID, bankID ) != AK_Success )
	{
		LogError( "Init.bnk" );
		return false;
	}


	m_pDefaultListener = new WwiseListener(0);

	return true;
}

void WwiseSystem::Shoutdown()
{
#ifdef _DEBUG
	// Terminate communications between Wwise and the game
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	if ( AK::SoundEngine::IsInitialized() )
	{
		AK::SoundEngine::Term();
	}

	// Terminate the streaming device and streaming manager
	// CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	if ( AK::IAkStreamMgr::Get() )
	{
		g_pLowLevelIO.Term();
		AK::IAkStreamMgr::Get()->Destroy();
	}

	// Terminate the Memory Manager
	if ( AK::MemoryMgr::IsInitialized() )
	{
		AK::MemoryMgr::Term();
	}
}

void WwiseSystem::Update()
{
	AKRESULT hr = AK::SoundEngine::RenderAudio();
	ASSERT(hr == AK_Success);
}

void WwiseSystem::SetState(const char* pszStateGroup,const char* pszState)
{
	AKRESULT hr = AK::SoundEngine::SetState(pszStateGroup,pszState);
	ASSERT(hr == AK_Success);
}

void WwiseSystem::SetRTPValue(const char* pszRtpcName,float fValue)
{
	AKRESULT hr = AK::SoundEngine::SetRTPCValue(pszRtpcName,fValue,AK_INVALID_GAME_OBJECT);
	ASSERT(hr == AK_Success);
}

void WwiseSystem::StopAll()
{
	AK::SoundEngine::StopAll();
}


RefPtr<WwiseListener> WwiseSystem::GetDefaultListener() const
{
	return m_pDefaultListener;
}

WwiseSystem* g_pWwiseSystem = NULL;
WwiseSystem* GetWwiseSystem()
{
	return g_pWwiseSystem;
}

}