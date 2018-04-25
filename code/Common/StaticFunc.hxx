#include "StaticFunc.h"
#include "md5.h"
#include "BaseType.h"
#ifdef WIN32
#include <TimeAPI.h>
#include <direct.h>
#endif

#define MAX_STR_NAME 512
#define MAX_STR 512

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#endif

#ifdef __ANDROID__
#include <android/log.h>
#endif
StaticFunc::StaticFunc(void)
{
}

StaticFunc::~StaticFunc(void)
{
}

// ---------------------------------------------------------------------
// ��־
// ---------------------------------------------------------------------
// ��־��ӡ���ļ���
void StaticFunc::LogMsg(const char* pszFormat, ...)
{
	if (NULL == pszFormat)
	{
		return;
	}

#ifdef WIN32
	// ��ȡʱ��
	time_t ltime;
	time(&ltime);
	struct tm* pTime = localtime(&ltime);

	// ��ȡ·�����ļ���
	string strDir = "";//GetCurrentExeDir();
	char szLogName[MAX_STR_NAME] = "";
	sprintf(szLogName, "%sLog_%u_%u_%u.log",
			strDir.c_str(),
			pTime->tm_year + 1900,
			pTime->tm_mon + 1,
			pTime->tm_mday);

	string strFile = szLogName;

	// ��ӡ���ļ���
	char szContent[1024];
	vsprintf(szContent, pszFormat, (char*)((&pszFormat)+1));

	char szBuf[1024];
	sprintf(szBuf, "[%u:%u:%u]", pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

	strcat(szBuf, szContent);

	FILE* pFile = fopen(strFile.c_str(), "a+");
	if (NULL == pFile)
	{
		return;
	}

	fprintf(pFile, "%s\n", szBuf);
	fclose(pFile);
#endif
}

void StaticFunc::LogMsgEx(const char* pszFile, const char* pszFormat, ...)
{
	if (NULL == pszFormat || NULL == pszFile)
	{
		return;
	}

#ifdef WIN32
	char szBuf[MAX_STR] = "";
	vsprintf(szBuf, pszFormat, (char*)((&pszFormat)+1) );

	FILE* pFile = fopen(pszFile, "a+");
	if (NULL == pFile)
	{
		return;
	}

	fprintf(pFile, "%s\n", szBuf);
	fclose(pFile);
#endif
}

// ��־��ӡ�����������
void StaticFunc::DebugMsg(const char* pszFormat, ...)
{
    if (pszFormat == NULL)
    {
        return;
    }

	char szMsg[10240];
	memset(szMsg, 0, 10240);

	va_list va_alist;
	va_start(va_alist, pszFormat);
	vsprintf(szMsg, pszFormat, va_alist);
	va_end(va_alist);

	strcat(szMsg, "\r\n");

#ifdef WIN32
    ::OutputDebugString(szMsg);
#elif defined(__ANDROID__)
    __android_log_write(ANDROID_LOG_INFO, "tq", szMsg);
#endif
}

// ---------------------------------------------------------------------
// ʱ��
// ---------------------------------------------------------------------
unsigned long StaticFunc::GetTime(void)
{
#ifdef WIN32
	#pragma comment(lib, "winmm.lib")
    return timeGetTime();
#else
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec/1000;
#endif
	
}

void StaticFunc::GetTime(OUT int& nYear, OUT int& nMonth, OUT int& nDay, OUT int& nHour, OUT int& nMin, OUT int& nSec)
{
	time_t long_time;
	time(&long_time);

	struct tm* pTime = localtime(&long_time);

	nYear = pTime->tm_year + 1900;
	nMonth = pTime->tm_mon + 1;
	nDay = pTime->tm_mday;
	nHour = pTime->tm_hour;
	nMin = pTime->tm_min;
	nSec = pTime->tm_sec;
}

// ---------------------------------------------------------------------
// �����
// ---------------------------------------------------------------------
int StaticFunc::RandGet(int nMax, bool bRealRand/* = false*/)
{
	if (nMax == 0)
	{
		return 0;
	}

	if (bRealRand)
	{
		::srand( (unsigned)GetTime() );
	}

	return ::rand()%nMax;
}

// ---------------------------------------------------------------------
// exe
// ---------------------------------------------------------------------
string StaticFunc::GetCurrentExeDir()
{
#ifdef WIN32
	char szFullPath[MAX_STR_NAME] = "";
	::GetModuleFileName(NULL, szFullPath, MAX_STR_NAME);
#endif
	char szDrive[MAX_STR_NAME] = "";
	char szDir[MAX_STR_NAME] = "";
#ifdef WIN32
	char szFileName[MAX_STR_NAME] = "";
	char szExt[MAX_STR_NAME] = "";
	_splitpath(szFullPath, szDrive, szDir, szFileName, szExt);
#endif
	char szReturn[MAX_STR_NAME] = "";
	strcpy(szReturn, szDrive);
	strcat(szReturn, szDir);

	return szReturn;
}

// ��ȡ��ǰ·��
string StaticFunc::GetCurrentDir()
{
	char szDir[MAX_STR];
#ifdef WIN32
	::GetCurrentDirectory(MAX_STR, szDir);
#endif
	return szDir;
}


// ��ȡ�ļ�·����(/)
string StaticFunc::GetFilePath(const string& strFile)
{
	string strPath;
	int index = strFile.find_last_of( "\\" );
	if ( index == -1 )
		index = strFile.find_last_of( "/" );
	if ( index==-1 )
	{
		return "";
	}

	strPath = strFile.substr(0, index + 1);
	return strPath;
}

string StaticFunc::GetFileDir(const string& strFile)
{
    string strPath;
    int index1 = strFile.find_last_of( "\\" );
    int index2 = strFile.find_last_of( "/" );
    int index = /*std::*/max(index1,index2);
    if (index == -1)
    {
        return "";
    }

    strPath = strFile.substr(0, index);
    return strPath;
}

// ���ز���·��Ҳ������׺���ļ���
string StaticFunc::GetBaseName(const string& strFile)
{
	string strFileName;
	int index = strFile.find_last_of( "\\" );
	if ( index == -1 )
		index = strFile.find_last_of( "/" );
	if ( index==-1 )
		strFileName = strFile;

	if ( index==((int)strFile.length()-1) )
		return "";

	strFileName = strFile.substr(index + 1);

	index = strFileName.find_last_of(".");
	if (index != -1)
	{
		strFileName = strFileName.substr(0, index);
	}

	return strFileName;
}

string StaticFunc::GetFileName(const string& str)
{
    int index = (int)str.find_last_of( "\\" );
    if ( index == -1 )
        index = (int)str.find_last_of( "/" );
    if ( index==-1 )
        return str;
    if ( index==((int)str.length()-1) )
        return "";
    return str.substr( index+1 );
}

// ��ȡ�ļ���չ��(��.)
string StaticFunc::GetFileExt(const string& strFile)
{
	string strFileExt;

	int index = strFile.find_last_of(".");
	if (index>=0)
	{
		strFileExt = strFile.substr(index+1, strFile.size()-index);
	}

	return strFileExt;
}

string StaticFunc::ReplaceFileExt(const string& strFile,const string& strNewExt)
{
	string strNewFile = strFile;

	int index = strNewFile.find_last_of(".");
	if (index>=0)
	{
		strNewFile = strNewFile.replace(index + 1, strNewFile.size() - index,strNewExt);
	}

	return strNewFile;
}

// ·��תСд
string StaticFunc::ToLowerCase(const string& strFile)
{
	string strLower;
	strLower.assign(strFile.size(), 0);

	for(uint32_t i = 0; i < strFile.size(); i++) 
	{	
		if(strFile[i] >= 'A' && strFile[i] <= 'Z') 
		{
			strLower[i] = strFile[i] + 'a' - 'A';
		}
		else if(strFile[i] == '\\') 
		{
			strLower[i] = '/';
		}
		else 
		{
			strLower[i] = strFile[i];
		}
	}

	return strLower;
}

void StaticFunc::MakeDir( const char* pszPath )
{
	string strPath = pszPath;
	for(uint32_t i = 0; i < strPath.size(); i++) 
	{	
		if(strPath[i] == '\\') { strPath[i] = '/'; }
	}

	if (strPath[strPath.size()-1] != '/')
	{
		strPath += "/";
	}

	int nR = 0;
	do 
	{
		nR = strPath.find_first_of('/', nR);
		if (string::npos == nR)
		{
			break;
		}
		string strSub = strPath.substr(0, nR);
		nR++;
#if WIN32
		_mkdir(strSub.c_str());
#else
		mkdir(strSub.c_str(), S_IRWXU);
#endif
	} while (nR < (int)strPath.size());
}

bool StaticFunc::IsFileExist( const char* fullFilePath )
{
#ifdef WIN32
    FILE* fp = fopen( fullFilePath, "rb" );
    if ( fp == NULL )
    {
        return false;
    }

    fclose(fp);
    return true;
#else
    struct stat sys_stat;
    return (stat(fullFilePath, &sys_stat) == 0);
    //return access(fullFilePath.getData(),F_OK) == 0;
#endif
}

void StrRemoveChar(string& str, char ch )
{
    int len = str.length();
    if ( len == 0 )
        return;
    int begin = 0;
    const char* buf = str.c_str();
    for ( ; begin<len; begin++ )
    {
        if ( buf[begin] != ch )
            break;
    }
    int end = len-1;
    for ( ; end>=0; end-- )
    {
        if ( buf[end] != ch )
            break;
    }
    if ( begin==0 && end==len-1 )
        return;
    str = str.substr( begin, end+1-begin );
}

void StaticFunc::StrRemoveSpaces(string& str)
{
    StrRemoveChar(str, ' ');
}

void StaticFunc::StrSplit(const string& strSource, const string& delimiter, OUT std::vector<string>& vecResult)
{
    int nL = 0;
    string strCell = "";
    int i = 0;

    while (i < (int)strSource.size())
    {
        if (strSource.substr(i, delimiter.size()) == delimiter)
        {
            if (nL < i)
            {
                strCell = strSource.substr(nL, i - nL);
                vecResult.push_back(strCell);
            }

            i += delimiter.size();
            nL = i;
        }
        else
        {
            ++i;
        }
    }

    if (nL < i)
    {
        strCell = strSource.substr(nL, i - nL);
        vecResult.push_back(strCell);
    }
}

int StaticFunc::FileLength(FILE* pFile)
{
    fseek(pFile, 0, SEEK_END);
    int fileLen= ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    return fileLen;
}

int StaticFunc::FileRead(FILE* pFile, OUT char* buf, int len)
{
    int iLeft = len;
    while(iLeft > 0)
    {
        if (feof(pFile) || ferror(pFile))
            return len - iLeft;

        iLeft -= (int)fread(buf+(len-iLeft), 1, iLeft, pFile);
    }

    return len;
}

string StaticFunc::ToString(int n)
{
    char szBuff[64];
#ifdef WIN32
    sprintf_s(szBuff, 64, "%d", n);
#else
    sprintf(szBuff, "%d", n);
#endif
    return szBuff;
}

 void StaticFunc::StrMakePath(IN OUT string& str)
 {
    if (str.empty())
    {
        return;
    }

    char ch = str[str.length() -1];
    if (ch != '/' && ch != '\\')
    {
        str += '/';
    }
 }

 void StaticFunc::StrFormatPath(IN OUT string& str, bool url)
 {
     uint32_t len = str.length();
     if ( url )
     {
         for ( uint32_t i=0; i<len; i++ )
         {
             if ( str[i] == '\\' )
                 str[i] = '/';
         }
         std::string::size_type pos;
         while ( (pos=str.find("//")) != std::string::npos )
             str.replace( pos, 2, "/" );
     }
     else
     {
         for ( uint32_t i=0; i<len; i++ )
         {
             if ( str[i] == '/' )
                 str[i] = '\\';
         }
         std::string::size_type pos;
         while ( (pos=str.find("\\\\")) != std::string::npos )
             str.replace( pos, 2, "\\" );
     }
 }

 int StaticFunc::StrHexToInt(const string& str)
 {
     int val = 0;
     int j=0;
     std::string buff;
     bool bNega = false;
     if ( str[0] == '-' )
     {
         buff = str.substr( 1 );
         bNega = true;
     }
     else
     {	
         buff = str;
     }
     for ( int i=(int)buff.length()-1; i>=0; i--,j++ )
     {
         char ch = buff[i];
         if ( ch>='a' && ch <='f')
         {
             val += ( (ch-'a'+0xa) << (j<<2) );
         }
         else if ( ch>='A' && ch <='F' )
         {
             val += ( (ch-'A'+0xa) << (j<<2) );
         }
         else if ( ch>='0' && ch <='9' )
         {
             val += ( (ch-'0') << (j<<2) );
         }
         else
         {
             return 0;
         }
     }
     if ( bNega )
         return -val;
     else
         return val;
 }

 bool StaticFunc::StrIsEndWith(const string& str, const string& strEndWith, bool bCase)
 {
    if (str.length() < strEndWith.length())
        return false;
    
    string subStr = str.substr(str.length() - strEndWith.length());
    if (bCase)
    {
        if (subStr == strEndWith)
            return true;
        else
            return false;
    }
    else
    {
        if (strcmp(subStr.c_str(), strEndWith.c_str()) == 0)
            return true;
        else
            return false;
    }
 }

// ---------------------------------------------------------------------
// �������ת��
// ---------------------------------------------------------------------
// Ansi�ַ���ת��Unicode�ַ���
// Unicode�ַ���ת��Ansi�ַ���
string StaticFunc::UnicodeToAnsi(const wchar_t* pszUnicode)
{
#ifdef WIN32
	uint32_t unLenW = wcslen(pszUnicode);
	uint32_t unLen = MAX_STR;
	char szBuf[MAX_STR] = "";
	unLen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pszUnicode, unLenW, szBuf, unLen, NULL, NULL);
	szBuf[unLen] = '\0';
	return szBuf;
#else
	return "";
#endif
}

// Ansi�ַ���ת��Unicode�ַ���
wstring StaticFunc::AnsiToUnicode(const char* pszAnisi)
{
#ifdef WIN32
	uint32_t unLen = strlen(pszAnisi);
	uint32_t unLenW = MAX_STR;
	wchar_t szBuf[MAX_STR] = L"";
	unLenW = MultiByteToWideChar(CP_ACP, 0, pszAnisi, unLen, szBuf, unLenW);
	szBuf[unLenW] = '\0';
	return szBuf;
#else
	return L"";
#endif
}

// ---------------------------------------------------------------------
// ini��ȡ
// ---------------------------------------------------------------------
int StaticFunc::GetIniInt(const char* pszFileName, const char* pszTitle, const char* pszKey)
{
    #ifdef WIN32
	int nReturn = ::GetPrivateProfileInt(pszTitle, pszKey, 0, pszFileName);
	return nReturn;
#else
    return 0;
#endif
}

string StaticFunc::GetIniString(const char* pszFileName, const char* pszTitle, const char* pszKey)
{
	char szValue[MAX_STR_NAME] = "";
#ifdef WIN32
	::GetPrivateProfileString(pszTitle, pszKey, NULL, szValue, sizeof(szValue), pszFileName);
#endif
	return szValue;
}

// ---------------------------------------------------------------------
// ����
// ---------------------------------------------------------------------
string StaticFunc::GetMd5(const string& strBuffer)
{
	MD5 md5;
	md5.update(strBuffer);
	return md5.toString();
}
string StaticFunc::GetMd5ByFile(const char* pszFileName)
{
	ifstream in(pszFileName,std::ios::binary);
	if(!in)return " ";
	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while(!in.eof()){
		in.read(buffer,1024);
		length = in.gcount();
		if(length > 0)
			md5.update(buffer,(size_t)length);
	}
	in.close();
	return md5.toString();
}
