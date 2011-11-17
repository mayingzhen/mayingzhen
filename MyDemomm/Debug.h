#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

//#include <tchar.h>

///////////////////////////////////////////////////////////////////////////////
//
// 取消之前的定义
//
///////////////////////////////////////////////////////////////////////////////
#undef assert				// ANSI 标准断言
#undef ASSERT				// MFC 断言
#undef _ASSERT				// CRT 断言
#undef _ASSERTE				// CRT 断言
#undef VERIFY				// MFC 校验
#undef TRACE				// MFC Trace
#undef TRACE0				// MFC Trace
#undef TRACE1				// MFC Trace
#undef TRACE2				// MFC Trace
#undef TRACE3				// MFC Trace

///////////////////////////////////////////////////////////////////////////////
//
// 将 ASNI 和 CRT 断言宏转到我们的定义
//
///////////////////////////////////////////////////////////////////////////////
#define assert		ASSERT
#define _ASSERT		ASSERT
#define _ASSERTE	ASSERT

///////////////////////////////////////////////////////////////////////////////
//
// 不建议与 CRT Debug Report 函数混用
//
///////////////////////////////////////////////////////////////////////////////
#pragma deprecated("_CrtDbgReport")

#ifdef _FINAL

///////////////////////////////////////////////////////////////////////////////
//
// Final 版，关闭所有调试宏
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
// 非 Final 版，打开所有调试宏
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
// _DbgAssertionFailed: 断言失败后的处理，弹出警告对话框
//
// 参数:
//
//		pszFileName				发生断言失败的文件名，NULL 合法
//
//		pszFunctionName			发生断言失败的函数名，NULL 合法
//
//		nLineNo					发生断言失败的行号
//
//		pszExpression			发生断言失败的表达式，NULL 合法
//
//		pszFormat				格式化字符串，NULL 合法
//
//		...						格式化参数
//
// 返回值:
//
//		0						用户选择了“忽略”，应该继续执行
//
//		1						用户选择了“重试”，应该立即唤起 Debugger
//
// 说明:
//
//		在断言失败后调用该函数，会弹出警告对话框提示用户进行选择。Final 版 Base
//		库中将不包括此函数，所以建议通过辅助宏展开为该函数提供大部分参数，不建
//		议直接调用，而应使用断言宏。
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
// _DbgBreak: 引发一个 int 3 中断
//
// 参数:
//
//		无
//
// 返回值:
//
//		无
//
// 说明:
//
//		_DbgAssertionFailed 返回 1 后调用该函数引发一个调试中断，从而唤起
//		Debugger。如果没有 Debugger 附加在当前的进程上，则会因未捕获的异常而导
//		致进程在断言失败处立即崩溃。Final 版 Base 库中将不包括此函数，所以不建
//		议直接调用，而应使用断言宏处理这部分逻辑。
//
///////////////////////////////////////////////////////////////////////////////
void __cdecl _DbgBreak() throw();

///////////////////////////////////////////////////////////////////////////////
//
// _DbgTraceV: 向 Debugger 窗口输出字符串
//
// 参数:
//
//		pszFileName				发生断言失败的文件名，NULL 合法
//
//		pszFunctionName			发生断言失败的函数名，NULL 合法
//
//		nLineNo					发生断言失败的行号
//
//		pszFormat				格式化字符串，NULL 合法
//
//		ap						参数表
//
// 返回值:
//
//		无
//
// 说明:
//
//		格式化一段文本信息并输出到 Debugger 的输出窗口。如果没有 Debugger 附加
//		在当前的进程上，则本次函数调用不产生任何效果。Final 版 Base 库中将不包
//		括此函数，所以不建议直接调用，而应使用 TRACE 宏。
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
// 提供 TRACE(format[, ...]) 支持
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
	{ // 构造，记住辅助宏展开后传入的参数
	}

	template <class _CharT>
	void __cdecl operator()(const _CharT *pszFormat, ...) const throw()
	{ // 格式化输出
		va_list ap; va_start(ap, pszFormat);
		::_DbgTraceV(m_pszFileName, m_pszFunctionName, m_nLineNo,
			pszFormat, ap);
		va_end(ap);
	}
};

#endif // _FINAL

#endif // __DEBUG_H__