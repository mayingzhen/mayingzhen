
#ifndef __LOG_H_
#define __LOG_H_


//��־����
class Log
{
public:
	virtual void WriteInfo( const char* sFormat, ... ) = 0;
};


//ȫ����־
bool InitLog( const char* logName );
Log* GetLog();

#endif	//__LOG_H_