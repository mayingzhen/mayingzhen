#include "Serialize/FBXImporter.h"

namespace ma
{
	FBXImporter::FBXImporter()
	{
		mpFBXSDKManager = NULL;
	}

	FBXImporter::~FBXImporter()
	{

	}

	bool FBXImporter::Initialize()
	{
		// Create the FBX SDK Manager, destroy the old manager at first
		if(mpFBXSDKManager)
		{
			mpFBXSDKManager->Destroy();
		}
		mpFBXSDKManager = FbxManager::Create();

		if(mpFBXSDKManager == NULL)
		{
			return false;
		}

		// Create an IOSettings object
		FbxIOSettings* ios = FbxIOSettings::Create(mpFBXSDKManager , IOSROOT);
		mpFBXSDKManager->SetIOSettings(ios);

		// Load plug-ins from the executable directory
		KString lExtension = "dll";
		KString lPath = FbxGetApplicationDirectory();
		mpFBXSDKManager->LoadPluginsDirectory(lPath.Buffer() , lExtension.Buffer());

		// Create the entity that hold the whole Scene
		//mpFBXSDKScene = FbxScene::Create(mpFBXSDKManager , "");

		return true;
	}

	void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)
	{
		FbxAnimCurve* lAnimCurve = NULL;

		// Display general curves.
		if (!isSwitcher)
		{
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				FBXSDK_printf("        TX\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				FBXSDK_printf("        TY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				FBXSDK_printf("        TZ\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				FBXSDK_printf("        RX\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				FBXSDK_printf("        RY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				FBXSDK_printf("        RZ\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (lAnimCurve)
			{
				FBXSDK_printf("        SX\n");
				DisplayCurve(lAnimCurve);
			}    
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (lAnimCurve)
			{
				FBXSDK_printf("        SY\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (lAnimCurve)
			{
				FBXSDK_printf("        SZ\n");
				DisplayCurve(lAnimCurve);
			}
		}

		// Display curves specific to a light or marker.
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute)
		{
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Red\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Green\n");
				DisplayCurve(lAnimCurve);
			}
			lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Blue\n");
				DisplayCurve(lAnimCurve);
			}

			// Display curves specific to a light.
			FbxLight* light = pNode->GetLight();
			if (light)
			{
				lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Intensity\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Outer Angle\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = light->Fog.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Fog\n");
					DisplayCurve(lAnimCurve);
				}
			}

			// Display curves specific to a camera.
			FbxCamera* camera = pNode->GetCamera();
			if (camera)
			{
				lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Field of View\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Field of View X\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Field of View Y\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
				if (lAnimCurve)
				{
					FBXSDK_printf("        Optical Center X\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
				if(lAnimCurve)
				{
					FBXSDK_printf("        Optical Center Y\n");
					DisplayCurve(lAnimCurve);
				}

				lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
				if(lAnimCurve)
				{
					FBXSDK_printf("        Roll\n");
					DisplayCurve(lAnimCurve);
				}
			}

			// Display curves specific to a geometry.
			if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
				lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
				lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
			{
				FbxGeometry* lGeometry = (FbxGeometry*) lNodeAttribute;

				int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
				for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
				{
					FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

					int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
					for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
					{
						FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
						const char* lChannelName = lChannel->GetName();

						lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
						if(lAnimCurve)
						{
							FBXSDK_printf("        Shape %s\n", lChannelName);
							DisplayCurve(lAnimCurve);
						}
					}
				}
			}
		}

		// Display curves specific to properties
		FbxProperty lProperty = pNode->GetFirstProperty();
		while (lProperty.IsValid())
		{
			if (lProperty.GetFlag(FbxPropertyAttr::eUser))
			{
				FbxString lFbxFCurveNodeName  = lProperty.GetName();
				FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

				if (!lCurveNode){
					lProperty = pNode->GetNextProperty(lProperty);
					continue;
				}

				FbxDataType lDataType = lProperty.GetPropertyDataType();
				if (lDataType.GetType() == eFbxBool || lDataType.GetType() == eFbxDouble || lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
				{
					FbxString lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					};

					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
							DisplayCurve(lAnimCurve);
					}
				}
				else if(lDataType.GetType() == eFbxDouble3 || lDataType.GetType() == eFbxDouble4 || lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
				{
					char* lComponentName1 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
					char* lComponentName2 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
					char* lComponentName3 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE  : (char*)"Z";
					FbxString      lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					}
					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName1);
							DisplayCurve(lAnimCurve);
						}
					}

					for( int c = 0; c < lCurveNode->GetCurveCount(1U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(1U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName2);
							DisplayCurve(lAnimCurve);
						}
					}

					for( int c = 0; c < lCurveNode->GetCurveCount(2U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(2U, c);
						if (lAnimCurve)
						{
							DisplayString("        Component ", lComponentName3);
							DisplayCurve(lAnimCurve);
						}
					}
				}
				else if (lDataType.GetType() == eFbxEnum)
				{
					FbxString lMessage;

					lMessage =  "        Property ";
					lMessage += lProperty.GetName();
					if (lProperty.GetLabel().GetLen() > 0)
					{
						lMessage += " (Label: ";
						lMessage += lProperty.GetLabel();
						lMessage += ")";
					};
					DisplayString(lMessage.Buffer());

					for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
					{
						lAnimCurve = lCurveNode->GetCurve(0U, c);
						if (lAnimCurve)
							DisplayListCurve(lAnimCurve, &lProperty);
					}                
				}
			}

			lProperty = pNode->GetNextProperty(lProperty);
		} // while

	}

	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
	{
		int lModelCount;
		FbxString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
		FBXSDK_printf ("\n");

		for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
		}
	}

	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
	{
		int l;
		int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
		FbxString lOutputString;

		lOutputString = "Animation stack contains ";
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer(s)\n";
		FBXSDK_printf(lOutputString);

		for (l = 0; l < nbAnimLayers; l++)
		{
			FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(lAnimLayer, pNode, isSwitcher);
		}
	}

	bool FBXImporter::LoadScene(const char* pSeneName,MeshData* pMeshData, SkeletonData* pSkeData)
	{
		if(mpFBXSDKManager == NULL)
		{
			return false;
		}

		// Get the file version number generate by the FBX SDK.
		//FbxSdkManager::GetFileFormatVersion(mSDKVersion.mMajor , mSDKVersion.mMinor , mSDKVersion.mRevision);

		// Create an importer.
		FbxImporter* pFBXImporter = FbxImporter::Create(mpFBXSDKManager , "");

		// Initialize the importer by providing a filename
		//FBXFileVersion fileVersion;

		bool importStatus = pFBXImporter->Initialize(pSeneName , -1 , mpFBXSDKManager->GetIOSettings());
		//lImporter->GetFileVersion(fileVersion.mMajor , fileVersion.mMinor , fileVersion.mRevision);

		if(!importStatus)
		{
			return NULL;
		}

		// Import the scene
		//mpFBXScene->Clear();
		FbxScene* lScene = FbxScene::Create(mpFBXSDKManager,"myScene");

		importStatus = pFBXImporter->Import(lScene);

		FbxAxisSystem fbxAxis = lScene->GetGlobalSettings().GetAxisSystem();

		ProcessSkeleton(lScene->GetRootNode(),pSkeData);

		//ProcessAnimation(lScene->GetRootNode(),m_vAnimData);

		ProcessMesh(pMeshData,lScene->GetRootNode());

		for (int i = 0; i < lScene->GetSrcObjectCount<FbxAnimStack>(); i++)
		{
			FbxAnimStack* lAnimStack = lScene->GetSrcObject<FbxAnimStack>(i);

			FbxString lOutputString = "Animation Stack Name: ";
			lOutputString += lAnimStack->GetName();
			lOutputString += "\n\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
			//DisplayAnimation(lAnimStack, pScene->GetRootNode());
		}

		//ProcessNode( pMeshData, pSkeData, lScene->GetRootNode() );

		// Destroy the importer.
		pFBXImporter->Destroy();

		return true;
	}

	void FBXImporter::ProcessAnimation(FbxNode* pNode,std::vector<AnimationData*> m_vAnimData)
	{
		if (pNode == NULL)
			return;


	}

// 	void FBXImporter::ProcessNode(MeshData* pMeshData, SkeletonData* pSkeData,FbxNode* pNode)
// 	{
// 		if (pMeshData == NULL || pNode == NULL)
// 			return;
// 
// 		if(pNode->GetNodeAttribute())
// 		{
// 			switch(pNode->GetNodeAttribute()->GetAttributeType())
// 			{
// 			case FbxNodeAttribute::eMesh:
// 				ProcessMesh(pMeshData,pNode);
// 				break;
// 			case FbxNodeAttribute::eSkeleton:
// 				ProcessSkeleton(pNode,pSkeData);
// 				break;
// 			case FbxNodeAttribute::eLight:
// 				//ProcessLight(pNode);
// 				break;
// 			case FbxNodeAttribute::eCamera:
// 				//ProcessCamera();
// 				break;
// 			}
// 		}
// 
// 		for(int i = 0 ; i < pNode->GetChildCount() ; ++i)
// 		{
// 			ProcessNode(pMeshData,pSkeData,pNode->GetChild(i));
// 		}
// 	}

	void  FBXImporter::GetSkeletonData(FbxSkeleton* pSkeleton,SkeletonData* pSkelData)
	{
		if (pSkeleton == NULL)
			return;

		FbxNode* pNode = pSkeleton->GetNode();

		BoneIndex parentID = InvalidID<BoneIndex>();
		FbxNode* pParentNode = pNode->GetParent();
		const char* pPrentName = pParentNode->GetName();
		std::vector<std::string>::iterator it = 
			std::find((pSkelData->m_arrBoneName).begin(),(pSkelData->m_arrBoneName).end(),pPrentName);
		if (it != (pSkelData->m_arrBoneName).end())
		{
			parentID = it - (pSkelData->m_arrBoneName).begin(); 
		}

		FbxString  name = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();
		D3DXVECTOR3 dxtranslation(translation[0],translation[1],translation[2]);
		xmEulerAngleXYZ dxrotation(rotation[0],rotation[1],rotation[2]);
		D3DXVECTOR3 dxscaling(scaling[0],scaling[1],scaling[2]);
		D3DXQUATERNION qtot;
		maQuaternionFromEulerAngleXYZ(&qtot,&dxrotation);

		int index = (pSkelData->m_arrBoneName).size();

		(pSkelData->m_arrBoneName).push_back(name.Buffer());
		(pSkelData->m_arrScaleOS).push_back(dxscaling);
		(pSkelData->m_arrRotOS).push_back(qtot);
		(pSkelData->m_arrPosOS).push_back(dxtranslation);
		(pSkelData->m_arrParentIndice).push_back(parentID);
		pSkelData->m_nBoneNum = index + 1;
	}

	void  FBXImporter::ProcessSkeleton(FbxNode* pNode,SkeletonData* pSkelData)
	{
		if (pNode == NULL)
			return;

		FbxSkeleton* pSkeleton = pNode->GetSkeleton();
		if (pSkeleton)
		{
			GetSkeletonData(pSkeleton,pSkelData);
		}

		int nChildNode = pNode->GetChildCount();
		for (UINT i = 0; i < pNode->GetChildCount(); ++i)
		{
			FbxNode* pChildNode = pNode->GetChild(i);
			ProcessSkeleton(pChildNode, pSkelData);
		}
	}


	struct pointSkin
	{
		std::vector<std::string> m_vBoneName;
		std::vector<BoneIndex>	m_vBoneInd;
		std::vector<double> m_vBoneWeight;
	};

	void FBXImporter::GetMeshData(FbxMesh* pMesh,MeshData* pMeshData)
	{
		if (pMesh == NULL)
			return;

		if(!pMesh->IsTriangleMesh())
		{
			FbxGeometryConverter converter(mpFBXSDKManager);
			pMesh = converter.TriangulateMesh(pMesh);
		}

		int nControlPoint = pMesh->GetControlPointsCount();
		std::vector<pointSkin> m_vSkin;
		m_vSkin.resize(nControlPoint);

		int deformerCount  = pMesh->GetDeformerCount(); 
		int clusterCount; 
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
				int associatedCtrlPointCount = pCluster->GetControlPointIndicesCount();  
				int* pCtrlPointIndices = pCluster->GetControlPointIndices();  
				double* pCtrlPointWeights = pCluster->GetControlPointWeights();

				for (int k = 0; k < associatedCtrlPointCount; ++k)
				{
					int nIndex = pCtrlPointIndices[k];
					double fWeight = pCtrlPointWeights[k];
					m_vSkin[nIndex].m_vBoneName.push_back(pszName);
					m_vSkin[nIndex].m_vBoneInd.push_back(j);
					m_vSkin[nIndex].m_vBoneWeight.push_back(fWeight);
				}	
			}
		}

		std::vector<VertexType0> vertexList;
		std::vector<xmUint16> indexList;

		int triangleCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;

		D3DXVECTOR3 vertex[3];
		D3DXVECTOR4 color[3];
		D3DXVECTOR3 normal[3];
		D3DXVECTOR3 tangent[3];
		D3DXVECTOR2 uv[3][2];

		int* pTriangleMtlIndex = new int[triangleCount];
		ConnectMaterialToMesh(pMesh,triangleCount,pTriangleMtlIndex);

		//D3DXMATRIX                matrix;
		//D3DXMatrixRotationX(&matrix, -(3.14/2));

		for(int i = 0 ; i < triangleCount ; ++i)
		{
			assert(pMesh->GetPolygonSize(i) == 3); 
			for(int j = 0 ; j < 3 ; j++)
			{
				int ctrlPointIndex = pMesh->GetPolygonVertex(i , j);

				// Read the vertex
				ReadVertex(pMesh , ctrlPointIndex , &vertex[j]);

				// Read the color of each vertex
				ReadColor(pMesh , ctrlPointIndex , vertexCounter , &color[j]);

				// Read the UV of each vertex
				// 				FbxStringList lUVSetNameList;
				// 				pMesh->GetUVSetNames(lUVSetNameList);
				// 				for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
				// 				{
				// 					const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
				// 					const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
				// 				}
				for(int k = 0 ; k < 2 ; ++k)
				{
					ReadUV(pMesh , ctrlPointIndex , pMesh->GetTextureUVIndex(i, j) , k , &(uv[j][k]));
				}

				// Read the normal of each vertex
				ReadNormal(pMesh , ctrlPointIndex , vertexCounter , &normal[j]);

				// Read the tangent of each vertex
				ReadTangent(pMesh , ctrlPointIndex , vertexCounter , &tangent[j]);

				vertexCounter++;

				//D3DXVec3TransformCoord(&vertex[j],&vertex[j],&matrix);
				VertexType0 vertert;
				memset(&vertert,0,sizeof(VertexType0));
				vertert.p = vertex[j];
				vertert.uv = uv[j][0];

				xmUint8 boneInd[4]  = {0};
				xmUint8 weight[4] = {0};
				for (int k = 0; k < 4; ++k)
				{
					if (k < m_vSkin[ctrlPointIndex].m_vBoneInd.size())
					{
						boneInd[k] = m_vSkin[ctrlPointIndex].m_vBoneInd[k];
						weight[k] = m_vSkin[ctrlPointIndex].m_vBoneWeight[k] * 255;
					}
				}
				memcpy(&vertert.b,&boneInd[0],sizeof(xmUint32));
				memcpy(&vertert.w,&weight[0],sizeof(xmUint32));
				
				vertert.b = 0;
				vertert.w = 0x3f3f3f3f;
				xmUint8 testBoneInd[4];
				xmUint8 testWeight[4];
				for (int k = 0; k < 4; ++k)
				{
					testBoneInd[k] = (vertert.b >> (k*8)) & 0xff;
					testWeight[k] = ((vertert.w >> (k*8)) & 0xff);
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

			// 根据读入的信息组装三角形，并以某种方式使用即可，比如存入到列表中、保存到文件等...
		}

		// Ib
		int nIndexCount = indexList.size();
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
		pSubMeshData->m_arrBonePalette.resize(clusterCount);
		for (UINT i = 0; i <clusterCount; ++i)
		{
			pSubMeshData->m_arrBonePalette[i] = i;
		}
		pMeshData->m_arrMeshLOD.push_back(pMeshLodData);

		LoadMaterial(pMesh);
	}

	void FBXImporter::ProcessMesh(MeshData* pMeshData,FbxNode* pNode)
	{
		if (pMeshData == NULL || pNode == NULL)
			return;

		FbxMesh* pFbxMesh = pNode->GetMesh();
		if (pFbxMesh)
		{
			GetMeshData(pFbxMesh,pMeshData);
		}
		else
		{
			for(int i = 0 ; i < pNode->GetChildCount() ; ++i)
			{
				ProcessMesh(pMeshData,pNode->GetChild(i));
			}
		}
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

		pVertex->x = pCtrlPoint[ctrlPointIndex][0];
		pVertex->y = pCtrlPoint[ctrlPointIndex][1];
		pVertex->z = pCtrlPoint[ctrlPointIndex][2];
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
						pColor->x = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
						pColor->y = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
						pColor->z = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
						pColor->w = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
						pColor->x = pVertexColor->GetDirectArray().GetAt(id).mRed;
						pColor->y = pVertexColor->GetDirectArray().GetAt(id).mGreen;
						pColor->z = pVertexColor->GetDirectArray().GetAt(id).mBlue;
						pColor->w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
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
						pColor->x = pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
						pColor->y = pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
						pColor->z = pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
						pColor->w = pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
					}
					break;
				case FbxGeometryElement::eIndexToDirect:
					{
						int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
						pColor->x = pVertexColor->GetDirectArray().GetAt(id).mRed;
						pColor->y = pVertexColor->GetDirectArray().GetAt(id).mGreen;
						pColor->z = pVertexColor->GetDirectArray().GetAt(id).mBlue;
						pColor->w = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
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
			pUV->y = (float)uvValue[1];
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
						pNormal->x = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
						pNormal->y = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
						pNormal->z = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
						pNormal->x = leNormal->GetDirectArray().GetAt(id)[0];
						pNormal->y = leNormal->GetDirectArray().GetAt(id)[1];
						pNormal->z = leNormal->GetDirectArray().GetAt(id)[2];
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
						pNormal->x = leNormal->GetDirectArray().GetAt(vertexCounter)[0];
						pNormal->y = leNormal->GetDirectArray().GetAt(vertexCounter)[1];
						pNormal->z = leNormal->GetDirectArray().GetAt(vertexCounter)[2];
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexCounter);
						pNormal->x = leNormal->GetDirectArray().GetAt(id)[0];
						pNormal->y = leNormal->GetDirectArray().GetAt(id)[1];
						pNormal->z = leNormal->GetDirectArray().GetAt(id)[2];
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
						pTangent->x = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[0];
						pTangent->y = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[1];
						pTangent->z = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[2];
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
						pTangent->x = leTangent->GetDirectArray().GetAt(id)[0];
						pTangent->y = leTangent->GetDirectArray().GetAt(id)[1];
						pTangent->z = leTangent->GetDirectArray().GetAt(id)[2];
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
						pTangent->x = leTangent->GetDirectArray().GetAt(vertecCounter)[0];
						pTangent->y = leTangent->GetDirectArray().GetAt(vertecCounter)[1];
						pTangent->z = leTangent->GetDirectArray().GetAt(vertecCounter)[2];
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertecCounter);
						pTangent->x = leTangent->GetDirectArray().GetAt(id)[0];
						pTangent->y = leTangent->GetDirectArray().GetAt(id)[1];
						pTangent->z = leTangent->GetDirectArray().GetAt(id)[2];
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



