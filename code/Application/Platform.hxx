#include "Platform.h"

#if PLATFORM_WIN == 1
#include "Win/PlatformWin.hxx"
#elif PLAFTORM_IOS == 1
#elif PLATFORM_ANDROID == 1
#include "Android/PlatformAndroid.hxx"
#endif

namespace ma
{
	const char*	Platform::GetAppPath() const
	{
		return m_strAppPath.c_str();
	}

	void Platform::SetAppPath(const char* pAppPath) 
	{
		m_strAppPath = pAppPath ? pAppPath : "";
	}
}


