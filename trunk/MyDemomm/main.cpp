#include "GameApp.h"

//#include "CGFLoader.h"
//#include "ReadOnlyChunkFile.h"

// CContentCGF* LoadChunkFileContent( const char *filename,bool bNoWarningMode )
// {
// 	CLoaderCGF cgf;
// 	CReadOnlyChunkFile *pChunkFile = new CReadOnlyChunkFile(true,bNoWarningMode);
// 	CContentCGF *pCGF = cgf.LoadCGF(filename,*pChunkFile,NULL);
// 	if (pCGF)
// 	{
// 		pCGF->SetChunkFile(pChunkFile);
// 	}
// 	return pCGF;
// }

// #include "Common/Module.h"
// #include "Framework/Module.h"
// #include "DXRender/Module.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	
	// test load
	//LoadChunkFileContent("D:/Crytek/CryENGINE3/Game/Objects/Objects/characters/neutral_male/skeleton_character.chr",true);	
	//

 	if ( !FAILED( g_GameApp.Create(hInstance) ) )
 	{
 		g_GameApp.Run();
 	}

	return 1 ;
}