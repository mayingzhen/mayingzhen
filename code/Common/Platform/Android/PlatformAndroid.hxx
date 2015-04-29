#include <Common/Platform/Android/PlatformAndroid.h>
#include <android/log.h>


void _OutputDebugString(const char* buf)
{
	if (buf == NULL)
		return;
       
	__android_log_print(ANDROID_LOG_INFO,"MaEngine",buf);
	
}