#ifndef __ASSERT_H__
#define __ASSERT_H__


COMMON_API void AssertMsg(bool bOK, const char* pszExpr, const char* pszFile, 
						  const char* pszFunction,Uint nLine,const char* fmt,...);

// inline void ASSERT_MSG(bool expr,const char* fmt,...)
// {
// 	va_list args;
// 	va_start( args, fmt );
// 	AssertMsg(expr,"","Unknown",0,fmt,args);
// }

#define ASSERT(expr) AssertMsg((bool)(expr), #expr , __FILE__ , __FUNCTION__, __LINE__ ,NULL)
#define ASSERT_MSG(expr,MSG) AssertMsg((bool)(expr), #expr , __FILE__ , __FUNCTION__, __LINE__ ,MSG)

#endif