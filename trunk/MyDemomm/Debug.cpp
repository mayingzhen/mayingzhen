#include <Windows.h>
#include <signal.h>
#include <strsafe.h>

#include "Debug.h"


#ifndef _FINAL

#define MAX_BUFFER		4096	// ���ɶ��Ա�����ʹ�õĻ�������С
#define MAX_MODULE_NAME	64		// �Ի������ܹ����̵�ģ�������ȣ������ͽض�

#ifndef _countof
#define _countof(v) (sizeof(v) / sizeof(v[0]))
#endif // _countof

///////////////////////////////////////////////////////////////////////////////
//
// _DbgAssertionFailed: ����ʧ�ܺ�Ĵ���
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

	// ȡ��ǰģ����
	cchModuleName = GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	if (cchModuleName == 0) {
		StringCchCopyA(szModuleName, _countof(szModuleName), pszUnknown);
	} else if (cchModuleName > MAX_MODULE_NAME) {
		pszModuleName += cchModuleName - MAX_MODULE_NAME;
		memcpy(pszModuleName, "...", 3); // MAX_MODULE_NAME ����̫СŶ����
	}

	// ����
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

	// ������Ϣ
	if (pszFormat && (*pszFormat != '\0')) {
		va_start(ap, pszFormat);
		StringCchVPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
			0, pszFormat, ap);
		va_end(ap);
	} else {
		StringCchCopyExA(pszBuffer, cchRemaining, "[None]", &pszBuffer,
			&cchRemaining, 0);
	}

	// HOWTO ��Ϣ
	StringCchCopyExA(pszBuffer, cchRemaining,
		"\n\nPlease inform the developers immediately, although it is possible"
		"\nfor the program to continue execution after ignoring this error."
		"\n\n(Press Retry to debug the application)",
		&pszBuffer, &cchRemaining, 0);

	// �����Ի���
	nCode = MessageBoxA(
		GetActiveWindow(),
		szBuffer,
		"Oasis Debug Library",
		MB_TASKMODAL | MB_ICONSTOP | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND
	);

	// ��ֹ
	if (nCode == IDABORT) {
		raise(SIGABRT);
		_exit(3);
	}

	// ���ԣ����� 1 ���� int 3
	if (nCode == IDRETRY)
		return 1;

	// ���ԣ����� 0 ��������
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

	// ȡ��ǰģ��������Ȼ���� ANSI �ַ�������
	cchModuleName = GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	if (cchModuleName == 0) {
		StringCchCopyA(szModuleName, _countof(szModuleName), pszUnknown);
	} else if (cchModuleName > MAX_MODULE_NAME) {
		pszModuleName += cchModuleName - MAX_MODULE_NAME;
		memcpy(pszModuleName, "...", 3); // MAX_MODULE_NAME ����̫СŶ����
	}

	// ����
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

	// ������Ϣ
	if (pszFormat && (*pszFormat != L'\0')) {
		va_start(ap, pszFormat);
		StringCchVPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
			0, pszFormat, ap);
		va_end(ap);
	} else {
		StringCchCopyExW(pszBuffer, cchRemaining, L"[None]", &pszBuffer,
			&cchRemaining, 0);
	}

	// HOWTO ��Ϣ
	StringCchCopyExW(pszBuffer, cchRemaining,
		L"\n\nPlease inform the developers immediately, although it is possible"
		L"\nfor the program to continue execution after ignoring this error."
		L"\n\n(Press Retry to debug the application)",
		&pszBuffer, &cchRemaining, 0);

	// �����Ի���
	nCode = MessageBoxW(
		GetActiveWindow(),
		szBuffer,
		L"Oasis Debug Library",
		MB_TASKMODAL | MB_ICONSTOP | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND
	);

	// ��ֹ
	if (nCode == IDABORT) {
		raise(SIGABRT);
		_exit(3);
	}

	// ���ԣ����� 1 ���� int 3
	if (nCode == IDRETRY)
		return 1;

	// ���ԣ����� 0 ��������
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// _DbgBreak: ����һ�� int 3 �ж�
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgBreak() throw()
{
	DebugBreak();
}

///////////////////////////////////////////////////////////////////////////////
//
// _DbgTraceV: �� Debugger ��������ַ���
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgTraceV(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const char *pszFormat, va_list ap) throw()
{
	char	szBuffer[MAX_BUFFER];
	char	*pszBuffer = szBuffer;
	size_t	cchRemaining = _countof(szBuffer);

	// ���ǰ׺��"�ļ���(�к�) : ������: "
	StringCchPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		"%s(%d) : %s: ",
		(pszFileName ? pszFileName : "[Unknown File]"),
		nLineNo,
		(pszFunctionName ? pszFunctionName : "[Unknown Function]"));

	// ���� TRACE �ı�
	StringCchVPrintfExA(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
		STRSAFE_IGNORE_NULLS, pszFormat, ap);

	// ���
	OutputDebugStringA(szBuffer);
}

void __cdecl _DbgTraceV(const char *pszFileName,
						   const char *pszFunctionName, int nLineNo,
						   const wchar_t *pszFormat, va_list ap) throw()
{
	wchar_t	szBuffer[MAX_BUFFER];
	wchar_t	*pszBuffer = szBuffer;
	size_t	cchRemaining = _countof(szBuffer);

	// ���ǰ׺��"�ļ���(�к�) : ������: "
	StringCchPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining, 0,
		L"%S(%d) : %S: ", // pszFileName �� pszFunctionName ��Ȼ�� char
		(pszFileName ? pszFileName : "[Unknown File]"),
		nLineNo,
		(pszFunctionName ? pszFunctionName : "[Unknown Function]"));

	// ���� TRACE �ı�
	StringCchVPrintfExW(pszBuffer, cchRemaining, &pszBuffer, &cchRemaining,
		STRSAFE_IGNORE_NULLS, pszFormat, ap);

	// ���
	OutputDebugStringW(szBuffer);
}

#endif // _FINAL