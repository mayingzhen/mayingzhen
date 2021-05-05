
#include "microprofile.h"

namespace ma
{
	#define ProfileInit() MicroProfileOnThreadCreate("MainThread"); MicroProfileSetEnableAllGroups(true);
	#define ProfileOnThreadCreate(name) MicroProfileOnThreadCreate(name)
	#define ProfileUpdateFrame() MicroProfileFlip(nullptr)
	#define ROFILE_SCOPEI(name, color) MICROPROFILE_SCOPEI("", name, color);

}