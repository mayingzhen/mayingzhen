#ifndef __ASSERT_H__
#define __ASSERT_H__

COMMON_API int AssertMsg(bool bOK, const char* exper, const char* description, 
								const char* file, int line);





#define ASSERT_ENABLE

#if defined(ASSERT_ENABLE) 
	
	#if PLATFORM_WIN == 1

		#define DEBUG_BACKPOINT			0
		#define DEBUG_IGNORE			1
		#define DEBUG_CONTINUE			2

		#define debug_assert(x, comment)	\
		{									\
			static bool bIgnore = false;	\
			if (!bIgnore && !(x))					\
			{								\
				switch (AssertMsg(!!(x), #x, comment, __FILE__, __LINE__)) \
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
		
		#define ASSERT(expr) debug_assert(expr,"")
		#define ASSERT_MSG(expr,MSG) debug_assert(expr,MSG)

		// Error macro.
		#define GP_ERROR(...) do \
		{ \
			ASSERT(false); \
			std::exit(-1); \
		} while (0)

	#elif PLATFORM_ANDROID == 1

		#define ASSERT(expr) AssertMsg(!!(expr),#expr,"",__FILE__, __LINE__)
		#define ASSERT_MSG(expr,MSG) AssertMsg(!!(expr),#expr,MSG,__FILE__, __LINE__)
	
	#else
		
		#define ASSERT(expr) 
		#define ASSERT_MSG(expr,MSG)

	#endif	//PLATFORM_WIN == 1


#else	//#if defined(ASSERT_ENABLE) 

	#define ASSERT(x) 
	#define ASSERT_MSG(expr,MSG)

#endif





#endif
