#include "Log.h"

ILogListener* g_pLogListener = NULL;

void SetLogListener(ILogListener* pLog)
{
	g_pLogListener = pLog;
}

void Log(Uint msgType,Uint msgSource,const char* fmt,...)
{
	if (fmt)
	{
		const int len = 1024;
		char buffer[len];
		va_list args;
		va_start( args, fmt );
		vsprintf( buffer, fmt, args );

		if (NULL != g_pLogListener)
		{
			g_pLogListener->OnLog(msgType,msgSource,buffer);
		}
		else
		{
			OutputDebugString(buffer);
		}	
	}
}

void Log(const char* fmt,...)
{
	if (fmt)
	{
		const int len = 1024;
		char buffer[len];
		va_list args;
		va_start( args, fmt );
		vsprintf( buffer, fmt, args );
		Log(0,0,buffer);
	}
}
