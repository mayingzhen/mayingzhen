#ifndef __ASSERT_H__
#define __ASSERT_H__

#define DEBUG_BACKPOINT			0
#define DEBUG_IGNORE			1
#define DEBUG_CONTINUE			2

#define debug_assert(x, comment)	\
{									\
	static bool bIgnore = false;	\
	if (!bIgnore && !(x))			\
	{								\
		switch (DisplayError("ASSERT FAILED!", #x, comment, __FILE__, __LINE__)) \
		{							\
		case DEBUG_IGNORE:			\
			bIgnore = true;         \
			break;					\
		case DEBUG_BACKPOINT:		\
			__asm {int 3};          \
			break;					\
		}							\
	}								\
}

COMMON_API void AssertMsg(bool bOK, const char* pszExpr, const char* pszFile, 
						  const char* pszFunction,Uint nLine,const char* fmt,...);


#define ASSERT(expr) debug_assert(expr,"")
#define ASSERT_MSG(expr,MSG) debug_assert(expr,MSG)
//#define ASSERT(expr) AssertMsg((bool)(expr), #expr , __FILE__ , __FUNCTION__, __LINE__ ,NULL)
//#define ASSERT_MSG(expr,MSG) AssertMsg((bool)(expr), #expr , __FILE__ , __FUNCTION__, __LINE__ ,MSG)

#endif