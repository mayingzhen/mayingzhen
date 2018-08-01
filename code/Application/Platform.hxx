#include "Platform.h"

#if defined(_WIN32)
#include "Win/PlatformWin.hxx"
#include "Win/imgui_impl_win32.hxx"
#elif defined(__APPLE__)
#elif defined(__ANDROID__)
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


