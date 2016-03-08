#include "PlatformWin.h"


void _OutputDebugString(const char* buf)
{
	::OutputDebugString(buf);
	::OutputDebugString("\n");	

	printf("%s \n",buf);
}
