#include "FBXToSkeletonMesh.h"
#include "FbxImportUtil.h"


namespace ma
{

	bool LoadSkeletonMeshData(const char* pFileName,ImportParm* pImportParm,		
		const char* pOutMeshFile, const char* pOutMatFile, 
		const char* pOutSkeFile, const char* pOutSkaFile)
	{
		std::string strDir = GetArchiveMananger()->GetArchiveByIndex(0)->GetName();	
		std::string strMeshFile = strDir + "/" + string(pFileName);

		std::string strOutMeshFile = pOutMeshFile ? pOutMeshFile : StringUtil::replaceFileExt(pFileName,"skn");
		std::string strOutMatFile = pOutMatFile ? pOutMatFile : StringUtil::replaceFileExt(pFileName,"mat");
		std::string strOutSkeFile = pOutSkeFile ? pOutSkeFile : StringUtil::replaceFileExt(pFileName,"ske");
		std::string strOutSkaFile = pOutSkaFile ? pOutSkaFile : StringUtil::replaceFileExt(pFileName,"ska");

		FbxScene* pFbxScene = GetFbxScene(strMeshFile.c_str());
		if (pFbxScene == NULL)
			return false;
	
		// Ske
		int nPoseCount = pFbxScene->GetPoseCount();
		FbxPose* pBindPose = pFbxScene->GetPose(0);
		ASSERT(pBindPose && pBindPose->IsBindPose());
		if (pBindPose == NULL)
			return false;

		Skeleton skeData;

		FbxSkeleton* pRootBone = GetFbxRootBone(pFbxScene->GetRootNode());
		GetSkeletonData(pRootBone,pBindPose,skeData);
		//skeData.m_nBoneNum = skeData.m_arrBoneName.size();

		skeData.InitResPose();

		skeData.SaveToFile(strOutSkeFile.c_str());

		// Mesh
		MeshData meshData; 
		meshData.SetVertexType(DUM_POSITION | DUM_TEXCOORD | DUM_NORMAL | DUM_BLENDWEIGHT | DUM_BLENDINDICES);
		meshData.SetIndexType(INDEX_TYPE_U16);

		FbxMesh* pFbxMesh = GetFbxMesh( pFbxScene->GetRootNode() );

		GetSkinMeshData<V_3P_2UV_3N_3T_S,uint16>(pFbxMesh,&meshData,skeData,pImportParm);

		//RefPtr<CMaterial> pMaterial = pMeshData->GetSubMeshByIndex(0,0)->m_pMaterial;
		//pMaterial->Save(strOutMatFile.c_str());

		meshData.SaveToFile(strOutMeshFile.c_str());

		//SAFE_DELETE(pMeshData);

		// Animation
		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		if (nAnimStackCount > 0)
		{
			LoadAnimationData(pFileName,skeData,strOutSkaFile.c_str());
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


	void GetSkeletonData(FbxSkeleton* pSkeleton,FbxPose* pBindPose,Skeleton& skeData)
	{
 		if (pSkeleton == NULL)
 			return;
 
 		if (pBindPose == NULL)
 			return;
 
		FbxNode* pNode = pSkeleton->GetNode();
		const char*  pName = pNode->GetName();
		FbxNode* pParentNode = pNode->GetParent();

		UINT parentID = skeData.GetBoneIdByName( pParentNode->GetName() ); 
		
		UINT nCount = pBindPose->GetCount();
		int PoseLinkIndex = pBindPose->Find(pNode);
		ASSERT(PoseLinkIndex >= 0);
		FbxMatrix NoneAffineMatrix = pBindPose->GetMatrix(PoseLinkIndex);

		skeData.AddBone( pName, parentID, ToMaUnit(NoneAffineMatrix) );

		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			FbxSkeleton* pSkeletonx = pChildNode->GetSkeleton();
			ASSERT(pSkeletonx);
			GetSkeletonData(pSkeletonx,pBindPose,skeData);
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

	
	template<class VertexType,class IndexType>
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

		std::vector<VertexType> arrVertex;
		std::vector<IndexType> arrIndex;

		std::vector<pointSkin> arrPointSkin;	
		int clusterCount = 0;
		GetSkinInfo(pMesh,skeData,arrPointSkin,clusterCount);

		std::map< int,std::vector<int> >::iterator it = matIndexToTriangleIndex.begin();
		for (; it != matIndexToTriangleIndex.end(); ++it) 
		{
			// One SunMesh
			SubMeshData* pSubmesh = pMeshData->AddSubMeshData();
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

				VertexType vertex[3];

				GetTriangleData(pMesh,nTriangleIndex,vertex,pImportParm);

				GetSkinData(pMesh,nTriangleIndex,vertex,arrPointSkin,pImportParm);
			
				for(int j = 0; j < 3 ; ++j)
				{
					FbxVector4 fPos(vertex[j].m_position.x,vertex[j].m_position.y,vertex[j].m_position.z,1);

					//vertex[j].m_uv.y = 1.0f - vertex[j].m_uv.y;

					vertex[j].m_position = ToMaUnit( globalTransform.MultT(fPos) );	

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

		UpdateHardwareBuffer(arrVertex,arrIndex, pMeshData->GetVertexBuffer().get(), pMeshData->GetIndexBuffer().get());

		// Bound Box
		pMesh->ComputeBBox();
		AABB aabb;
		aabb.setMinimum( ToMaUnit( (FbxDouble3)pMesh->BBoxMin ) );
		aabb.setMaximum( ToMaUnit( (FbxDouble3)pMesh->BBoxMax ) );
		pMeshData->SetBoundingAABB(aabb);
	}

	template<class VertexType>
	void GetSkinData(FbxMesh* pMesh,int nTriangleIndex, VertexType vertex[3],std::vector<pointSkin>& arrPointSkin, ImportParm* pImportParm)
	{
		for(int j = 0; j < 3 ; ++j)
		{
			int vertexCounter = nTriangleIndex * 3  + j;

			int ctrlPointIndex = pMesh->GetPolygonVertex(nTriangleIndex , j);

			SkinDataCovert(vertex[j].m_boneID,vertex[j].m_weight,
				arrPointSkin[ctrlPointIndex].m_vBoneInd,
				arrPointSkin[ctrlPointIndex].m_vBoneWeight);
		}
	}
}



