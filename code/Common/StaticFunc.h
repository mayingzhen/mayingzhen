/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	静态函数类
\*********************************************************************/

#pragma once

#include "Macro.h"
#include "StringUtils.h"

class StaticFunc
{
public:
	StaticFunc(void);
	virtual ~StaticFunc(void);


// ---------------------------------------------------------------------
// 日志
// ---------------------------------------------------------------------
public:
	// 日志打印到文件里
	static void LogMsg(const char* pszFormat, ...);
	static void LogMsgEx(const char* pszFile, const char* pszFormat, ...);

	// 日志打印到输出窗口里
	static void DebugMsg(const char* pszFormat, ...);
// ---------------------------------------------------------------------
// 时间
// ---------------------------------------------------------------------
public:
	// 获取时间滴答（毫秒）
	static unsigned long GetTime(void);

	static void GetTime(OUT int& nYear, OUT int& nMonth, OUT int& nDay, OUT int& nHour, OUT int& nMin, OUT int& nSec);

// ---------------------------------------------------------------------
// 随机数
// ---------------------------------------------------------------------
public:
	static int RandGet(int nMax, bool bRealRand = false);

// ---------------------------------------------------------------------
// exe
// ---------------------------------------------------------------------
public:
	// 获取exe所在路径（以/结束）
	static std::string GetCurrentExeDir();

	// 获取当前路径
	static std::string GetCurrentDir();

	// 获取文件路径带(/)
	static std::string GetFilePath(const std::string& strFile);

    // 获取文件目录
    static std::string GetFileDir(const std::string& strFile);

	// 返回不带路径也不带后缀的文件名
	static std::string GetBaseName(const std::string& strFile);

    // 获取不带路径的文件名
    static std::string GetFileName(const std::string& strFile);

	// 获取文件扩展名(带.)
	static std::string GetFileExt(const std::string& strFile);

	// 路径转小写并且'\\'变成'/'
	static std::string ToLowerCase(const std::string& strFile);

	static std::string ReplaceFileExt(const std::string& strFile,const std::string& strNewExt);

	// 创建文件夹
	static void MakeDir(const char* pszPath);

    static bool IsFileExist( const char* fullFilePath );

    static int FileLength(FILE* pFile);
    static int FileRead(FILE* pFile, OUT char* buf, int len);
    static std::string ToString(int n);

    static void StrRemoveSpaces(std::string& str);
    static void StrSplit(const std::string& str, const std::string& delimiter, OUT std::vector<std::string>& outElements);

    // 保证最后一个字符是/
    static void StrMakePath(IN OUT std::string& str);

    // 若url为true，则将'\'替换为/；否则将/替换为'\'
    static void StrFormatPath(IN OUT std::string& str, bool url);
    static int StrHexToInt(const std::string& str);
    static bool StrIsEndWith(const std::string& str, const std::string& strEndWith, bool bCase);
// ---------------------------------------------------------------------
// 字体编码转换
// ---------------------------------------------------------------------
public:
	// Unicode字符串转成Ansi字符串
	static std::string UnicodeToAnsi(const wchar_t* pszUnicode);

	// Ansi字符串转成Unicode字符串
	static std::wstring AnsiToUnicode(const char* pszAnisi);

// ---------------------------------------------------------------------
// 加密
// ---------------------------------------------------------------------
public:
	static std::string GetMd5(const std::string& strBuffer);
	static std::string GetMd5ByFile(const char* pszFileName);
    
};


// ---------------------------------------------------------------------
// 测试时间
// ---------------------------------------------------------------------
#ifdef WIN32
    #define BEGIN_TIME(name) \
	    LARGE_INTEGER nFreq##name;\
	    LARGE_INTEGER nBeginTime##name;\
	    LARGE_INTEGER nEndTime##name;\
	    QueryPerformanceFrequency(&nFreq##name);\
	    QueryPerformanceCounter(&nBeginTime##name)

    #define END_TIME(name)	\
        QueryPerformanceCounter(&nEndTime##name);\
        double dWasteTime##name = (double)(nEndTime##name.QuadPart-nBeginTime##name.QuadPart)/(double)nFreq##name.QuadPart*1000;\
        StaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

    #define END_TIME_FILTER(name,timefilter)	\
		QueryPerformanceCounter(&nEndTime##name);\
		double dWasteTime##name = (double)(nEndTime##name.QuadPart-nBeginTime##name.QuadPart)/(double)nFreq##name.QuadPart*1000;\
		if(dWasteTime##name >timefilter) CStaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

#else

    #define BEGIN_TIME(name) \
        //timeval ttBegin##name;\
        //gettimeofday(&ttBegin##name, NULL)

    #define END_TIME(name)	\
        //timeval ttEnd##name;\
        //gettimeofday(&ttEnd##name, NULL);\
        //double dWasteTime##name = (ttEnd##name.tv_sec-ttBegin##name.tv_sec)*1000 + (ttEnd##name.tv_usec-ttBegin##name.tv_usec)*0.001;\
        //StaticFunc::DebugMsg("%s %fms", #name, dWasteTime##name)

#endif




