#ifndef __Log_H__
#define __Log_H__

namespace ma
{
	class COMMON_API ILogListener
	{
	public:

		virtual void LogInfo(const char* pszMsg) = 0;
		virtual void LogError(const char* pszMsg) = 0;
		virtual void LogWarning(const char* pszMsg) = 0;

	};

	COMMON_API void SetLogListener(ILogListener* pLog);

	COMMON_API void LogInfo(const char* pszFormat, ...);

	COMMON_API void LogError(const char* pszFormat, ...);
	
	COMMON_API void LogWarning(const char* pszFormat, ...);

	COMMON_API void DebugMsg(const char* pszFormat, ...);
}



#endif
