#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	bool bRenderThread = false;
	bool bDataThread = false;
	bool bJobScheduler = false;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bRenderThread,bDataThread,bJobScheduler,"SampleAnimationIK");
	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
