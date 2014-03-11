#ifndef  _FbxImportBase__H__
#define  _FbxImportBase__H__

namespace ma
{
	extern FbxManager*	gpFBXSDKManager;
	extern FbxImporter*	gpFBXImporter;

	FbxScene* GetFbxScene(const char* pFileName)
	{
		if (pFileName == NULL)
			return NULL;

		bool importStatus = gpFBXImporter->Initialize(pFileName , -1 , gpFBXImporter->GetIOSettings());
		if(!importStatus)
			return NULL;

		int lFileMajor, lFileMinor, lFileRevision;
		gpFBXImporter->GetFileVersion(lFileMajor , lFileMinor, lFileRevision);

		// Create the entity that hold the whole Scene
		FbxScene* pFbxScene = FbxScene::Create(gpFBXImporter , "");

		importStatus = gpFBXImporter->Import(pFbxScene);
		if (!importStatus)
			return false;

//  		FbxRootNodeUtility::RemoveAllFbxRoots(pFbxScene);
// 		FbxAxisSystem maAxis(FbxAxisSystem::eDirectX);
// 		maAxis.ConvertScene(pFbxScene);


		return pFbxScene;
	}

	FbxMesh* GetFbxMesh(FbxNode* pNode)
	{
		if ( pNode->GetMesh() )
		{
			return pNode->GetMesh();
		}

		int nChildNode = pNode->GetChildCount();
		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			FbxMesh* pFbxMesh = GetFbxMesh(pChildNode);
			FbxNode* pNode = pFbxMesh->GetNode();
			if (pFbxMesh)
			{
				return pFbxMesh;
			}
		}

		return NULL;
	}
}


#endif