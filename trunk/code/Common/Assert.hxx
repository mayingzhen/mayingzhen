#include "Assert.h"

#if PLATFORM_WIN == 1

int AssertMsg(bool bOK, const char* text, const char* description, const char* file,int line)
{
	if (bOK)
		return DEBUG_CONTINUE;

	HWND hwnd = GetActiveWindow();
	if (!hwnd)
		hwnd = GetLastActivePopup(hwnd);

	const int BUFFER_SIZE = 1024;
	const int MODULE_NAME_SIZE = 255;
	char buffer[BUFFER_SIZE] = {0};
	char program[MODULE_NAME_SIZE];

	if (!GetModuleFileName(NULL, program, MODULE_NAME_SIZE))
	{
		strcpy(program, "<unknow application>");
	}

	_snprintf(buffer, BUFFER_SIZE, 
		"Assert Failed \n\n"		\
		"Program:      %s\n"		\
		"File:         %s\n"		\
		"Line:         %d\n"		\
		"Error:        %s\n"		\
		"Description:  %s\n",
		program, file, line, text, description);

	int iRet = ::MessageBox(hwnd, buffer, "xmEngine Run Time", 
		MB_SYSTEMMODAL | MB_TOPMOST | MB_SETFOREGROUND | MB_ABORTRETRYIGNORE | MB_ICONERROR);
	switch (iRet)
	{
	case IDABORT:
		ExitProcess(-1);
		return 0;

	case IDRETRY:
		return DEBUG_BACKPOINT;

	case IDIGNORE:
		return DEBUG_IGNORE;

	}

	return DEBUG_CONTINUE;
}

#elif PLATFORM_ANDROID == 1

int AssertMsg(bool bOk, const char* exper, const char* description, 
				const char* file, int line)
{
	if (bOk)
		return 0;

	const int BUFFER_SIZE = 1024;
	const int MODULE_NAME_SIZE = 255;
	char buffer[BUFFER_SIZE] = {0};
	char program[MODULE_NAME_SIZE];

	snprintf(buffer, BUFFER_SIZE, 
		"Assert Failed   \n"	    \
		"exper:        %s\n"		\
		"description:  %s\n"		\
		"File:         %s\n"		\
		"Line:         %d\n",		
		exper, description, file, line);

	Log(buffer);

	return 0;
}

#endif
