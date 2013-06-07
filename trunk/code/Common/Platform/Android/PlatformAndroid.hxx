#include <Common/Platform/Android/PlatformAndroid.h>
#include <android/log.h>


void OutputDebugString(const char* buf)
{
	if (buf == NULL)
		return;
       
	__android_log_write(ANDROID_LOG_DEBUG,"",buf);
	
}

int DisplayError(const char * title, 
				 const char * text, 
				 const char * description, 
				 const char * file, 
				 int line)
{
	HWND hwnd = GetActiveWindow();
	if (!hwnd)
		hwnd = GetLastActivePopup(hwnd);

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	_snprintf(buffer, BUFFER_SIZE, 
		"%s\n\n"					\
		"File:         %s\n"		\
		"Line:         %d\n\n"	\
		"Error:        %s\n"		\
		"Description:  %s\n",
		title, file, line, text, description);

	OutputDebugString(buffer);	
}
