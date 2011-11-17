
#ifndef __LOG_H_
#define __LOG_H_


//日志基类
class Log
{
public:
	virtual void WriteInfo( const char* sFormat, ... ) = 0;
};


//全局日志
bool InitLog( const char* logName );
Log* GetLog();

#endif	//__LOG_H_