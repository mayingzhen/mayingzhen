#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

//#include <tchar.h>

///////////////////////////////////////////////////////////////////////////////
//
// ȡ��֮ǰ�Ķ���
//
///////////////////////////////////////////////////////////////////////////////
#undef assert				// ANSI ��׼����
#undef ASSERT				// MFC ����
#undef _ASSERT				// CRT ����
#undef _ASSERTE				// CRT ����
#undef VERIFY				// MFC У��
#undef TRACE				// MFC Trace
#undef TRACE0				// MFC Trace
#undef TRACE1				// MFC Trace
#undef TRACE2				// MFC Trace
#undef TRACE3				// MFC Trace

///////////////////////////////////////////////////////////////////////////////
//
// �� ASNI �� CRT ���Ժ�ת�����ǵĶ���
//
///////////////////////////////////////////////////////////////////////////////
#define assert		ASSERT
#define _ASSERT		ASSERT
#define _ASSERTE	ASSERT

///////////////////////////////////////////////////////////////////////////////
//
// �������� CRT Debug Report ��������
//
///////////////////////////////////////////////////////////////////////////////
#pragma deprecated("_CrtDbgReport")

#ifdef _FINAL

///////////////////////////////////////////////////////////////////////////////
//
// Final �棬�ر����е��Ժ�
//
///////////////////////////////////////////////////////////////////////////////

#define ASSERT(expr) ((void)0)
#define ASSERTEX(expr, msg) ((void)0)
#define ASSERTEX1(expr, format, a) ((void)0)
#define ASSERTEX2(expr, format, a1, a2) ((void)0)
#define ASSERTEX3(expr, format, a1, a2, a3) ((void)0)
#define ASSERTEX4(expr, format, a1, a2, a3, a4) ((void)0)
#define ASSERTEX5(expr, format, a1, a2, a3, a4, a5) ((void)0)
#define ASSERTEX6(expr, format, a1, a2, a3, a4, a5, a6) ((void)0)
#define ASSERTEX7(expr, format, a1, a2, a3, a4, a5, a6, a7) ((void)0)
#define ASSERTEX8(expr, format, a1, a2, a3, a4, a5, a6, a7, a8) ((void)0)
#define ASSERTEX9(expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9) ((void)0)

#define VERIFY(expr) ((void)(expr))
#define VERIFYEX(expr, msg) ((void)(expr))
#define VERIFYEX1(expr, format, a) ((void)(expr))
#define VERIFYEX2(expr, format, a1, a2) ((void)(expr))
#define VERIFYEX3(expr, format, a1, a2, a3) ((void)(expr))
#define VERIFYEX4(expr, format, a1, a2, a3, a4) ((void)(expr))
#define VERIFYEX5(expr, format, a1, a2, a3, a4, a5) ((void)(expr))
#define VERIFYEX6(expr, format, a1, a2, a3, a4, a5, a6) ((void)(expr))
#define VERIFYEX7(expr, format, a1, a2, a3, a4, a5, a6, a7) ((void)(expr))
#define VERIFYEX8(expr, format, a1, a2, a3, a4, a5, a6, a7, a8) ((void)(expr))
#define VERIFYEX9(expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9) ((void)(expr))

#define TRACE0(msg)
#define TRACE1(format, a)
#define TRACE2(format, a1, a2)
#define TRACE3(format, a1, a2, a3)
#define TRACE4(format, a1, a2, a3, a4)
#define TRACE5(format, a1, a2, a3, a4, a5)
#define TRACE6(format, a1, a2, a3, a4, a5, a6)
#define TRACE7(format, a1, a2, a3, a4, a5, a6, a7)
#define TRACE8(format, a1, a2, a3, a4, a5, a6, a7, a8)
#define TRACE9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define TRACE		__noop

#else // _FINAL

///////////////////////////////////////////////////////////////////////////////
//
// �� Final �棬�����е��Ժ�
//
///////////////////////////////////////////////////////////////////////////////

#define __ASSERT_BASE(args) \
	(1 != _DbgAssertionFailed args) || (_DbgBreak(), 0)

#define ASSERT(expr) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, (const char*)NULL)))

#define ASSERTEX(expr, msg) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, msg)))

#define ASSERTEX1(expr, format, a) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a)))

#define ASSERTEX2(expr, format, a1, a2) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2)))

#define ASSERTEX3(expr, format, a1, a2, a3) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3)))

#define ASSERTEX4(expr, format, a1, a2, a3, a4) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4)))

#define ASSERTEX5(expr, format, a1, a2, a3, a4, a5) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4, a5)))

#define ASSERTEX6(expr, format, a1, a2, a3, a4, a5, a6) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4, a5, a6)))

#define ASSERTEX7(expr, format, a1, a2, a3, a4, a5, a6, a7) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4, a5, a6, a7)))

#define ASSERTEX8(expr, format, a1, a2, a3, a4, a5, a6, a7, a8) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4, a5, a6, a7, a8)))

#define ASSERTEX9(expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
	(void) ((expr) || \
			__ASSERT_BASE((__FILE__, __FUNCTION__ "()", __LINE__, #expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)))

#define VERIFY(expr) \
	ASSERT(expr)

#define VERIFYEX(expr, msg) \
	ASSERTEX(expr, msg)

#define VERIFYEX1(expr, format, a) \
	ASSERTEX1(expr, format, a)

#define VERIFYEX2(expr, format, a1, a2) \
	ASSERTEX2(expr, format, a1, a2)

#define VERIFYEX3(expr, format, a1, a2, a3) \
	ASSERTEX3(expr, format, a1, a2, a3)

#define VERIFYEX4(expr, format, a1, a2, a3, a4) \
	ASSERTEX4(expr, format, a1, a2, a3, a4)

#define VERIFYEX5(expr, format, a1, a2, a3, a4, a5) \
	ASSERTEX5(expr, format, a1, a2, a3, a4, a5)

#define VERIFYEX6(expr, format, a1, a2, a3, a4, a5, a6) \
	ASSERTEX6(expr, format, a1, a2, a3, a4, a5, a6)

#define VERIFYEX7(expr, format, a1, a2, a3, a4, a5, a6, a7) \
	ASSERTEX7(expr, format, a1, a2, a3, a4, a5, a6, a7)

#define VERIFYEX8(expr, format, a1, a2, a3, a4, a5, a6, a7, a8) \
	ASSERTEX8(expr, format, a1, a2, a3, a4, a5, a6, a7, a8)

#define VERIFYEX9(expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
	ASSERTEX9(expr, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define TRACE0(msg) \
	TRACE(_T(msg))

#define TRACE1(format, a) \
	TRACE(_T(format), a)

#define TRACE2(format, a1, a2) \
	TRACE(_T(format), a1, a2)

#define TRACE3(format, a1, a2, a3) \
	TRACE(_T(format), a1, a2, a3)

#define TRACE4(format, a1, a2, a3, a4) \
	TRACE(_T(format), a1, a2, a3, a4)

#define TRACE5(format, a1, a2, a3, a4, a5) \
	TRACE(_T(format), a1, a2, a3, a4, a5)

#define TRACE6(format, a1, a2, a3, a4, a5, a6) \
	TRACE(_T(format), a1, a2, a3, a4, a5, a6)

#define TRACE7(format, a1, a2, a3, a4, a5, a6, a7) \
	TRACE(_T(format), a1, a2, a3, a4, a5, a6, a7)

#define TRACE8(format, a1, a2, a3, a4, a5, a6, a7, a8) \
	TRACE(_T(format), a1, a2, a3, a4, a5, a6, a7, a8)

#define TRACE9(format, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
	TRACE(_T(format), a1, a2, a3, a4, a5, a6, a7, a8, a9)

///////////////////////////////////////////////////////////////////////////////
//
// _DbgAssertionFailed: ����ʧ�ܺ�Ĵ�����������Ի���
//
// ����:
//
//		pszFileName				��������ʧ�ܵ��ļ�����NULL �Ϸ�
//
//		pszFunctionName			��������ʧ�ܵĺ�������NULL �Ϸ�
//
//		nLineNo					��������ʧ�ܵ��к�
//
//		pszExpression			��������ʧ�ܵı��ʽ��NULL �Ϸ�
//
//		pszFormat				��ʽ���ַ�����NULL �Ϸ�
//
//		...						��ʽ������
//
// ����ֵ:
//
//		0						�û�ѡ���ˡ����ԡ���Ӧ�ü���ִ��
//
//		1						�û�ѡ���ˡ����ԡ���Ӧ���������� Debugger
//
// ˵��:
//
//		�ڶ���ʧ�ܺ���øú������ᵯ������Ի�����ʾ�û�����ѡ��Final �� Base
//		���н��������˺��������Խ���ͨ��������չ��Ϊ�ú����ṩ�󲿷ֲ���������
//		��ֱ�ӵ��ã���Ӧʹ�ö��Ժꡣ
//
///////////////////////////////////////////////////////////////////////////////
int __cdecl _DbgAssertionFailed(
	const char		*pszFileName,
	const char		*pszFunctionName,
	int				nLineNo,
	const char		*pszExpression,
	const char		*pszFormat,
	...
) throw();

int __cdecl _DbgAssertionFailed(
	const char		*pszFileName,
	const char		*pszFunctionName,
	int				nLineNo,
	const char		*pszExpression,
	const wchar_t	*pszFormat,
	...
) throw();

///////////////////////////////////////////////////////////////////////////////
//
// _DbgBreak: ����һ�� int 3 �ж�
//
// ����:
//
//		��
//
// ����ֵ:
//
//		��
//
// ˵��:
//
//		_DbgAssertionFailed ���� 1 ����øú�������һ�������жϣ��Ӷ�����
//		Debugger�����û�� Debugger �����ڵ�ǰ�Ľ����ϣ������δ������쳣����
//		�½����ڶ���ʧ�ܴ�����������Final �� Base ���н��������˺��������Բ���
//		��ֱ�ӵ��ã���Ӧʹ�ö��Ժ괦���ⲿ���߼���
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgBreak() throw();

///////////////////////////////////////////////////////////////////////////////
//
// _DbgTraceV: �� Debugger ��������ַ���
//
// ����:
//
//		pszFileName				��������ʧ�ܵ��ļ�����NULL �Ϸ�
//
//		pszFunctionName			��������ʧ�ܵĺ�������NULL �Ϸ�
//
//		nLineNo					��������ʧ�ܵ��к�
//
//		pszFormat				��ʽ���ַ�����NULL �Ϸ�
//
//		ap						������
//
// ����ֵ:
//
//		��
//
// ˵��:
//
//		��ʽ��һ���ı���Ϣ������� Debugger ��������ڡ����û�� Debugger ����
//		�ڵ�ǰ�Ľ����ϣ��򱾴κ������ò������κ�Ч����Final �� Base ���н�����
//		���˺��������Բ�����ֱ�ӵ��ã���Ӧʹ�� TRACE �ꡣ
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgTraceV(
	const char		*pszFileName,
	const char		*pszFunctionName,
	int				nLineNo,
	const char		*pszFormat,
	va_list			ap
) throw();

void __cdecl _DbgTraceV(
	const char		*pszFileName,
	const char		*pszFunctionName,
	int				nLineNo,
	const wchar_t	*pszFormat,
	va_list			ap
) throw();

///////////////////////////////////////////////////////////////////////////////
//
// �ṩ TRACE(format[, ...]) ֧��
//
///////////////////////////////////////////////////////////////////////////////
#define TRACE _TraceHelper(__FILE__, __FUNCTION__ "()", __LINE__)

class _TraceHelper
{
private:
	const char	*m_pszFileName;
	const char	*m_pszFunctionName;
	const int	m_nLineNo;

public:
	_TraceHelper(const char *pszFileName, const char *pszFunctionName,
					int nLineNo) throw()
		: m_pszFileName(pszFileName)
		, m_pszFunctionName(pszFunctionName)
		, m_nLineNo(nLineNo)
	{ // ���죬��ס������չ������Ĳ���
	}

	template <class _CharT>
	void __cdecl operator()(const _CharT *pszFormat, ...) const throw()
	{ // ��ʽ�����
		va_list ap; va_start(ap, pszFormat);
		::_DbgTraceV(m_pszFileName, m_pszFunctionName, m_nLineNo,
			pszFormat, ap);
		va_end(ap);
	}
};

#endif // _FINAL

#endif // __DEBUG_H__