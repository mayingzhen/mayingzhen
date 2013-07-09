#include "Samples/stdafx.h"

#include "Engine/Application/Platform.h"


using namespace ma;


int main(int argc, char* argv[])
{
	Platform app;
	app.Init();
	app.Run();
	app.Shutdown();
	return 0;
}
