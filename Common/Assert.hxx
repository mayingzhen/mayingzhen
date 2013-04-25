#include "Assert.h"

void AssertMsg(bool bOK,const char* pszExpr,const char* pszFile,
			   const char* pszFunction,Uint nLine,const char* fmt,...)
{
	if (!bOK)
	{
		const int len = 1024;
		char buffer[len];

		if (fmt)
		{
			va_list args;
			va_start( args, fmt );
			vsprintf( buffer, fmt, args );
			va_end(args);
		}
		else
		{
			buffer[0] = '\0';
		}

#ifdef _DEBUG
		assert(false);
#else
		Log("%s(%d)%s : Assertion Fail\n\
			expr : %s\n\
			msg : %s\n"
			,pszFile
			,nLine
			,pszFunction
			,pszExpr
			,buffer);
#endif

	}
}
