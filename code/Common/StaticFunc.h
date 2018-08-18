/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	��̬������
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
// ��־
// ---------------------------------------------------------------------
public:
	// ��־��ӡ���ļ���
	static void LogMsg(const char* pszFormat, ...);
	static void LogMsgEx(const char* pszFile, const char* pszFormat, ...);

	// ��־��ӡ�����������
	static void DebugMsg(const char* pszFormat, ...);
// ---------------------------------------------------------------------
// ʱ��
// ---------------------------------------------------------------------
public:
	// ��ȡʱ��δ𣨺��룩
	static unsigned long GetTime(void);

	static void GetTime(OUT int& nYear, OUT int& nMonth, OUT int& nDay, OUT int& nHour, OUT int& nMin, OUT int& nSec);

// ---------------------------------------------------------------------
// �����
// ---------------------------------------------------------------------
public:
	static int RandGet(int nMax, bool bRealRand = false);

// ---------------------------------------------------------------------
// exe
// ---------------------------------------------------------------------
public:
	// ��ȡexe����·������/������
	static std::string GetCurrentExeDir();

	// ��ȡ��ǰ·��
	static std::string GetCurrentDir();

	// ��ȡ�ļ�·����(/)
	static std::string GetFilePath(const std::string& strFile);

    // ��ȡ�ļ�Ŀ¼
    static std::string GetFileDir(const std::string& strFile);

	// ���ز���·��Ҳ������׺���ļ���
	static std::string GetBaseName(const std::string& strFile);

    // ��ȡ����·�����ļ���
    static std::string GetFileName(const std::string& strFile);

	// ��ȡ�ļ���չ��(��.)
	static std::string GetFileExt(const std::string& strFile);

	// ·��תСд����'\\'���'/'
	static std::string ToLowerCase(const std::string& strFile);

	static std::string ReplaceFileExt(const std::string& strFile,const std::string& strNewExt);

	// �����ļ���
	static void MakeDir(const char* pszPath);

    static bool IsFileExist( const char* fullFilePath );

    static int FileLength(FILE* pFile);
    static int FileRead(FILE* pFile, OUT char* buf, int len);
    static std::string ToString(int n);

    static void StrRemoveSpaces(std::string& str);
    static void StrSplit(const std::string& str, const std::string& delimiter, OUT std::vector<std::string>& outElements);

    // ��֤���һ���ַ���/
    static void StrMakePath(IN OUT std::string& str);

    // ��urlΪtrue����'\'�滻Ϊ/������/�滻Ϊ'\'
    static void StrFormatPath(IN OUT std::string& str, bool url);
    static int StrHexToInt(const std::string& str);
    static bool StrIsEndWith(const std::string& str, const std::string& strEndWith, bool bCase);
// ---------------------------------------------------------------------
// �������ת��
// ---------------------------------------------------------------------
public:
	// Unicode�ַ���ת��Ansi�ַ���
	static std::string UnicodeToAnsi(const wchar_t* pszUnicode);

	// Ansi�ַ���ת��Unicode�ַ���
	static std::wstring AnsiToUnicode(const char* pszAnisi);

// ---------------------------------------------------------------------
// ����
// ---------------------------------------------------------------------
public:
	static std::string GetMd5(const std::string& strBuffer);
	static std::string GetMd5ByFile(const char* pszFileName);
    
};


// ---------------------------------------------------------------------
// ����ʱ��
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




