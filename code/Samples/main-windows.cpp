#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	Setting set;
	set.bForceGLES = false;
	set.bRenderThread = false;
	set.bDataThread = false;
	set.bJobScheduler = true;

	SampleBrowser sampleBrowser("SampleBrowser", set, "SampleMaterial");

	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
