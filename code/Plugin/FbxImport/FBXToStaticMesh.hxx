#include "FBXToStaticMesh.h"
#include "FbxImportUtil.h"
#include "FBXImportBase.h"


namespace ma
{
	void GetMeshData(FbxMesh* pMesh,MeshData* pMeshData,ImportParm* pImportParm)
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

		std::map< int,std::vector<int> >::iterator it = matIndexToTriangleIndex.begin();
		for (; it != matIndexToTriangleIndex.end(); ++it) 
		{
			// One SunMesh
			RefPtr<SubMeshData> pSubmesh = CreateSubMeshData();
			pMeshData->AddSubMeshData(pSubmesh.get());
			pSubmesh->m_nMateiralID = it->first;

			//submesh->m_pMaterial = CreateMeshMaterial(pMesh,it->first,pImportParm,false);

			pSubmesh->m_nVertexStart = arrVertex.size();
			pSubmesh->m_nIndexStart = arrIndex.size();

			int nVertexSize = arrVertex.size();
			int nIndexSize = arrIndex.size();

			std::vector<int>& arrTriangle = it->second;
			for (UINT i = 0; i < arrTriangle.size(); ++i)
			{
				int nTriangleIndex = arrTriangle[i];

				assert(pMesh->GetPolygonSize(nTriangleIndex) == 3); 

				SkinVertexV0 vertex[3];

				GetTriangleData(pMesh,nTriangleIndex,vertex,pImportParm);

				for(int j = 0; j < 3 ; ++j)
				{
					FbxVector4 fPos(vertex[j].pos.x,vertex[j].pos.y,vertex[j].pos.z,1);
					FbxVector4 fNor(vertex[j].nor.x,vertex[j].nor.y,vertex[j].nor.z,0);

					vertex[j].pos = ToMaUnit( globalTransform.MultT(fPos) );	
					vertex[j].nor = ToMaUnit( globalTransform.MultT(fNor) );	

					UpdateVertexArray(arrVertex,arrIndex,vertex[j]);
				}
			}

			pSubmesh->m_nVertexCount = arrVertex.size() - nVertexSize;
			pSubmesh->m_nIndexCount = arrIndex.size() - nIndexSize;
		}

		vector<StaticVertexV1> pVertexV1;
		pVertexV1.resize(arrVertex.size());
		pMeshData->UpdateMeshData(&pVertexV1[0],&arrVertex[0],arrVertex.size(),&arrIndex[0]);

		pMeshData->GetVertexBuffer()->SetData((uint8*)&pVertexV1[0], sizeof(StaticVertexV1)* pVertexV1.size(), sizeof(StaticVertexV1));
		pMeshData->GetIndexBuffer()->SetData((uint8*)&arrIndex[0], sizeof(UINT16) * arrIndex.size(), sizeof(UINT16));

// 		pMesh->ComputeBBox();
// 		AABB aabb;
// 		aabb.setMinimum( ToMaUnit( (FbxDouble3)pMesh->BBoxMin ) );
// 		aabb.setMaximum( ToMaUnit( (FbxDouble3)pMesh->BBoxMax ) );
// 		pMeshData->SetBoundingAABB(aabb);
	}

	bool LoadStaticMeshData(const char* pFileName,ImportParm* pImportParm,
		const char* pOutMeshFile, const char* pOutMatFile)
	{
		std::string strDir = GetArchiveMananger()->GetSaveDir();	
		std::string strMeshFile = strDir + "/" + string(pFileName);

		std::string strOutMeshFile = pOutMeshFile ? pOutMeshFile : StaticFunc::ReplaceFileExt(pFileName,"skn");
		std::string strOutMatFile = pOutMatFile ? pOutMatFile : StaticFunc::ReplaceFileExt(pFileName,"mat");
	
		RefPtr<MeshData> pMeshData = CreateMeshData();
		pMeshData->SetVertexType(STATIC_VERTEX_1);
		pMeshData->SetIndexType(INDEX_TYPE_U16);

		FbxScene* pFbxScene = GetFbxScene( strMeshFile.c_str() );
		ASSERT(pFbxScene);
		if (pFbxScene == NULL)
			return false;

		FbxMesh* pFbxMesh = GetFbxMesh( pFbxScene->GetRootNode() );

		GetMeshData(pFbxMesh,pMeshData.get(),pImportParm);

		pMeshData->SaveToFile(strOutMeshFile.c_str());	

		return true;
	}

	void UpdateVertexArray(std::vector<SkinVertexV0>& arrVertex,std::vector<UINT16>& arrIndex,SkinVertexV0 vertex)
	{
		UINT index = 0;

		std::vector<SkinVertexV0>::iterator it = std::find(arrVertex.begin(),arrVertex.end(),vertex);
		if (it != arrVertex.end())
		{
			index = it - arrVertex.begin();
		}
		else
		{
			index = arrVertex.size();
			arrVertex.push_back(vertex);
		}

		arrIndex.push_back(index);
	}

 
	void GetTriangleData(FbxMesh* pMesh,int nTriangleIndex, SkinVertexV0 vertex[3],ImportParm* pImportParm)
	{
		for(int j = 0; j < 3 ; ++j)
		{
			int vertexCounter = nTriangleIndex * 3  + j;

			int ctrlPointIndex = pMesh->GetPolygonVertex(nTriangleIndex , j);

			// Read the vertex
			ReadVertex(pMesh , ctrlPointIndex , &vertex[j].pos);


			// Read the color of each vertex
			//ReadColor(pMesh , ctrlPointIndex , vertexCounter , &color);

			ReadUV(pMesh,ctrlPointIndex,pMesh->GetTextureUVIndex(nTriangleIndex, j),0,&vertex[j].uv);

			// Read the normal of each vertex
			//if (pImportParm && pImportParm->m_eImportNormal == Import)
			{
				ReadNormal(pMesh , ctrlPointIndex , vertexCounter , &vertex[j].nor);
			}

			// Read the tangent of each vertex
			if (pImportParm && pImportParm->m_eImportTangents == Import)
			{
				//ReadTangent(pMesh , ctrlPointIndex , vertexCounter , &vertex[j].m_tan);
			}
		}
	}

	void GetTextureNames( FbxProperty &pProperty, std::string& pConnectionString )
	{
		int lLayeredTextureCount = pProperty.GetSrcObjectCount(FbxLayeredTexture::ClassId);
		if(lLayeredTextureCount > 0)
		{
			for(int j=0; j<lLayeredTextureCount; ++j)
			{
				FbxLayeredTexture *lLayeredTexture = FbxCast <FbxLayeredTexture>(pProperty.GetSrcObject(FbxLayeredTexture::ClassId, j));
				int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);

				for(int k =0; k<lNbTextures; ++k)
				{
					pConnectionString += (char*)lLayeredTexture->GetName();
					pConnectionString += " ";
				}
			}
		}
		else
		{
			//no layered texture simply get on the property
			int lNbTextures = pProperty.GetSrcObjectCount(FbxTexture::ClassId);

			if(lNbTextures > 0)
			{
				for(int j =0; j<lNbTextures; ++j)
				{
					FbxTexture* lTexture = FbxCast <FbxTexture> (pProperty.GetSrcObject(FbxTexture::ClassId,j));
					if(lTexture)
					{
						pConnectionString += (char*)lTexture->GetName();
						pConnectionString += " ";
					}
				}

			}
		}
	}

	void GetMaterialTextureData( FbxSurfaceMaterial* pMaterial, subMaterial& smData)
	{
		if(!pMaterial)
			return;

		FbxProperty lProperty;
		//Diffuse Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		GetTextureNames(lProperty, smData.m_strDiff);

		//Emissive Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		GetTextureNames(lProperty, smData.m_strEmiss);

		//Normal Map Textures
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
		GetTextureNames(lProperty, smData.m_strNormal);

	}

	void GetSubMaterialData(FbxMesh* pMesh, std::vector<subMaterial>& arrSubmaterial)
	{
		int i, l, lPolygonCount = pMesh->GetPolygonCount();

		//check whether the material maps with only one mesh
		bool lIsAllSame = true;
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
			if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) 
			{
				lIsAllSame = false;
				break;
			}
		}

		if(lIsAllSame) //For eAllSame mapping type, just out the material and texture mapping info once
		{
			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{
				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
				if( lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame) 
				{
					FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));    
					int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
					if(lMatId >= 0)
					{
						subMaterial smData;
						
						smData.m_matId = lMatId;

						GetMaterialTextureData(lMaterial, smData);

						arrSubmaterial.push_back(smData);
					}
				}
			}
		}
		else //For eByPolygon mapping type, just out the material and texture mapping info once
		{
			for (i = 0; i < lPolygonCount; i++)
			{
				for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
				{
					FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial( l);
					FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
					int lMatId = lMaterialElement->GetIndexArray().GetAt(i);

					if(lMatId >= 0)
					{
						subMaterial smData;

						smData.m_matId = lMatId;

						GetMaterialTextureData(lMaterial, smData);

						arrSubmaterial.push_back(smData);
					}
				}
			}
		}
	}



	void ConnectMaterialToMesh(FbxMesh* pMesh, std::map< int,std::vector<int> >& matIndexToTriangleIndex)
	{
		int triangleCount = pMesh->GetPolygonCount();

		// renew the base layer
		KFbxLayer* pBaseLayer = pMesh->GetLayer(0);
			
		//	get the "material index" layer.  Do this AFTER the triangulation step as that may reorder material indices
		KFbxLayerElementMaterial* pLayerElementMaterial = pBaseLayer->GetMaterials();
		KFbxLayerElement::EMappingMode eMaterialMappingMode = pLayerElementMaterial ? 
			pLayerElementMaterial->GetMappingMode() : KFbxLayerElement::eByPolygon;

		if (pLayerElementMaterial)
		{
			switch(eMaterialMappingMode)
			{
			case KFbxLayerElement::eAllSame:
				{	
					int nMaterialIndex = pLayerElementMaterial->GetIndexArray().GetAt(0);

					for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
					{
						matIndexToTriangleIndex[nMaterialIndex].push_back(triangleIndex);
					}
				}
				break;
			case KFbxLayerElement::eByPolygon:
				{	
					for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
					{
						int nMaterialIndex = pLayerElementMaterial->GetIndexArray().GetAt(triangleIndex);
						matIndexToTriangleIndex[nMaterialIndex].push_back(triangleIndex);
					}
				}
				break;
			}
		}
		else
		{
			for(int triangleIndex = 0 ; triangleIndex < triangleCount ; ++triangleIndex)
			{
				matIndexToTriangleIndex[0].push_back(triangleIndex);
			}
		}
	}

	void ReadVertex(FbxMesh* pMesh , int ctrlPointIndex , Vector3* pVertex)
	{
		FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

		*pVertex = ToMaUnit(pCtrlPoint[ctrlPointIndex]);
	}

	void ReadColor(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector4* pColor)
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

	void ReadUV(FbxMesh* pMesh , int ctrlPointIndex , int textureUVIndex , int uvLayer , Vector2* pUV)
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

	void ReadNormal(FbxMesh* pMesh , int ctrlPointIndex , int vertexCounter , Vector3* pNormal)
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

	void ReadTangent(FbxMesh* pMesh , int ctrlPointIndex , int vertecCounter , Vector3* pTangent)
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

	void LoadMaterialTexture(FbxMesh* pMesh,int materialID,std::string& strDiff,std::string strNormal)
	{
		if (pMesh == NULL)
			return;
		
		FbxNode* pNode = pMesh->GetNode();
		if (pNode == NULL)
			return;

		FbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial(materialID);
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

	void LoadMaterialTexture(FbxSurfaceMaterial* pSurfaceMaterial)
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



