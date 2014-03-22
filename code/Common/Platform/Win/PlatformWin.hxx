#include "PlatformWin.h"


void _OutputDebugString(const char* buf)
{
	::OutputDebugString(buf);
	::OutputDebugString("\n");	

	printf("%s \n",buf);
}

int gettimeofday(struct timeval * val, struct timezone *)
{
	if (val)
	{
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);

		struct tm tTm;
		tTm.tm_year     = wtm.wYear - 1900;
		tTm.tm_mon      = wtm.wMonth - 1;
		tTm.tm_mday     = wtm.wDay;
		tTm.tm_hour     = wtm.wHour;
		tTm.tm_min      = wtm.wMinute;
		tTm.tm_sec      = wtm.wSecond;
		tTm.tm_isdst    = -1;

		val->tv_sec     = (long)mktime(&tTm);       // time_t is 64-bit on win32
		val->tv_usec    = wtm.wMilliseconds * 1000;
	}
	return 0;
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
	const int MODULE_NAME_SIZE = 255;
	char buffer[BUFFER_SIZE];
	char program[MODULE_NAME_SIZE];

	if (!GetModuleFileName(NULL, program, MODULE_NAME_SIZE))
	{
		strcpy(program, "<unknow application>");
	}

	_snprintf(buffer, BUFFER_SIZE, 
		"%s\n\n"					\
		"Program:      %s\n"		\
		"File:         %s\n"		\
		"Line:         %d\n\n"	\
		"Error:        %s\n"		\
		"Description:  %s\n",
		title, program, file, line, text, description);

	int iRet = MessageBox(hwnd, buffer, "maEngine Run Time", 
		MB_TASKMODAL | MB_SETFOREGROUND | MB_ABORTRETRYIGNORE | MB_ICONERROR);
	switch (iRet)
	{
	case IDIGNORE:
		return DEBUG_IGNORE;

	case IDCONTINUE:
		return DEBUG_CONTINUE;

	case IDRETRY:
		return DEBUG_CONTINUE;

	case IDABORT:
		return DEBUG_BACKPOINT;

	default:
		ExitProcess(-1);
		return 0;
	}
}


