#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	Setting set;
	set.eRenderType = RenderDevice_VULKAN;
	set.bRenderThread = true;
	set.bDataThread = false;
	set.bJobScheduler = false;

	SampleBrowser sampleBrowser("SampleBrowser", set, "SampleMaterial");

	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
