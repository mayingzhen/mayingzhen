#include "Assert.h"
#include "Log.h"

namespace ma
{
	void AssertMsg(bool bOK,const char* pszExpr,const char* pszFile,UINT nLine,const char* fmt,...)
	{
		if (!bOK)
		{
			const int len = 10240;
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

			char szNum[16];
			itoa(nLine, szNum, 10);
			
			DebugMsg("Assert Expr: %s, Occur: %s(%d), Desc: %s",pszExpr,pszFile,nLine,buffer);
			assert(false);
		}
	}
}




