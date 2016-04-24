/*********************************************************************\
 Copyright(c) 2010, ***, All Rights Reserved
 Author:	
 Created:	2010-07-23
 Describe:	静态函数类
\*********************************************************************/

#pragma once

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
	static string GetCurrentExeDir();

	// 获取当前路径
	static string GetCurrentDir();

	// 获取文件路径带(/)
	static string GetFilePath(const string& strFile);

    // 获取文件目录
    static string GetFileDir(const string& strFile);

	// 返回不带路径也不带后缀的文件名
	static string GetBaseName(const string& strFile);

    // 获取不带路径的文件名
    static string GetFileName(const string& strFile);

	// 获取文件扩展名(带.)
	static string GetFileExt(const string& strFile);

	// 路径转小写并且'\\'变成'/'
	static string ToLowerCase(const string& strFile);

	static string ReplaceFileExt(const string& strFile,const string& strNewExt);

	// 创建文件夹
	static void MakeDir(const char* pszPath);

    static bool IsFileExist( const char* fullFilePath );

    static int FileLength(FILE* pFile);
    static int FileRead(FILE* pFile, OUT char* buf, int len);
    static string ToString(int n);

    static void StrRemoveSpaces(string& str);
    static void StrSplit(const string& str, const string& delimiter, OUT std::vector<string>& outElements);

    // 保证最后一个字符是/
    static void StrMakePath(IN OUT string& str);

    // 若url为true，则将'\'替换为/；否则将/替换为'\'
    static void StrFormatPath(IN OUT string& str, bool url);
    static int StrHexToInt(const string& str);
    static bool StrIsEndWith(const string& str, const string& strEndWith, bool bCase);
// ---------------------------------------------------------------------
// 字体编码转换
// ---------------------------------------------------------------------
public:
	// Unicode字符串转成Ansi字符串
	//static string UnicodeToAnsi(const wchar_t* pszUnicode);

	// Ansi字符串转成Unicode字符串
	//static wstring AnsiToUnicode(const char* pszAnisi);

// ---------------------------------------------------------------------
// ini读取
// ---------------------------------------------------------------------
public:
	static int GetIniInt(const char* pszFileName, const char* pszTitle, const char* pszKey);
	static string GetIniString(const char* pszFileName, const char* pszTitle, const char* pszKey);

// ---------------------------------------------------------------------
// 加密
// ---------------------------------------------------------------------
public:
	static string GetMd5(const string& strBuffer);
	static string GetMd5ByFile(const char* pszFileName);
    
};

