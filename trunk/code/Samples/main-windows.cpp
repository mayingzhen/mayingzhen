#include "Samples/stdafx.h"

#include "Engine/Application/Platform.h"

#include "SampleBrowser.h"


using namespace ma;


int main(int argc, char* argv[])
{
	SampleBrowser sampleBrowser("SampleBrowser");

	Platform app;

	app.Init();
	app.Run();
	app.Shutdown();

	return 1;
}
