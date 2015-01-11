#ifndef __ASSERT_H__
#define __ASSERT_H__


#define ASSERT_ENABLE

#if defined(ASSERT_ENABLE) 
	#define ASSERT(x, ...) AssertMsg(!!(x), #x , __FILE__ , __LINE__ ,NULL)
#else
	#define ASSERT(x, ...) 
#endif


namespace ma
{
	COMMON_API void AssertMsg(bool bOK,const char* pszExpr,const char* pszFile,UINT nLine,const char* fmt,...);
}

#endif
