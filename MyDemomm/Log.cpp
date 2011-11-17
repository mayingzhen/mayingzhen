#include <stdio.h>
#include <assert.h>
#include <atltime.h>

#include "Log.h"


//日志实现类

class LogSystem : public Log
{
public:
	LogSystem()
	{
		m_fp = NULL;
	}

	virtual ~LogSystem()
	{
		if( m_fp != NULL )
		{
			fclose( m_fp );
			m_fp = NULL;
		}
	}

	bool Init( const char* pName )
	{
		if( NULL == pName )
		{
			assert( NULL != pName );
			return false;
		}

		//打开文件
		m_fp = fopen(pName, "at");
		if( m_fp == NULL )
		{
			assert( NULL != m_fp );
			return false;
		}

		//写文件头
		CTime nowTime = CTime::GetCurrentTime();
		char sInfo[1024] = {0};
		_snprintf( sInfo, sizeof( sInfo ),
			"========\nLog Instance\nDate: %d-%d-%d %d:%d:%d\n========\n",
			nowTime.GetYear(),
			nowTime.GetMonth(),
			nowTime.GetDay(),
			nowTime.GetHour(),
			nowTime.GetMinute(),
			nowTime.GetSecond() );
		
		assert( sizeof( sInfo ) > strlen( sInfo ) );

		fwrite( sInfo, sizeof(char), strlen(sInfo), m_fp );
		fflush(m_fp);
		return true;
	}
	
public:
	virtual void WriteInfo( const char* sFormat, ... )
	{
		if( m_fp == NULL )
		{
			assert( NULL != m_fp );
			return;
		}

		char sInfo[1024] = {0};
		int nRet = 0;

		va_list argptr;         
		va_start( argptr, sFormat );
		nRet = _vsnprintf( sInfo, sizeof( sInfo ) - 1, sFormat, argptr );
		va_end( argptr );

		assert( sizeof( sInfo ) > strlen( sInfo ) ) ;

		char sLog[1280] = {0};
		CTime nowTime = CTime::GetCurrentTime();
		_snprintf( sLog, sizeof( sLog ), 
			"%d-%d %d:%d:%d :   %s", 
			nowTime.GetMonth(), 
			nowTime.GetDay(), 
			nowTime.GetHour(), 
			nowTime.GetMinute(), 
			nowTime.GetSecond(), 
			sInfo );

		assert( sizeof( sLog ) > strlen( sLog ) );

		fwrite( sLog, sizeof(char), strlen(sLog), m_fp );
		fflush(m_fp);
	}

private:
	FILE* m_fp;
};


static LogSystem* g_pLog = NULL;


//全局日志

bool InitLog( const char* logName )
{
	if( logName == NULL )
	{
		assert( logName != NULL );
		return false;
	}
	g_pLog = new LogSystem;
	return g_pLog->Init( logName );
}

Log* GetLog()
{
	assert( NULL != g_pLog );
	return g_pLog;
}
