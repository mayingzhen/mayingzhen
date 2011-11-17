#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	CGameApp gameApp;
	if ( !FAILED( gameApp.Create(hInstance) ) )
	{
		gameApp.Run();
	}

	return 1 ;
}