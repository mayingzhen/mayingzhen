#ifndef __Log_H__
#define __Log_H__


class COMMON_API ILogListener
{
public:

	virtual void OnLog(Uint msgType,Uint msgSource,const char* desc) = 0;

};

COMMON_API void SetLogListener(ILogListener* pLog);

COMMON_API void Log(Uint msgType,Uint msgSource,const char* fmt,...);

COMMON_API void Log(const char* fmt,...);

#endif