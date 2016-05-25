#include "FBXToSkeletonMesh.h"
#include "FbxImportUtil.h"


namespace ma
{

	bool LoadSkeletonMeshData(const char* pFileName,ImportParm* pImportParm,		
		const char* pOutMeshFile, const char* pOutMatFile, 
		const char* pOutSkeFile, const char* pOutSkaFile)
	{
		std::string strDir = GetArchiveMananger()->GetSaveDir();	
		std::string strMeshFile = strDir + "/" + string(pFileName);

		std::string strOutMeshFile = pOutMeshFile ? pOutMeshFile : StaticFunc::ReplaceFileExt(pFileName,"skn");
		std::string strOutSkeFile = pOutSkeFile ? pOutSkeFile : StaticFunc::ReplaceFileExt(pFileName,"ske");
		std::string strOutSkaFile = pOutSkaFile ? pOutSkaFile : StaticFunc::ReplaceFileExt(pFileName,"ska");

		FbxScene* pFbxScene = GetFbxScene(strMeshFile.c_str());
		if (pFbxScene == NULL)
			return false;
	
		// Ske
		int nPoseCount = pFbxScene->GetPoseCount();
		FbxPose* pBindPose = pFbxScene->GetPose(0);
		ASSERT(pBindPose && pBindPose->IsBindPose());
		if (pBindPose == NULL)
			return false;

		RefPtr<Skeleton> pSkeData = CreateSkeleton();

		FbxSkeleton* pRootBone = GetFbxRootBone(pFbxScene->GetRootNode());
		GetSkeletonData(pRootBone->GetNode(), pRootBone,pBindPose,*pSkeData);

		pSkeData->InitResPose();

		pSkeData->SaveToFile(strOutSkeFile.c_str());

		// Mesh
		RefPtr<MeshData> pMeshData = CreateMeshData();
		pMeshData->SetBoneNumber(pSkeData->GetBoneNumer());
		pMeshData->SetVertexType(SKIN_VERTEX_1);
		pMeshData->SetIndexType(INDEX_TYPE_U16);

		FbxMesh* pFbxMesh = GetFbxMesh( pFbxScene->GetRootNode() );

		GetSkinMeshData(pFbxMesh,pMeshData.get(),*pSkeData,pImportParm);

		pMeshData->SaveToFile(strOutMeshFile.c_str());

		// Animation
		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		if (nAnimStackCount > 0)
		{
			LoadAnimationData(pFileName,*pSkeData,strOutSkaFile.c_str());
		}

		return true;
	}
	
	

	FbxSkeleton* GetFbxRootBone(FbxNode* pNode)
	{
		if ( pNode->GetSkeleton() )
		{
			return pNode->GetSkeleton();
		}

		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			FbxSkeleton* pFbxSkeleton = GetFbxRootBone(pChildNode);
			if (pFbxSkeleton)
			{
				return pFbxSkeleton;
			}
		}

		return NULL;
	}


	void GetSkeletonData(FbxNode* pRoot, FbxSkeleton* pSkeleton,FbxPose* pBindPose,Skeleton& skeData)
	{
 		if (pSkeleton == NULL)
 			return;
 
 		if (pBindPose == NULL)
 			return;

		FbxNode* pNode = pSkeleton->GetNode();
		const char*  pName = pNode->GetName();
		FbxNode* pParentNode = pNode->GetParent();

		FbxAMatrix GlobalTransform = pRoot->GetScene()->GetEvaluator()->GetNodeGlobalTransform(pRoot);

		UINT parentID = skeData.GetBoneIdByName( pParentNode->GetName() ); 
		
		UINT nCount = pBindPose->GetCount();
		int PoseLinkIndex = pBindPose->Find(pNode);
		ASSERT(PoseLinkIndex >= 0);
		FbxMatrix NoneAffineMatrix = pBindPose->GetMatrix(PoseLinkIndex);

// 		FbxVector4 fTranslation;
// 		FbxQuaternion fRotation;
// 		FbxVector4 fShearing;
// 		FbxVector4 fScaling;
// 		double fSign;
// 
// 		NoneAffineMatrix.GetElements(fTranslation,fRotation,fShearing,fScaling,fSign);
// 
// 		Transform tsf;
// 		tsf.m_vPos = ToMaUnit( GlobalTransform.MultT(fTranslation) );
// 		tsf.m_qRot = ToMaUnit( GlobalTransform.MultQ(fRotation) );
// 		tsf.m_vScale = ToMaUnit( fScaling );
// 
// 		skeData.AddBone( pName, parentID, tsf );

		skeData.AddBone( pName, parentID, ToMaUnit(NoneAffineMatrix) );

		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			FbxSkeleton* pSkeletonx = pChildNode->GetSkeleton();
			ASSERT(pSkeletonx);
			GetSkeletonData(pRoot,pSkeletonx,pBindPose,skeData);
		}
	}
 
 
	void GetSkinInfo(const FbxMesh* pMesh,const Skeleton& skeData, std::vector<pointSkin>& vPointSkin, int& clusterCount)
	{
		if (pMesh == NULL)
			return;

		FBXSDK_printf("Begin Get Skin Info .....\n");

		vPointSkin.resize(pMesh->GetControlPointsCount());

		int deformerCount  = pMesh->GetDeformerCount(); 
		for (int i =0; i < deformerCount; ++i)
		{
			KFbxDeformer* pFBXDeformer = pMesh->GetDeformer(i); 
			if (pFBXDeformer == NULL)
				continue;

			if(pFBXDeformer->GetDeformerType() != KFbxDeformer::eSkin)  
				continue;  

			FbxSkin* pFBXSkin = (KFbxSkin*)(pFBXDeformer);  
			if(pFBXSkin == NULL)  
				continue;  

			clusterCount = pFBXSkin->GetClusterCount(); 
			for (int j = 0; j< clusterCount; ++j)
			{
				KFbxCluster* pCluster =  pFBXSkin->GetCluster(j);  
				if (pCluster == NULL)
					continue;

				KFbxNode* pLinkNode = pCluster->GetLink();  
				if (pLinkNode == NULL)
					continue;

				const char* pszName = pLinkNode->GetName();

				UINT boneIndex = skeData.GetBoneIdByName(pszName); 
				ASSERT( Math::IsValidID(boneIndex) );

				int associatedCtrlPointCount = pCluster->GetControlPointIndicesCount();  
				int* pCtrlPointIndices = pCluster->GetControlPointIndices();  
				double* pCtrlPointWeights = pCluster->GetControlPointWeights();

				for (int k = 0; k < associatedCtrlPointCount; ++k)
				{
					int nIndex = pCtrlPointIndices[k];
					float fWeight = pCtrlPointWeights[k];
					vPointSkin[nIndex].m_vBoneName.push_back(pszName);
					vPointSkin[nIndex].m_vBoneInd.push_back(boneIndex);
					vPointSkin[nIndex].m_vBoneWeight.push_back(fWeight);
				}	
			}
		}

		FBXSDK_printf("End Get Skin Info .....\n");
	}


	void SkinDataCovert(UINT& skinBoneID,UINT& skinBoneWeight,const std::vector<UINT>& arrBone,const std::vector<float>& arrWeight)
	{
		union UIndex
		{
			UINT uInde;
			BYTE uByte[4];
		};

		UIndex boneInd;
		UIndex weight;
		boneInd.uInde = 0;
		weight.uInde = 0;

		for (UINT i = 0; i < 4; ++i)
		{
			if (i < arrBone.size())
			{
				boneInd.uByte[i] = arrBone[i];
				weight.uByte[i] = arrWeight[i] * 255;
			}
		}

		skinBoneID = boneInd.uInde;
		skinBoneWeight = weight.uInde;
	}

	void GetSkinMeshData(FbxMesh* pMesh,MeshData* pMeshData,const Skeleton& skeData,ImportParm* pImportParm)
	{
		if (pMesh == NULL)
			return;

		int nLodIndex = 0;

		FbxAMatrix globalTransform = pMesh->GetNode()->GetScene()->GetEvaluator()->GetNodeGlobalTransform( pMesh->GetNode());	
// 		if (pImportParm)
// 		{
// 			float fScale = pImportParm->m_fScaleFactor;
// 			globalTransform.SetS(FbxVector4(fScale,fScale,fScale,fScale));
// 		}

		pMesh->RemoveBadPolygons();

		FbxGeometryConverter converter(gpFBXSDKManager);

		INT LayerSmoothingCount = pMesh->GetLayerCount(KFbxLayerElement::eSmoothing);
		for(INT i = 0; i < LayerSmoothingCount; i++)
		{
			converter.ComputePolygonSmoothingFromEdgeSmoothing(pMesh, i);
		}

		if(!pMesh->IsTriangleMesh())
		{
			pMesh = converter.TriangulateMesh(pMesh);
		}

		std::map< int,std::vector<int> > matIndexToTriangleIndex;
		ConnectMaterialToMesh(pMesh,matIndexToTriangleIndex);

		std::vector<SkinVertexV0> arrVertex;
		std::vector<UINT16> arrIndex;

		std::vector<pointSkin> arrPointSkin;	
		int clusterCount = 0;
		GetSkinInfo(pMesh,skeData,arrPointSkin,clusterCount);

		std::map< int,std::vector<int> >::iterator it = matIndexToTriangleIndex.begin();
		for (; it != matIndexToTriangleIndex.end(); ++it) 
		{
			// One SunMesh
			RefPtr<SubMeshData> pSubmesh = CreateSubMeshData();
			pMeshData->AddSubMeshData(pSubmesh.get());
			pSubmesh->m_nMateiralID = it->first;

			pSubmesh->m_nVertexStart = arrVertex.size();
			pSubmesh->m_nIndexStart = arrIndex.size();

			int nVertexSize = arrVertex.size();
			int nIndexSize = arrIndex.size();

			std::vector<int>& arrTriangle = it->second;
			for (int i = 0; i < arrTriangle.size(); ++i)
			{
				FBXSDK_printf("Parase triangle %d/%d .....\n",i,arrTriangle.size());

				int nTriangleIndex = arrTriangle[i];

				assert(pMesh->GetPolygonSize(nTriangleIndex) == 3); 

				SkinVertexV0 vertex[3];

				GetTriangleData(pMesh,nTriangleIndex,vertex,pImportParm);

				GetSkinData(pMesh,nTriangleIndex,vertex,arrPointSkin,pImportParm);
			
				for(int j = 0; j < 3 ; ++j)
				{
					FbxVector4 fPos(vertex[j].pos.x,vertex[j].pos.y,vertex[j].pos.z,1);
					FbxVector4 fNor(vertex[j].nor.x,vertex[j].nor.y,vertex[j].nor.z,0);

					vertex[j].pos = ToMaUnit( globalTransform.MultT(fPos) );	
					vertex[j].nor = ToMaUnit( globalTransform.MultT(fNor) );

					UpdateVertexArray(arrVertex,arrIndex,vertex[j]);

					//pMeshData->m_aabb.merge(vertex[j].m_position);
				}
			}

			pSubmesh->m_nVertexCount = arrVertex.size() - nVertexSize;
			pSubmesh->m_nIndexCount = arrIndex.size() - nIndexSize;

			for (int j = 0; j <clusterCount; ++j)
			{
				pSubmesh->AddBonePalette(j);
			}
		}

		vector<SkinVertexV1> pVertexV1;
		pVertexV1.resize(arrVertex.size());
		pMeshData->UpdateMeshData(&pVertexV1[0],&arrVertex[0],arrVertex.size(),&arrIndex[0]);

		pMeshData->GetVertexBuffer()->SetData((uint8*)&pVertexV1[0], sizeof(SkinVertexV1)* pVertexV1.size(), sizeof(SkinVertexV1));
		pMeshData->GetIndexBuffer()->SetData((uint8*)&arrIndex[0], sizeof(UINT16) * arrIndex.size(), sizeof(UINT16));

		// Bound Box
// 		pMesh->ComputeBBox();
// 		AABB aabb;
// 		aabb.setMinimum( ToMaUnit( (FbxDouble3)pMesh->BBoxMin ) );
// 		aabb.setMaximum( ToMaUnit( (FbxDouble3)pMesh->BBoxMax ) );
// 		pMeshData->SetBoundingAABB(aabb);
	}

	void GetSkinData(FbxMesh* pMesh,int nTriangleIndex, SkinVertexV0 vertex[3],std::vector<pointSkin>& arrPointSkin, ImportParm* pImportParm)
	{
		for(int j = 0; j < 3 ; ++j)
		{
			int vertexCounter = nTriangleIndex * 3  + j;

			int ctrlPointIndex = pMesh->GetPolygonVertex(nTriangleIndex , j);

			SkinDataCovert(vertex[j].bone_index,vertex[j].bone_weight,
				arrPointSkin[ctrlPointIndex].m_vBoneInd,
				arrPointSkin[ctrlPointIndex].m_vBoneWeight);
		}
	}
}



