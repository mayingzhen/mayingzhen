#pragma once

#if defined(_WIN32)
#include "microprofile.h"
#include "microprofileui.h"
#elif defined(__ANDROID__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#else

#endif

//#define PROFILE 1


namespace ma
{
#if defined(_WIN32)
	inline void SystraceInit()
	{
		MicroProfileToggleDisplayMode();
		MicroProfileInitUI();
		MicroProfileOnThreadCreate("Main");
	}

#elif defined(__ANDROID__)
	int g_trace_marker_fd = -1;

	int void SystraceInit()
	{
		g_trace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);
		if (g_trace_marker_fd == -1)
		{
			LogError("SystraceInit: failed to open /sys/kernel/debug/tracing/trace_marker!");
		}
		else
		{
			LogWarning("Succeeded to open /sys/kernel/debug/tracing/trace_marker, we are running in debug mode!");
		}
	}

	class Systrace
	{
	public:
		Systrace(const char* name)
		{
			if (g_trace_marker_fd != -1)
			{
				char buf[256];
				int len = snprintf(buf, 256, "B|%d|%s", getpid(), name);
				write(g_trace_marker_fd, buf, len);
			}
		}

		~Systrace()
		{
			if (g_trace_marker_fd != -1)
			{
				const char c = 'E';
				write(g_trace_marker_fd, &c, 1);
			}
		}
	};

#else
	inline void SystraceInit() {}
#endif



#ifdef PROFILE
	#if defined(_WIN32)
		#define SYSTRACE_INIT SystraceInit
		#define SYSTRACE(var) MICROPROFILE_SCOPE(var)
	#elif defined(__ANDROID__)
		#define SYSTRACE_INIT SystraceInit
		#define SYSTRACE(name) Systrace systrace_##name(#name)
	#else
		#define SYSTRACE_INIT ((void)0)
		#define SYSTRACE(name) ((void)0)
	#endif
#else
	#define SYSTRACE_INIT ((void)0)
	#define SYSTRACE(name) ((void)0)
#endif

}


