#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main()
{

	bool bRenderThread = true;
	bool bDataThread = true;
	bool bJobScheduler = true;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bRenderThread,bDataThread,bJobScheduler);
	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
