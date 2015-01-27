#ifndef __Log_H__
#define __Log_H__

namespace ma
{
	class ILogListener
	{
	public:

		virtual void LogInfo(const char* pszMsg) = 0;
		virtual void LogError(const char* pszMsg) = 0;
		virtual void LogWarning(const char* pszMsg) = 0;

	};

	void SetLogListener(ILogListener* pLog);

	void LogInfo(const char* pszFormat, ...);

	void LogError(const char* pszFormat, ...);
	
	void LogWarning(const char* pszFormat, ...);

	void DebugMsg(const char* pszFormat, ...);
}



#endif
