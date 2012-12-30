#include "Serialize/stdAfx.h"
#include "Serialize/FbxLoad/FbxModule.h"


#include <FBX/fbxsdk.h>

void FBXModuleInit()
{
	// Initialize the sdk manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the io settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using our sdk manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
}

void FBXModuleShutdown()
{

}