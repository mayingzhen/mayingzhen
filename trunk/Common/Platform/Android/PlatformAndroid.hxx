#include <Common/Platform/Android/PlatformAndroid.h>
#include <android/log.h>


void OutputDebugString(const char* buf)
{
	if (buf == NULL)
		return;
       
	__android_log_write(ANDROID_LOG_DEBUG,"",buf);
	
}
