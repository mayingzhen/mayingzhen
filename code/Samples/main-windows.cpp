#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	bool bForceGLES = false;
	bool bRenderThread = false;
	bool bDataThread = false;
	bool bJobScheduler = false;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bForceGLES,bRenderThread,bDataThread,bJobScheduler,"SampleAnimationRetarget");
	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
