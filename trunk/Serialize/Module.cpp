#include "Serialize/stdAfx.h"
#include "Serialize/Module.h"

#include "Serialize/Serialize/SerializeListener.hxx"
#include "Serialize/Serialize/BinaryInputArchive.hxx"
#include "Serialize/Serialize/BinaryOutputArchive.hxx"

#include "Serialize/ResourceBuilder.hxx"
#include "Serialize/MeshData.hxx"
#include "Serialize/SkeletonData.hxx"
#include "Serialize/AnimationData.hxx"


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