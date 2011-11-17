#include <Windows.h>
#include <signal.h>
#include <strsafe.h>

#include "Debug.h"


#ifndef _FINAL

#define MAX_BUFFER		4096	// 生成断言报告所使用的缓冲区大小
#define MAX_MODULE_NAME	64		// 对话框中能够容忍的模块名长度，超过就截断

#ifndef _countof
#define _countof(v) (sizeof(v) / sizeof(v[0]))
#endif // _countof

///////////////////////////////////////////////////////////////////////////////
//
// _DbgAssertionFailed: 断言失败后的处理
//
///////////////////////////////////////////////////////////////////////////////
int _DbgAssertionFailed(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const char *pszExpression,
						   const char *pszFormat, ...) throw()
{
	char				szBuffer[MAX_BUFFER];
	char				*pszBuffer = szBuffer;
	size_t				cchRemaining = _countof(szBuffer);
	char				szModuleName[MAX_PATH + 1];
	char				*pszModuleName = szModuleName;
	size_t				cchModuleName;
	va_list				ap;
	int					nCode;
	static const char	*pszUnknown = "[Unknown]";

	// 取当前模块名
	cchModuleName = GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	if (cchModuleName == 0) {
		StringCchCopyA(szModuleName, _countof(szModuleName), pszUnknown);
	} else if (cchModuleName > MAX_MODULE_NAME) {
		pszModuleName += cchModuleName - MAX_MODULE_NAME;
		memcpy(pszModuleName, "...", 3); // MAX_MODULE_NAME 不能太小哦……
	}

	// 标题
	StringCchPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		"%s Assertion failed!\n\nModule: %s\nFile: %s\nLine: %d\nFunction: %s"
		"\n\nExpression: %s\nDescription: ",
#ifdef _DEBUG
		"Debug",
#else // _DEBUG
		"Release",
#endif // _DEBUG
		pszModuleName,
		(pszFileName ? pszFileName : pszUnknown),
		nLineNo,
		(pszFunctionName ? pszFunctionName : pszUnknown),
		(pszExpression ? pszExpression : pszUnknown));

	// 描述信息
	if (pszFormat && (*pszFormat != '\0')) {
		va_start(ap, pszFormat);
		StringCchVPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
			0, pszFormat, ap);
		va_end(ap);
	} else {
		StringCchCopyExA(pszBuffer, cchRemaining, "[None]", &pszBuffer,
			&cchRemaining, 0);
	}

	// HOWTO 信息
	StringCchCopyExA(pszBuffer, cchRemaining,
		"\n\nPlease inform the developers immediately, although it is possible"
		"\nfor the program to continue execution after ignoring this error."
		"\n\n(Press Retry to debug the application)",
		&pszBuffer, &cchRemaining, 0);

	// 弹出对话框
	nCode = MessageBoxA(
		GetActiveWindow(),
		szBuffer,
		"Oasis Debug Library",
		MB_TASKMODAL | MB_ICONSTOP | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND
	);

	// 终止
	if (nCode == IDABORT) {
		raise(SIGABRT);
		_exit(3);
	}

	// 重试，返回 1 引发 int 3
	if (nCode == IDRETRY)
		return 1;

	// 忽略，返回 0 继续运行
	return 0;
}

int _DbgAssertionFailed(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const char *pszExpression,
						   const wchar_t *pszFormat, ...) throw()
{
	wchar_t				szBuffer[MAX_BUFFER];
	wchar_t				*pszBuffer = szBuffer;
	size_t				cchRemaining = _countof(szBuffer);
	char				szModuleName[MAX_PATH + 1];
	char				*pszModuleName = szModuleName;
	size_t				cchModuleName;
	va_list				ap;
	int					nCode;
	static const char	*pszUnknown = "[Unknown]";

	// 取当前模块名，仍然按照 ANSI 字符串处理
	cchModuleName = GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	if (cchModuleName == 0) {
		StringCchCopyA(szModuleName, _countof(szModuleName), pszUnknown);
	} else if (cchModuleName > MAX_MODULE_NAME) {
		pszModuleName += cchModuleName - MAX_MODULE_NAME;
		memcpy(pszModuleName, "...", 3); // MAX_MODULE_NAME 不能太小哦……
	}

	// 标题
	StringCchPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		L"%s Assertion failed!\n\nModule: %S\nFile: %S\nLine: %d\nFunction: %S"
		L"\n\nExpression: %S\nDescription: ",
#ifdef _DEBUG
		L"Debug",
#else // _DEBUG
		L"Release",
#endif // _DEBUG
		pszModuleName,
		(pszFileName ? pszFileName : pszUnknown),
		nLineNo,
		(pszFunctionName ? pszFunctionName : pszUnknown),
		(pszExpression ? pszExpression : pszUnknown));

	// 描述信息
	if (pszFormat && (*pszFormat != L'\0')) {
		va_start(ap, pszFormat);
		StringCchVPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
			0, pszFormat, ap);
		va_end(ap);
	} else {
		StringCchCopyExW(pszBuffer, cchRemaining, L"[None]", &pszBuffer,
			&cchRemaining, 0);
	}

	// HOWTO 信息
	StringCchCopyExW(pszBuffer, cchRemaining,
		L"\n\nPlease inform the developers immediately, although it is possible"
		L"\nfor the program to continue execution after ignoring this error."
		L"\n\n(Press Retry to debug the application)",
		&pszBuffer, &cchRemaining, 0);

	// 弹出对话框
	nCode = MessageBoxW(
		GetActiveWindow(),
		szBuffer,
		L"Oasis Debug Library",
		MB_TASKMODAL | MB_ICONSTOP | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND
	);

	// 终止
	if (nCode == IDABORT) {
		raise(SIGABRT);
		_exit(3);
	}

	// 重试，返回 1 引发 int 3
	if (nCode == IDRETRY)
		return 1;

	// 忽略，返回 0 继续运行
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// _DbgBreak: 引发一个 int 3 中断
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgBreak() throw()
{
	DebugBreak();
}

///////////////////////////////////////////////////////////////////////////////
//
// _DbgTraceV: 向 Debugger 窗口输出字符串
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgTraceV(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const char *pszFormat, va_list ap) throw()
{
	char	szBuffer[MAX_BUFFER];
	char	*pszBuffer = szBuffer;
	size_t	cchRemaining = _countof(szBuffer);

	// 输出前缀："文件名(行号) : 函数名: "
	StringCchPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		"%s(%d) : %s: ",
		(pszFileName ? pszFileName : "[Unknown File]"),
		nLineNo,
		(pszFunctionName ? pszFunctionName : "[Unknown Function]"));

	// 接上 TRACE 文本
	StringCchVPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
		STRSAFE_IGNORE_NULLS, pszFormat, ap);

	// 输出
	OutputDebugStringA(szBuffer);
}

void __cdecl _DbgTraceV(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const wchar_t *pszFormat, va_list ap) throw()
{
	wchar_t	szBuffer[MAX_BUFFER];
	wchar_t	*pszBuffer = szBuffer;
	size_t	cchRemaining = _countof(szBuffer);

	// 输出前缀："文件名(行号) : 函数名: "
	StringCchPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		L"%S(%d) : %S: ", // pszFileName 和 pszFunctionName 仍然是 char
		(pszFileName ? pszFileName : "[Unknown File]"),
		nLineNo,
		(pszFunctionName ? pszFunctionName : "[Unknown Function]"));

	// 接上 TRACE 文本
	StringCchVPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
		STRSAFE_IGNORE_NULLS, pszFormat, ap);

	// 输出
	OutputDebugStringW(szBuffer);
}

#endif // _FINAL