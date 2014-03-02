#include "Plugin/FbxImport/stdAfx.h"
#include "Module.h"

#include "FBXToStaticMesh.hxx"
#include "FBXToSkeletonMesh.hxx"
#include "FBXToAnimation.hxx"

namespace ma
{
	static	FbxManager*		gpFBXSDKManager = NULL;
	static	FbxImporter*	gpFBXImporter = NULL;

	void FBXImporterModuleInit()
	{
		assert(gpFBXImporter == NULL);
		if(gpFBXSDKManager)
			gpFBXSDKManager->Destroy();

		gpFBXSDKManager = FbxManager::Create();
		assert(gpFBXSDKManager);
		if(gpFBXSDKManager == NULL)
			return;

		FbxIOSettings* ios = FbxIOSettings::Create(gpFBXSDKManager , IOSROOT);
		gpFBXSDKManager->SetIOSettings(ios);

		gpFBXImporter = FbxImporter::Create(gpFBXSDKManager , "");
		assert(gpFBXImporter);
		if (gpFBXImporter == NULL)
			return;
	}

	void FBXImporterModuleShutdown()
	{
		if(gpFBXSDKManager)
			gpFBXSDKManager->Destroy();
	}

}



