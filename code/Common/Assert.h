#ifndef __ASSERT_H__
#define __ASSERT_H__


#define ASSERT_ENABLE

#if defined(ASSERT_ENABLE) 
	#define ASSERT(x) AssertMsg(!!(x), #x , __FILE__ , __LINE__ ,NULL)
	#define ASSERTMSG(x,fmt) AssertMsg(!!(x), #x , __FILE__ , __LINE__ ,fmt)
	#define ASSERTMSG1(x,fmt,parm) AssertMsg(!!(x), #x , __FILE__ , __LINE__ ,fmt,parm)
	#define ASSERTMSG2(x,fmt,parm1,parm2) AssertMsg(!!(x), #x , __FILE__ , __LINE__ ,fmt,parm1,parm2)
#else
	#define ASSERT(x, ...) 
	#define ASSERTMSG(x,fmt,...)
	#define ASSERTMSG1(x,fmt,parm)
	#define ASSERTMSG2(x,fmt,parm1,parm2)
#endif


namespace ma
{
	void AssertMsg(bool bOK,const char* pszExpr,const char* pszFile,UINT nLine,const char* fmt,...);
}

#endif
