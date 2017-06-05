#pragma once

#define USE_MY_ASSERT

//-----------------------------------------------------------------------------------------------------
// Use like this:
// MY_ASSERT(expression);
// MY_ASSERT_MESSAGE(expression,"Useful message");
// MY_ASSERT_TRACE(expression,("This should never happen because parameter %d named %s is %f",iParameter,szParam,fValue));
//-----------------------------------------------------------------------------------------------------
#if defined(USE_MY_ASSERT) && defined(WIN32) && !defined(WIN64)
//#pragma message("CryAssert enabled.")
void MyAssertTrace(const char *, ...);
bool MyAssert(const char *, const char *, unsigned int, bool *);
#define DEBUG_BREAK _asm { int 3 }
#define MY_ASSERT(condition) MY_ASSERT_MESSAGE(condition,NULL)
#define MY_ASSERT_MESSAGE(condition,message) MY_ASSERT_TRACE(condition,(message))

#define MY_ASSERT_TRACE(condition,parenthese_message)					\
    do														            \
    {															        \
        static bool s_bIgnoreAssert = false;					        \
        if(!s_bIgnoreAssert && !(condition))				            \
        {														        \
            MyAssertTrace parenthese_message;					        \
            if(MyAssert(#condition,__FILE__,__LINE__,&s_bIgnoreAssert)) \
            {									                        \
                 DEBUG_BREAK;								            \
            }												            \
        }														        \
    } while(0)

#else

#ifdef __APPLE__
#include <assert.h>
//#define ASSERT assert
#define chSTR2(x) #x
#define chSTR(x) chSTR2(x)
#define MY_ASSERT(condition) if(!(condition)) printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) "\n"); assert(condition);
#define MY_ASSERT_MESSAGE(condition,message) if(!(condition)) printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) " MESSAGE:" message "\n")
#define MY_ASSERT_TRACE(condition,parenthese_message) if(!(condition)) {printf("[Assertion Failed] CONDITION:(" #condition")" " FILE:" __FILE__ " LINE:" chSTR(__LINE__) " MESSAGE:"); printf parenthese_message; printf("\n");}
#else
#include <assert.h>
#define MY_ASSERT(condition) assert(condition)
#define MY_ASSERT_MESSAGE(condition,message) assert(condition)
#define MY_ASSERT_TRACE(condition,parenthese_message) assert(condition)
#endif

#endif

//#undef assert
//#define assert MY_ASSERT
#undef ASSERT
#define ASSERT MY_ASSERT
#define ASSERTMSG MY_ASSERT_MESSAGE

