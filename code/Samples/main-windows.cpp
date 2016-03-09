#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	SampleBrowser sampleBrowser("SampleBrowser");

	Platform app;

	bool bRenderThread = true;
	bool bDataThread = false;
	bool bJobScheduler = true;

	app.Init(bRenderThread,bDataThread,bJobScheduler);
	app.Run();
	app.Shutdown();

	return 1;
}
