#include "Samples/stdafx.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	bool bD3D11 = true;
	bool bRenderThread = false;
	bool bDataThread = false;
	bool bJobScheduler = false;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bD3D11,bRenderThread,bDataThread,bJobScheduler,"SampleShadowMap");
	sampleBrowser.Run();
	sampleBrowser.Shutdown();

	return 1;
}
