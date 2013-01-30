#include "Samples/stdafx.h"

using namespace ma;

int main(int argc, char* argv[])
{
	ApplicationWin app("Samples");

	app.Init(argc,argv);

	//AppRegisterScene();
	//app.SetScene(AppDefaultSceneName,argc,argv);
	app.Run();
	app.Shutdown();

	return 0;
}

