#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	bool bForceGLES = true;
	bool bRenderThread = false;
	bool bDataThread = false;
	bool bJobScheduler = false;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bForceGLES,bRenderThread,bDataThread,bJobScheduler,"SampleParticle");
	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
