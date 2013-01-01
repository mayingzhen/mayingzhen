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

	MeshData* FBXImporter::LoadScene(const char* pSeneName)
	{
		if(mpFBXSDKManager == NULL)
		{
			return NULL;
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

		// Destroy the importer.
		pFBXImporter->Destroy();

		MeshData* pMeshData = new MeshData;

		FbxAxisSystem fbxAxis = lScene->GetGlobalSettings().GetAxisSystem();
		if (fbxAxis == FbxAxisSystem::OpenGL)
		{
			int i = 5;
		}
		else
		{
			int i = 5;
		}

		ProcessNode( pMeshData, lScene->GetRootNode() );

		return pMeshData;
	}

	void FBXImporter::ProcessNode(MeshData* pMeshData,FbxNode* pNode)
	{
		if (pMeshData == NULL || pNode == NULL)
			return;

		if(pNode->GetNodeAttribute())
		{
			switch(pNode->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
				ProcessMesh(pMeshData,pNode);
				break;
			case FbxNodeAttribute::eSkeleton:
				//ProcessSkeleton(pNode);
				break;
			case FbxNodeAttribute::eLight:
				//ProcessLight(pNode);
				break;
			case FbxNodeAttribute::eCamera:
				//ProcessCamera();
				break;
			}
		}

		for(int i = 0 ; i < pNode->GetChildCount() ; ++i)
		{
			ProcessNode(pMeshData,pNode->GetChild(i));
		}
	}

	void FBXImporter::ProcessMesh(MeshData* pMeshData,FbxNode* pNode)
	{
		if (pMeshData == NULL || pNode == NULL)
			return;

		FbxMesh* pMesh = pNode->GetMesh();
		if(pMesh == NULL)
			return;

		if(!pMesh->IsTriangleMesh())
		{
			FbxGeometryConverter converter(mpFBXSDKManager);
			// #1
			//converter.TriangulateInPlace(pNode);
			//pMesh = dynamic_cast<KFbxMesh*>(pNode->GetNodeAttribute());

			// #2
			pMesh = converter.TriangulateMesh(pMesh);
		}
		
		// Ib
		int nIndexCount = pMesh->GetPolygonVertexCount();
		int* pIndexData = pMesh->GetPolygonVertices();
		pMeshData->m_nIndexType = INDEX_TYPE_U16;
		pMeshData->m_arrIndexBuffer.resize(nIndexCount * sizeof(UINT));
		BoneIndex* pIb = (BoneIndex*)&pMeshData->m_arrIndexBuffer[0];
		for (UINT i = 0; i < nIndexCount; ++i)
		{
			pIb[i] = pIndexData[i];
		}

		// vb
		UINT nVertexCount = pMesh->GetControlPointsCount();
		pMeshData->m_nVertexType = VT_SKIN_VERTEX_0;
		pMeshData->m_arrVertexBuffer.resize(nVertexCount * sizeof(VertexType0));
		VertexType0* pVb = (VertexType0*)&pMeshData->m_arrVertexBuffer[0]; 

		// subMesh
		MeshLODData* pMeshLodData = new MeshLODData;
		SubMeshData* pSubMeshData = new SubMeshData;
		pMeshLodData->m_arrSubMesh.push_back(pSubMeshData);
		pSubMeshData->m_nIndexStart = 0;
		pSubMeshData->m_nIndexCount = nIndexCount;
		pSubMeshData->m_nVertexStart = 0;
		pSubMeshData->m_nVertexCount = nVertexCount;
		pMeshData->m_arrMeshLOD.push_back(pMeshLodData);

		int triangleCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;

		D3DXVECTOR3 vertex[3];
		D3DXVECTOR4 color[3];
		D3DXVECTOR3 normal[3];
		D3DXVECTOR3 tangent[3];
		D3DXVECTOR2 uv[3][2];

		//D3DXMATRIX                matrix;
		//D3DXMatrixRotationX(&matrix, -(3.14/2));

		for(int i = 0 ; i < triangleCount ; ++i)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				int ctrlPointIndex = pMesh->GetPolygonVertex(i , j);

				// Read the vertex
				ReadVertex(pMesh , ctrlPointIndex , &vertex[j]);

				// Read the color of each vertex
				ReadColor(pMesh , ctrlPointIndex , vertexCounter , &color[j]);

				// Read the UV of each vertex
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

				pVb[ctrlPointIndex].p = vertex[j];
				//pVb[ctrlPointIndex].vc = color[j];
				pVb[ctrlPointIndex].uv = uv[j][0];
			}

			// 根据读入的信息组装三角形，并以某种方式使用即可，比如存入到列表中、保存到文件等...
		}

		LoadMaterial(pMesh);
	}

	void FBXImporter::ReadVertex(KFbxMesh* pMesh , int ctrlPointIndex , D3DXVECTOR3* pVertex)
	{
		KFbxVector4* pCtrlPoint = pMesh->GetControlPoints();

		pVertex->x = pCtrlPoint[ctrlPointIndex][0];
		pVertex->y = pCtrlPoint[ctrlPointIndex][1];
		pVertex->z = pCtrlPoint[ctrlPointIndex][2];
	}

	void FBXImporter::ReadColor(KFbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR4* pColor)
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

	void FBXImporter::ReadUV(KFbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , D3DXVECTOR2* pUV)
	{
		if(uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
		{
			return ;
		}

		FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

		switch(pVertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			{
				switch(pVertexUV->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
					{
						pUV->x = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
						pUV->y = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
					}
					break;

				case FbxGeometryElement::eIndexToDirect:
					{
						int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
						pUV->x = pVertexUV->GetDirectArray().GetAt(id)[0];
						pUV->y = pVertexUV->GetDirectArray().GetAt(id)[1];
					}
					break;

				default:
					break;
				}
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			{
				switch (pVertexUV->GetReferenceMode())
				{
				case FbxGeometryElement::eDirect:
				case FbxGeometryElement::eIndexToDirect:
					{
						pUV->x = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
						pUV->y = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
					}
					break;

				default:
					break;
				}
			}
			break;
		}
	}

	void FBXImporter::ReadNormal(KFbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , D3DXVECTOR3* pNormal)
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

	void FBXImporter::ReadTangent(KFbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , D3DXVECTOR3* pTangent)
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



