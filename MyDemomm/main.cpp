#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	if ( !FAILED( g_GameApp.Create(hInstance) ) )
	{
		g_GameApp.Run();
	}

	return 1 ;
}