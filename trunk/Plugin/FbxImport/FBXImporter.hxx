#include "Plugin/FbxImport/FBXImporter.h"
#include "Plugin/FbxImport/FbxImportUtil.h"

namespace ma
{
	FBXImporter::FBXImporter()
	{
		m_pFBXSDKManager = NULL;
		m_pFBXImporter = NULL;
	}

	FBXImporter::~FBXImporter()
	{

	}

	bool FBXImporter::Initialize()
	{
		// Create the FBX SDK Manager, destroy the old manager at first
		if(m_pFBXSDKManager)
		{
			m_pFBXSDKManager->Destroy();
		}
		m_pFBXSDKManager = FbxManager::Create();
		if(m_pFBXSDKManager == NULL)
		{
			return false;
		}

		// Create an IOSettings object
		FbxIOSettings* ios = FbxIOSettings::Create(m_pFBXSDKManager , IOSROOT);
		m_pFBXSDKManager->SetIOSettings(ios);

		// Load plug-ins from the executable directory
		//KString lExtension = "dll";
		//KString lPath = FbxGetApplicationDirectory();
		//mpFBXSDKManager->LoadPluginsDirectory(lPath.Buffer() , lExtension.Buffer());

		m_pFBXImporter = FbxImporter::Create(m_pFBXSDKManager , "");
		assert(m_pFBXImporter);
		if (m_pFBXImporter == NULL)
			return false;



		return true;
	}

	struct TrackData
	{
		Vector3TrackData posTrack;
		QuaternionTrackData RotTrack;
		Vector3TrackData scaleTrack;
	};

	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode,FbxAnimStack* pAnimStack,std::map<std::string,TrackData>& boneTrack)
	{
		int lModelCount;
		FbxString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		const char* pszBoneName = pNode->GetName();

		TrackData trackData;
	
		if ( pNode->GetSkeleton() )
		{
			FbxTimeSpan timeSpan = pAnimStack->GetLocalTimeSpan();
			FbxTime start = timeSpan.GetStart();
			FbxTime stop = timeSpan.GetStop();
			FbxTime duration = timeSpan.GetDuration();
			FbxLongLong frame = duration.GetFrameCount();
			FbxTime step;
			step.SetTime(0,0,0,1);
			int i = 0;
			while(start < stop)
			{
				start += step;

				FbxAMatrix mat = pNode->EvaluateLocalTransform(start);
				FbxVector4 pTranslation = mat.GetT();
				FbxQuaternion pRotation = mat.GetQ();
				FbxVector4 pScaling = mat.GetS();

				FbxVector4 euler = mat.GetR();

				trackData.posTrack.m_arrFrame.push_back(i);
				trackData.RotTrack.m_arrFrame.push_back(i);
				trackData.scaleTrack.m_arrFrame.push_back(i);

				trackData.posTrack.m_arrKey.push_back( ToMaUnit(pTranslation) );
				trackData.RotTrack.m_arrKey.push_back( ToMaUnit(pRotation) );
				trackData.scaleTrack.m_arrKey.push_back( ToMaUnit(pScaling) );

				++i;
			}

			boneTrack[pszBoneName] = trackData;	
		}

		for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount),pAnimStack,boneTrack);
		}
	}

	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode,AnimationData* pAnimData,const SkeletonData* pSkelData)
	{
		int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
		FbxString lOutputString;

		lOutputString = "Animation stack contains ";
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer(s)\n";
		FBXSDK_printf(lOutputString);

		std::map<std::string,TrackData> boneTrack;

		//assert(nbAnimLayers == 1);
		for (int l = 0; l < nbAnimLayers; l++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);

		
			DisplayAnimation(lAnimLayer, pNode, pAnimStack, boneTrack);

			break;
		}
		
		assert(boneTrack.size() == pSkelData->m_nBoneNum);
		UINT nBoneNum = boneTrack.size();
		pAnimData->m_nBoneNum = nBoneNum;
		pAnimData->m_arrTransfTrackName.resize(nBoneNum);
		pAnimData->m_arrPosTrack.resize(nBoneNum);
		pAnimData->m_arrRotTrack.resize(nBoneNum);
		pAnimData->m_arrScaleTrack.resize(nBoneNum);

		for (UINT i = 0; i < nBoneNum; ++i)
		{
			std::string boneName = pSkelData->m_arrBoneName[i];
			std::map<std::string,TrackData>::iterator it = boneTrack.find(boneName);
			assert(it != boneTrack.end());
			if (it == boneTrack.end())
				continue;

			//TrackData& trackData = it->second;
			pAnimData->m_arrTransfTrackName[i] = boneName;
			pAnimData->m_arrPosTrack[i] = it->second.posTrack;
			pAnimData->m_arrRotTrack[i] = it->second.RotTrack;
			pAnimData->m_arrScaleTrack[i] = it->second.scaleTrack;
			int nFrame = it->second.posTrack.m_arrFrame.size();
			pAnimData->m_nFrameNum = pAnimData->m_nFrameNum < nFrame ? nFrame : pAnimData->m_nFrameNum;

		}
	}

	bool	FBXImporter::LoadStaticMeshData(const char* pFileName,MeshData* pMeshData)
	{
		FbxScene* pFbxScene = GetFbxScene(pFileName);
		if (pFbxScene == NULL)
			return false;

		FbxMesh* pFbxMesh = GetFbxMesh( pFbxScene->GetRootNode() );

		GetMeshData(pFbxMesh,pMeshData,NULL);

		//m_pFBXImporter->Destroy();

		return true;
	}

	bool	FBXImporter::LoadSkeletonMeshData(const char* pFileName,MeshData* pMeshData,SkeletonData* pSkeData)
	{
		FbxScene* pFbxScene = GetFbxScene(pFileName);
		if (pFbxScene == NULL)
			return false;

		// get bind pose
		KFbxPose* pBindPose = pFbxScene->GetPose(0);
		assert(pBindPose && pBindPose->IsBindPose());
		if (pBindPose == NULL)
			return false;

		FbxSkeleton* pRootBone =  GetFbxRootBone(pFbxScene->GetRootNode());

		GetSkeletonData(pRootBone,pBindPose,pSkeData);

		FbxMesh* pFbxMesh = GetFbxMesh( pFbxScene->GetRootNode() );

		GetMeshData(pFbxMesh,pMeshData,pSkeData);

		//m_pFBXImporter->Destroy();

		return true;
	}

	bool	FBXImporter::LoadAnimationData(const char* pFileName,AnimationData* pAnimation, const SkeletonData* pSkelData)
	{
		if (pFileName == NULL)
			return false;

		FbxScene* pFbxScene = GetFbxScene(pFileName);
		if (pFbxScene == NULL)
			return false;

		int nAnimStackCount = pFbxScene->GetSrcObjectCount<FbxAnimStack>();
		assert(nAnimStackCount == 1);
		for (int i = 0; i < nAnimStackCount; i++)
		{
			FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(lAnimStack, pFbxScene->GetRootNode(),pAnimation,pSkelData);

			break;
		}

		//m_pFBXImporter->Destroy();

		return true;
	}

	FbxScene*	FBXImporter::GetFbxScene(const char* pFileName)
	{
		if (pFileName == NULL)
			return NULL;
			
		bool importStatus = m_pFBXImporter->Initialize(pFileName , -1 , m_pFBXImporter->GetIOSettings());
		//lImporter->GetFileVersion(fileVersion.mMajor , fileVersion.mMinor , fileVersion.mRevision);
		if(!importStatus)
			return NULL;

		// Create the entity that hold the whole Scene
		FbxScene* pFbxScene = FbxScene::Create(m_pFBXSDKManager , "");

		importStatus = m_pFBXImporter->Import(pFbxScene);
		if (!importStatus)
			return false;

		//FbxAxisSystem fbxAxis = lScene->GetGlobalSettings().GetAxisSystem();
		
		return pFbxScene;
	}

	FbxMesh* FBXImporter::GetFbxMesh(FbxNode* pNode)
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
			if (pFbxMesh)
			{
				return pFbxMesh;
			}
		}

		return NULL;
	}

	FbxSkeleton* FBXImporter::GetFbxRootBone(FbxNode* pNode)
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


	void  FBXImporter::GetSkeletonData(FbxSkeleton* pSkeleton,KFbxPose* pBindPose,SkeletonData* pSkelData)
	{
		if (pSkeleton == NULL)
			return;

		if (pBindPose == NULL)
			return;

		FbxNode* pNode = pSkeleton->GetNode();
		FbxString  name = pNode->GetName();
		FbxNode* pParentNode = pNode->GetParent();

		BoneIndex parentID = InvalidID<BoneIndex>();
		const char* pPrentName = pParentNode->GetName();
		std::vector<std::string>::iterator it = 
			std::find((pSkelData->m_arrBoneName).begin(),(pSkelData->m_arrBoneName).end(),pPrentName);
		if (it != (pSkelData->m_arrBoneName).end())
		{
			parentID = it - (pSkelData->m_arrBoneName).begin(); 
		}

		KFbxVector4 LocalLinkT;
		FbxQuaternion LocalLinkQ;
		KFbxVector4 LocalLinkS;
		
		int PoseLinkIndex = pBindPose->Find(pNode);
		assert(PoseLinkIndex >= 0);
		KFbxMatrix NoneAffineMatrix = pBindPose->GetMatrix(PoseLinkIndex);
		KFbxXMatrix Matrix = *(KFbxXMatrix*)(double*)&NoneAffineMatrix;
		
		LocalLinkT = Matrix.GetT();
		LocalLinkQ = Matrix.GetQ();
		LocalLinkS = Matrix.GetS();

		int index = (pSkelData->m_arrBoneName).size();

		(pSkelData->m_arrBoneName).push_back(name.Buffer());
		(pSkelData->m_arrScaleOS).push_back( ToMaUnit(LocalLinkS) );
		(pSkelData->m_arrRotOS).push_back( ToMaUnit(LocalLinkQ) );
		(pSkelData->m_arrPosOS).push_back( ToMaUnit(LocalLinkT) );
		(pSkelData->m_arrParentIndice).push_back(parentID);
		pSkelData->m_nBoneNum = index + 1;

		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			FbxSkeleton* pSkeletonx = pChildNode->GetSkeleton();
			assert(pSkeletonx);
			GetSkeletonData(pSkeletonx,pBindPose,pSkelData);
		}
	}


	struct pointSkin
	{
		std::vector<std::string> m_vBoneName;
		std::vector<BoneIndex>	m_vBoneInd;
		std::vector<double> m_vBoneWeight;
	};

	void GetSkinInfo(const FbxMesh* pMesh,const SkeletonData* pSkelData,
		std::vector<pointSkin>& vPointSkin, int& clusterCount)
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

				BoneIndex boneIndex = InvalidID<BoneIndex>();
				for (UINT index = 0; index < pSkelData->m_arrBoneName.size(); ++index)
				{
					const std::string& strBoneName = pSkelData->m_arrBoneName[index];
					if ( strcmp(strBoneName.c_str(),pszName) == 0 )
					{
						boneIndex = index;
						break;
					}
				}
				assert( IsValidID(boneIndex) );

				//pSkelData->m_arrBoneName

				int associatedCtrlPointCount = pCluster->GetControlPointIndicesCount();  
				int* pCtrlPointIndices = pCluster->GetControlPointIndices();  
				double* pCtrlPointWeights = pCluster->GetControlPointWeights();

				for (int k = 0; k < associatedCtrlPointCount; ++k)
				{
					int nIndex = pCtrlPointIndices[k];
					double fWeight = pCtrlPointWeights[k];
					vPointSkin[nIndex].m_vBoneName.push_back(pszName);
					vPointSkin[nIndex].m_vBoneInd.push_back(boneIndex);
					vPointSkin[nIndex].m_vBoneWeight.push_back(fWeight);
				}	
			}
		}

		FBXSDK_printf("End Get Skin Info .....\n");
		
	}

	void FBXImporter::GetMeshData(FbxMesh* pMesh,MeshData* pMeshData,const SkeletonData* pSkelData)
	{
		if (pMesh == NULL)
			return;

		if(!pMesh->IsTriangleMesh())
		{
			FbxGeometryConverter converter(m_pFBXSDKManager);
			pMesh = converter.TriangulateMesh(pMesh);
		}

		std::vector<pointSkin> vSkin;	
		int clusterCount = 0;
		if (pSkelData)
		{
			GetSkinInfo(pMesh,pSkelData,vSkin,clusterCount);
		}

		FBXSDK_printf("Begin Get Vertex Info .....\n");

		std::vector<VertexType0> vertexList;
		std::vector<xmUint16> indexList;

		int triangleCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;

		int* pTriangleMtlIndex = new int[triangleCount];
		ConnectMaterialToMesh(pMesh,triangleCount,pTriangleMtlIndex);
		for(int i = 0 ; i < triangleCount ; ++i)
		{
			assert(pMesh->GetPolygonSize(i) == 3); 
			for(int j = 0; j < 3 ; ++j)
			{
				int ctrlPointIndex = pMesh->GetPolygonVertex(i , j);

				D3DXVECTOR3 vertex;
				D3DXVECTOR4 color;
				D3DXVECTOR3 normal;
				D3DXVECTOR3 tangent;
				D3DXVECTOR2 uv[2];

				// Read the vertex
				ReadVertex(pMesh , ctrlPointIndex , &vertex);

				// Read the color of each vertex
				ReadColor(pMesh , ctrlPointIndex , vertexCounter , &color);

				for(int k = 0 ; k < 2 ; ++k)
				{
					ReadUV(pMesh , ctrlPointIndex , pMesh->GetTextureUVIndex(i, j) , k , &(uv[k]));
				}

				// Read the normal of each vertex
				ReadNormal(pMesh , ctrlPointIndex , vertexCounter , &normal);

				// Read the tangent of each vertex
				ReadTangent(pMesh , ctrlPointIndex , vertexCounter , &tangent);

				vertexCounter++;

				//D3DXVec3TransformCoord(&vertex[j],&vertex[j],&matrix);
				VertexType0 vertert;
				memset(&vertert,0,sizeof(VertexType0));
				vertert.p = vertex;
				vertert.uv = uv[0];

				if (pSkelData)
				{
					xmUint8 boneInd[4]  = {0};
					xmUint8 weight[4] = {0};
					for (UINT k = 0; k < 4; ++k)
					{
						if (k < vSkin[ctrlPointIndex].m_vBoneInd.size())
						{
							boneInd[k] = vSkin[ctrlPointIndex].m_vBoneInd[k];
							weight[k] = vSkin[ctrlPointIndex].m_vBoneWeight[k] * 255;
						}
					}
					memcpy(&vertert.b,&boneInd[0],sizeof(xmUint32));
					memcpy(&vertert.w,&weight[0],sizeof(xmUint32));
				}

				UINT index = 0;
				std::vector<VertexType0>::iterator it = std::find(vertexList.begin(),vertexList.end(),vertert);
				if (it != vertexList.end())
				{
					index = it - vertexList.begin();
				}
				else
				{
					index = vertexList.size();
					vertexList.push_back(vertert);
				}
				indexList.push_back(index);
			}
		}

		FBXSDK_printf("End Get Vertex Info .....\n");

		// Ib
		UINT nIndexCount = indexList.size();
		pMeshData->m_nIndexType = INDEX_TYPE_U16;
		pMeshData->m_arrIndexBuffer.resize(nIndexCount * sizeof(xmUint16));
		xmUint16* pIb = (xmUint16*)&pMeshData->m_arrIndexBuffer[0];
		for (UINT i = 0; i < nIndexCount; ++i)
		{
			pIb[i] = indexList[i];
		}

		// vb
		UINT nVertexCount = vertexList.size();
		pMeshData->m_nVertexType = VT_SKIN_VERTEX_0;
		pMeshData->m_arrVertexBuffer.resize(nVertexCount * sizeof(VertexType0));
		VertexType0* pVb = (VertexType0*)&pMeshData->m_arrVertexBuffer[0]; 
		for (UINT i = 0; i < vertexList.size(); ++i)
		{
			pVb[i] = vertexList[i];
		}

		// subMesh
		MeshLODData* pMeshLodData = new MeshLODData;
		SubMeshData* pSubMeshData = new SubMeshData;
		pMeshLodData->m_arrSubMesh.push_back(pSubMeshData);
		pSubMeshData->m_nIndexStart = 0;
		pSubMeshData->m_nIndexCount = nIndexCount;
		pSubMeshData->m_nVertexStart = 0;
		pSubMeshData->m_nVertexCount = nVertexCount;
		if (pSkelData)
		{
			pSubMeshData->m_arrBonePalette.resize(clusterCount);
			for (int i = 0; i <clusterCount; ++i)
			{
				pSubMeshData->m_arrBonePalette[i] = i;
			}
		}
		pMeshData->m_arrMeshLOD.push_back(pMeshLodData);

		LoadMaterial(pMesh);

		// Bound Box
		pMesh->ComputeBBox();
		D3DXVECTOR3 vBoxMin = ToMaUnit( (FbxDouble3)pMesh->BBoxMin );
		D3DXVECTOR3 vBoxMax = ToMaUnit( (FbxDouble3)pMesh->BBoxMax );

		pMeshData->SetBoundingAABB(&vBoxMin,&vBoxMax);
		
	}



	void FBXImporter::ConnectMaterialToMesh(FbxMesh* pMesh , int triangleCount , int* pTriangleMtlIndex)
	{
		// Get the material index list of current mesh
		FbxLayerElementArrayTemplate<int>* pMaterialIndices;
		FbxGeometryElement::EMappingMode   materialMappingMode = FbxGeometryElement::eNone;

		if(pMesh->GetElementMaterial())
		{
			pMaterialIndices    = &pMesh->GetElementMaterial()->GetIndexArray();
			materialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
			if(pMaterialIndices)
			{
				switch(materialMappingMode)
				{
				case FbxGeometryElement::eByPolygon:
					{
						if(pMaterialIndices->GetCount() == triangleCount)
						{
							for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
							{
								int materialIndex = pMaterialIndices->GetAt(triangleIndex);

								pTriangleMtlIndex[triangleIndex] = materialIndex;
							}
						}
					}
					break;

				case FbxGeometryElement::eAllSame:
					{
						int lMaterialIndex = pMaterialIndices->GetAt(0);

						for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
						{
							int materialIndex = pMaterialIndices->GetAt(triangleIndex);

							pTriangleMtlIndex[triangleIndex] = materialIndex;
						}
					}
				}
			}
		}
	}

	void FBXImporter::ReadVertex(FbxMesh* pMesh , int ctrlPointIndex , D3DXVECTOR3* pVertex)
	{
		FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

		*pVertex = ToMaUnit(pCtrlPoint[ctrlPointIndex]);
	}

	void FBXImporter::ReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR4* pColor)
	{
		if(pMesh->GetElementVertexColorCount() < 1)
		{
			return;
		}

		FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);
		switch(pVertexColor->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			{
				switch(pVertexColor->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						FbxColor fbxColor = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex);
						*pColor =  ToMaUnit(fbxColor);
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
						FbxColor fbxColor =  pVertexColor->GetDirectArray().GetAt(id);
						*pColor = ToMaUnit(fbxColor);
					}
					break;

				default:
					break;
				}
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pVertexColor->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						FbxColor fbxColor = pVertexColor->GetDirectArray().GetAt(vertexCounter);
						*pColor = ToMaUnit(fbxColor);
					}
					break;
				case FbxGeometryElement::eIndexToDirect:
					{
						int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
						FbxColor fbxColor =  pVertexColor->GetDirectArray().GetAt(id);
						*pColor = ToMaUnit(fbxColor);
					}
					break;
				default:
					break;
				}
			}
			break;
		}
	}

	void FBXImporter::ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , D3DXVECTOR2* pUV)
	{
// 		for (int layerIndex = 0; layerIndex < pMesh->GetLayerCount(); ++layerIndex)
// 		{
// 			FbxLayer* pLayer = pMesh->GetLayer(layerIndex);
// 			for (int uvSetIndex = 0; uvSetIndex < pLayer->GetUVSetCount();++uvSetIndex)
// 			{
// 				FbxLayerElementUV* pElementUV = pLayer->GetUVSets()[uvSetIndex];
// 				if (pElementUV == NULL)
// 					continue;
// 
// 				const char* pszUVSetName = pElementUV->GetName();
// 
// 			}
// 		}

		if(uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
		{
			return ;
		}

		int uvCount = pMesh->GetTextureUVCount();
		int uvLayerCount = pMesh->GetUVLayerCount();

		FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

		int uvIndex = -1;
		switch(pVertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			{
				const bool useIndex = pVertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
				uvIndex = useIndex ? pVertexUV->GetIndexArray().GetAt(ctrlPointIndex) : ctrlPointIndex;
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			{
				uvIndex = /*useIndex ? pVertexUV->GetIndexArray().GetAt(textureUVIndex) :*/ textureUVIndex;
			}
			break;
		}

		if (uvIndex != -1)
		{
			FbxVector2 uvValue = pVertexUV->GetDirectArray().GetAt(uvIndex);
			pUV->x = (float)uvValue[0];
			pUV->y = 1.0f - (float)uvValue[1];
		}
	}

	void FBXImporter::ReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR3* pNormal)
	{
		if(pMesh->GetElementNormalCount() < 1)
		{
			return;
		}

		FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
		switch(leNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			{
				switch(leNormal->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						FbxVector4 vFbx = leNormal->GetDirectArray().GetAt(ctrlPointIndex);
						*pNormal = ToMaUnit(vFbx);
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
						*pNormal = ToMaUnit( leNormal->GetDirectArray().GetAt(id) );
					}
					break;

				default:
					break;
				}
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			{
				switch(leNormal->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						*pNormal = ToMaUnit( leNormal->GetDirectArray().GetAt(vertexCounter) );
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexCounter);
						*pNormal = ToMaUnit(  leNormal->GetDirectArray().GetAt(id) );
					}
					break;

				default:
					break;
				}
			}
			break;
		}
	}

	void FBXImporter::ReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , D3DXVECTOR3* pTangent)
	{
		if(pMesh->GetElementTangentCount() < 1)
		{
			return;
		}

		FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

		switch(leTangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			{
				switch(leTangent->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						*pTangent = ToMaUnit( leTangent->GetDirectArray().GetAt(ctrlPointIndex) ); 
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
						*pTangent = ToMaUnit( leTangent->GetDirectArray().GetAt(id) );
					}
					break;

				default:
					break;
				}
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			{
				switch(leTangent->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						*pTangent = ToMaUnit( leTangent->GetDirectArray().GetAt(vertecCounter) );
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertecCounter);
						*pTangent = ToMaUnit( leTangent->GetDirectArray().GetAt(id) );
					}
					break;

				default:
					break;
				}
			}
			break;
		}
	}

// 	void FBXImporter::ConnectMaterialToMesh(KFbxMesh* pMesh , int triangleCount , int* pTriangleMtlIndex)
// 	{
// 		// Get the material index list of current mesh
// 		FbxLayerElementArrayTemplate<int>* pMaterialIndices;
// 		FbxGeometryElement::EMappingMode   materialMappingMode = FbxGeometryElement::eNone;
// 
// 		if(pMesh->GetElementMaterial())
// 		{
// 			pMaterialIndices    = &pMesh->GetElementMaterial()->GetIndexArray();
// 			materialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
// 			if(pMaterialIndices)
// 			{
// 				switch(materialMappingMode)
// 				{
// 				case FbxGeometryElement::eByPolygon:
// 					{
// 						if(pMaterialIndices->GetCount() == triangleCount)
// 						{
// 							for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
// 							{
// 								int materialIndex = pMaterialIndices->GetAt(triangleIndex);
// 
// 								pTriangleMtlIndex[triangleIndex] = materialIndex;
// 							}
// 						}
// 					}
// 					break;
// 
// 				case FbxGeometryElement::eAllSame:
// 					{
// 						int lMaterialIndex = pMaterialIndices->GetAt(0);
// 
// 						for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
// 						{
// 							int materialIndex = pMaterialIndices->GetAt(triangleIndex);
// 
// 							pTriangleMtlIndex[triangleIndex] = materialIndex;
// 						}
// 					}
// 				}
// 			}
// 		}
// 

	void FBXImporter::LoadMaterial(FbxMesh* pMesh)
	{
		if (pMesh == NULL)
			return;
		
		FbxNode* pNode = pMesh->GetNode();
		if (pNode == NULL)
			return;
		
		int materialCount = pNode->GetMaterialCount();
		for(int materialIndex = 0 ; materialIndex < materialCount ; materialIndex++)
		{
			FbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial(materialIndex);
			if (pSurfaceMaterial == NULL)
				continue;

			LoadMaterialAttribute(pSurfaceMaterial);

			LoadMaterialTexture(pSurfaceMaterial);
		}
	}

	void FBXImporter::LoadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial)
	{
		// Get the name of material
		pSurfaceMaterial->GetName();

		FbxSurfacePhong* pSurfacePhong = FbxCast<FbxSurfacePhong>(pSurfaceMaterial);
		// Phong material
		if(pSurfacePhong)
		{
			// Ambient Color
			fbxDouble3 ambient = pSurfacePhong->Ambient;
			// ...

			// Diffuse Color
			fbxDouble3 diffuse = pSurfacePhong->Diffuse;
			// ...

			// Specular Color
			fbxDouble3 specular = pSurfacePhong->Specular;
			// ...

			// Emissive Color
			fbxDouble3 emissive = pSurfacePhong->Emissive;
			// ...

			// Opacity
			FbxDouble opacity = pSurfacePhong->TransparencyFactor;
			// ...

			// Shininess
			FbxDouble shininess = pSurfacePhong->Shininess;
			// ...

			// Reflectivity
			FbxDouble reflect = pSurfacePhong->ReflectionFactor;
			// ...
			return;
		}

		FbxSurfaceLambert* pSurfaceLambert = FbxCast<FbxSurfaceLambert>(pSurfaceMaterial);
		// Lambert material
		if(pSurfaceLambert)
		{

			// Ambient Color
			fbxDouble3 ambient = pSurfaceLambert->Ambient;
			// ...

			// Diffuse Color
			fbxDouble3 diffuse = pSurfaceLambert->Diffuse;
			// ...

			// Emissive Color
			fbxDouble3 emissive = pSurfaceLambert->Emissive;
			// ...

			// Opacity
			FbxDouble opacity = pSurfaceLambert->TransparencyFactor;
			// ...
			return;
		}
	}

	void FBXImporter::LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial)
	{
		if (pSurfaceMaterial == NULL)
			return;

		int lTextureIndex = 0;
		//FBXSDK_FOR_EACH_TEXTURE(lTextureIndex);
		for(lTextureIndex=0; lTextureIndex < FbxLayerElement::sTypeTextureCount; lTextureIndex++)
		{
			const char* pszTextureChannelName = FbxLayerElement::sTextureChannelNames[lTextureIndex];
			FbxProperty pProperty = pSurfaceMaterial->FindProperty(pszTextureChannelName);
			//FbxProperty pProperty = pSurfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[i]);
			if ( pProperty.IsValid() )
			{
				int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();
				for(int j = 0 ; j < lTextureCount ; ++j)
				{
					FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
					if (lTexture == NULL)
						continue;

					FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(lTexture);
					//FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);
					const char* pFileName = lFileTexture->GetFileName();
					// Use pTexture to load the attribute of current texture..
				}
			}
		}
	}

}



