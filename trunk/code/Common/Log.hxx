#include "Log.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace ma
{
	static ILogListener* g_pLogListener = NULL;

	void SetLogListener(ILogListener* pLog)
	{
		g_pLogListener = pLog;
	}

// 	void Log(uint32 msgType,uint32 msgSource,const char* fmt,...)
// 	{
// 		if (fmt)
// 		{
// 			const int len = 1024;
// 			char buffer[len];
// 			va_list args;
// 			va_start( args, fmt );
// 			vsprintf( buffer, fmt, args );
// 
// 			if (NULL != g_pLogListener)
// 			{
// 				g_pLogListener->OnLog(msgType,msgSource,buffer);
// 			}
// 			else
// 			{
// 				_OutputDebugString(buffer);
// 			}	
// 		}
// 	}
// 
// 	void Log(const char* fmt,...)
// 	{
// 		if (fmt)
// 		{
// 			const int len = 1024;
// 			char buffer[len];
// 			va_list args;
// 			va_start( args, fmt );
// 			vsprintf( buffer, fmt, args );
// 			Log(0,0,buffer);
// 		}
// 	}

#ifdef __ANDROID__
	void android_print(android_LogPriority e, const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		__android_log_vprint(e, "tq", format, argptr);
		va_end(argptr);
	}
#endif

	void DebugMsg(const char* pszFormat, ...)
	{
		char szMsg[10240];
		memset(szMsg, 0, 10240);

		va_list va_alist;
		va_start(va_alist, pszFormat);
		vsprintf(szMsg, pszFormat, va_alist);
		va_end(va_alist);

		strcat(szMsg, "\r\n");

		printf(szMsg);
#ifdef WIN32
		::OutputDebugString(szMsg);
#endif
	}

	void LogInfo( const char* pszFormat, ... )
	{
		char szContent[10240] = "";
		va_list va_alist;
		va_start(va_alist, pszFormat);
		vsprintf(szContent, pszFormat, va_alist);
		va_end(va_alist);

		if (g_pLogListener != NULL)
		{
			g_pLogListener->LogInfo(szContent);
		}
#ifdef __ANDROID__
		android_print(ANDROID_LOG_INFO, szContent);
#endif

		// 获取时间
		time_t ltime;
		time(&ltime);
		struct tm* pTime = localtime(&ltime);

		char szMsg[10240];
		sprintf(szMsg, "Info[%02u:%02u:%02u] %s", pTime->tm_hour, pTime->tm_min, pTime->tm_sec, szContent);
		
		DebugMsg(szMsg);
	}

	void LogError( const char* pszFormat, ... )
	{
		char szContent[10240] = "";
		va_list va_alist;
		va_start(va_alist, pszFormat);
		vsprintf(szContent, pszFormat, va_alist);
		va_end(va_alist);

		if (g_pLogListener != NULL)
		{
			g_pLogListener->LogError(szContent);
		}
#ifdef __ANDROID__
		android_print(ANDROID_LOG_ERROR, szContent);
#endif

		// 获取时间
		time_t ltime;
		time(&ltime);
		struct tm* pTime = localtime(&ltime);

		char szMsg[10240];
		sprintf(szMsg, "Error[%02u:%02u:%02u] %s", pTime->tm_hour, pTime->tm_min, pTime->tm_sec, szContent);
		DebugMsg(szMsg);
		assert(false);

		if(0)
		{
			char szFileName[256];
			sprintf(szFileName, "Log_%02u_%02u_%02u.log", pTime->tm_year + 1900,
				pTime->tm_mon + 1,
				pTime->tm_mday);

			FILE* pFile = fopen(szFileName, "a+");
			if (NULL == pFile)
			{
				return;
			}

			fprintf(pFile, "%s\n", szFileName);
			fclose(pFile);
		}
	}

	void LogWarning( const char* pszFormat, ... )
	{
		char szContent[10240] = "";
		va_list va_alist;
		va_start(va_alist, pszFormat);
		vsprintf(szContent, pszFormat, va_alist);
		va_end(va_alist);

		if (g_pLogListener != NULL)
		{
			g_pLogListener->LogWarning(szContent);
		}
#ifdef __ANDROID__
		android_print(ANDROID_LOG_WARN, szContent);
#endif

		// 获取时间
		time_t ltime;
		time(&ltime);
		struct tm* pTime = localtime(&ltime);

		char szMsg[10240];
		sprintf(szMsg, "Warning[%02u:%02u:%02u] %s", pTime->tm_hour, pTime->tm_min, pTime->tm_sec, szContent);
		
		DebugMsg(szMsg);
	}
}


