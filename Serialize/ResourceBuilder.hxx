#include "Serialize/ResourceBuilder.h"
#include "Serialize/Serialize/SerializeListener.h"
#include "Serialize/Serialize/BinaryInputArchive.h"
#include "Serialize/MeshData.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
namespace ResourceBuilder
{


// 	Skeleton* LoadSkeleton(SerializeListener* pSL)
// 	{
// 		StackMemoryMark memMarker(GetDefaultStackAllocator());
// 
// 		Skeleton* pSkel = NULL;
// 		ExpSkeletonHeader skelHeader;
// 		ExpSkeletonData skelData;
// 		pSL->Serialize(skelHeader,"Header");
// 		pSL->PushVersion(skelHeader.m_nVersion);
// 		pSL->Serialize(skelData,"Skeleton");
// 		pSL->PopVersion();
// 		pSkel = reinterpret_cast<Skeleton*>(ExpDataConverter::CreateSkeleton(&skelData,skelHeader.m_nVersion));
// 		
// 
// 		GlobalConfig* pCfg = reinterpret_cast<GlobalConfig*>(GetGlobalConfig());
// 		if (pCfg->m_bAutoSocketFromBoneEnable)
// 		{
// 			ArrayAlloc<const char*> arrSockePrefix(GetDefaultStackAllocator());
// 			if (pCfg->m_arrSocketFromBoneNamePrefix.size() < 512)
// 			{
// 				for (xmUint nPCnt = 0; nPCnt < pCfg->m_arrSocketFromBoneNamePrefix.size(); ++nPCnt)
// 				{
// 					arrSockePrefix.push_back(pCfg->m_arrSocketFromBoneNamePrefix[nPCnt].c_str());
// 				}
// 				if (arrSockePrefix.size() > 0)
// 				{
// 					SkelUtil::BuildSocketFromBone(pSkel,&arrSockePrefix[0],arrSockePrefix.size());
// 				}
// 
// 			}else{
// 				LogError(_ERR_INVALID_CALL,"Fail to auto build socket from bone : number of bone prefix is larger then 512\n");
// 			}
// 		}
// 
// //#pragma message( __FILE__ "(" STRING(__LINE__) ") : TODO Remove UpdateSkeletonGUID on loading")
// 		
// 
// 		return pSkel;
// 	}


// 	Animation* LoadAnimation(SerializeListener* pSL)
// 	{
// 		Animation* pAnim = NULL;
// 		ExpAnimationHeader header;
// 		ExpAnimationData data;
// 		BinaryInputArchive ar;
// 
// 		pSL->Serialize(header,"AnimationHeader");
// 
// 		if (header.m_nVersion == EXP_ANIM_VER_CURRENT)
// 		{
// 			pSL->PushVersion(header.m_nVersion);
// 			pSL->Serialize(data,"Animation");
// 			pSL->PopVersion();
// 			pAnim = reinterpret_cast<Animation*>(ExpDataConverter::CreateAnimation(&header,&data,NULL));
// 		}else{
// 			
// 			LogError(_ERR_INVALID_CALL,"Fail to load animation : LoadLegacyAnimation function to load legacy animation ");
// 		}
// 
// 		
// 
// 		return pAnim;
// 
// 	}

// 	Animation* LoadLegacyAnimation(SerializeListener* pSL,ISkeleton* pSkel)
// 	{
// 		Animation* pAnim = NULL;
// 		ExpAnimationHeader header;
// 		ExpAnimationData data;
// 		BinaryInputArchive ar;
// 
// 		pSL->Serialize(header,"AnimationHeader");
// 
// 		bool bUpgradeOK = true;
// 		if (header.m_nIden != 'SANM')
// 		{
// 			LogError(_ERR_INVALID_CALL,"Invalid animation file");
// 			bUpgradeOK = false;
// 		}
// 
// 		if (bUpgradeOK)
// 		{
// 			pSL->PushVersion(header.m_nVersion);
// 			pSL->Serialize(data,"Animation");
// 			pSL->PopVersion();
// 
// 			bUpgradeOK = ExpDataConverter::UpgradeAnimationData(&header,&data
// 				,pSkel);
// 		}
// 
// 
// 
// 		if (bUpgradeOK)
// 		{
// 			pAnim = reinterpret_cast<Animation*>(ExpDataConverter::CreateAnimation(&header,&data,pSkel));
// 
// 		}else{
// 			LogError(_ERR_INVALID_CALL,"fail to upgrade animation\n");
// 		}
// 
// 		
// 
// 		return pAnim;
// 
// 	}

// 	bool LoadMeshFromFileVersion1(ExpMesh* pMesh,SerializeListener* pSL)
// 	{
// 		pSL->Seek(0);
// 
// 		S3_ANIM_MODEL_FILE_HEADER modelHeader;
// 		char szModelName[128];
// 		S3_VERTEX_DATA* arrSrcVertexData = NULL;
// 		xmUint16* arrSrcIndexData = NULL;
// 		S3_LOD_MESHVIEW lodMeshView;
// 		S3_GEOMETRY_CHUNK* arrSubMeshInfo = NULL;
// 
// 		pSL->SerializeByte((xmUint8*)&modelHeader,sizeof(S3_ANIM_MODEL_FILE_HEADER));
// 		pSL->SerializeByte((xmUint8*)szModelName,modelHeader.nModelNameSize + 1);
// 		{
// 			arrSrcVertexData = new S3_VERTEX_DATA[modelHeader.nNumVertices];
// 			pSL->SerializeByte((xmUint8*)arrSrcVertexData,sizeof(S3_VERTEX_DATA)* modelHeader.nNumVertices);
// 		}
// 		pSL->SerializeByte((xmUint8*)&lodMeshView,sizeof(S3_LOD_MESHVIEW));
// 		pSL->SkipByte(sizeof(xmUint16)* modelHeader.nNumVertices);
// 		{
// 			arrSrcIndexData = new xmUint16[lodMeshView.nPrimitivesCount];
// 			pSL->SerializeByte((xmUint8*)arrSrcIndexData,sizeof(xmUint16)*lodMeshView.nPrimitivesCount);
// 		}
// 
// 		{
// 			arrSubMeshInfo = new S3_GEOMETRY_CHUNK[lodMeshView.nNumGeoChunks];
// 			pSL->SerializeByte((xmUint8*)arrSubMeshInfo,sizeof(S3_GEOMETRY_CHUNK)*lodMeshView.nNumGeoChunks);
// 		}
// 
// 
// 		{
// 			ExpMeshHeader& header = pMesh->m_header;
// 			header.m_nIdent = modelHeader.nIdent;
// 			header.m_nVersion = 0;
// 			header.m_strMaxFile = szModelName;//TODO Resolve String
// 			header.m_nSkelGUID.m_a = modelHeader.nGlobalSkeletonID;
// 			header.m_nSkelGUID.m_b = 0;
// 			header.m_nBoneNum = modelHeader.nBonesCount;
// 			header.m_nIndexNum = lodMeshView.nPrimitivesCount;
// 			header.m_nVertexNum = modelHeader.nNumVertices;
// 		}
// 
// 		{
// 			ExpMeshData& meshData = pMesh->m_mesh;
// 			//xmUint m_nIndexType;
// 			//xmUint m_nVertexType;
// 			//std::vector<xmUint8>	m_arrIndexBuffer;
// 			//std::vector<xmUint8>	m_arrVertexBuffer;
// 			//std::vector<std::string>	m_arrBoneName;
// 
// 			//std::vector<ExpMeshLOD>	m_arrMeshLOD;
// 			//std::vector<ExpBounding> m_arrBoneBound;
// 			//ExpBounding				m_meshBound;
// 
// 			meshData.m_nIndexType	= EXP_INDEX_TYPE_U16;
// 			meshData.m_nVertexType	= EXP_VT_SKIN_VERTEX_0;
// 			meshData.m_arrIndexBuffer.resize(sizeof(xmUint16)*	pMesh->m_header.m_nIndexNum);
// 			meshData.m_arrVertexBuffer.resize(sizeof(ExpVertexType0)* pMesh->m_header.m_nVertexNum);
// 			meshData.m_arrBoneName.clear();		//Not avaible
// 			meshData.m_arrMeshLOD.resize(1);
// 			meshData.m_arrBoneBound.clear();	//Not avaible
// 			meshData.m_meshBound.SetInvalid();	//Not avaible
// 
// 			//fill index buffer vertex buffer
// 			{
// 				memcpy(&meshData.m_arrIndexBuffer[0],arrSrcIndexData,meshData.m_arrIndexBuffer.size());
// 
// 				for (xmUint nVCnt = 0; nVCnt < modelHeader.nNumVertices; ++nVCnt)
// 				{
// 					ExpVertexType0* pDestV = (ExpVertexType0*)&meshData.m_arrVertexBuffer[nVCnt*sizeof(ExpVertexType0)];
// 					memcpy(pDestV,&arrSrcVertexData[nVCnt],sizeof(S3_VERTEX_DATA));
// 					pDestV->vc = 0xffffffff;
// 					pDestV->vi = 0;
// 				}
// 			}
// 
// 			//fill mesh
// 			{
// 				std::vector<ExpSubMesh>& arrSubMesh = pMesh->m_mesh.m_arrMeshLOD[0].m_arrSubMesh;
// 				arrSubMesh.resize(lodMeshView.nNumGeoChunks);
// 				for (xmUint nSubMeshCnt = 0; nSubMeshCnt < lodMeshView.nNumGeoChunks; ++nSubMeshCnt)
// 				{
// 					ExpSubMesh& subMesh = arrSubMesh[nSubMeshCnt];
// 
// 					subMesh.m_nIndexStart = arrSubMeshInfo[nSubMeshCnt].nPrimitiveIndexBegin;
// 					subMesh.m_nIndexCount = arrSubMeshInfo[nSubMeshCnt].nNumPrimitives;
// 					subMesh.m_nVertexStart = arrSubMeshInfo[nSubMeshCnt].nVertexIndexBegin;
// 					subMesh.m_nVertexCount = arrSubMeshInfo[nSubMeshCnt].nNumVertices;
// 					subMesh.m_subMeshBound.SetInvalid();
// 					subMesh.m_name = StringConverter::ToString(arrSubMeshInfo[nSubMeshCnt].dwPartID);
// 					subMesh.m_submeshTag = subMesh.m_name;
// 					subMesh.m_arrBonePalette.clear();
// 				}
// 
// 			}
// 
// 		}
// 
// 		delete [] arrSrcVertexData;
// 		delete [] arrSrcIndexData;
// 		delete [] arrSubMeshInfo;
// 
// 		return true;
// 	}



// 	namespace UpdateInuseBoneAdapter
// 	{
// 		class BonePalatteAdapter : public MeshUtil::UpdateInuseBoneHelper::BonePalatte
// 		{
// 		public:
// 
// 			std::vector<xmUint16> m_arrBone;
// 
// 			void Clear()
// 			{
// 				m_arrBone.clear();
// 			}
// 
// 			void PushBack(xmUint nBoneInd)
// 			{
// 				m_arrBone.push_back((xmUint16)nBoneInd);
// 			}
// 
// 
// 			xmUint GetPalatteSize() const
// 			{
// 				return (xmUint) m_arrBone.size();
// 			}
// 
// 
// 			xmUint GetBoneByIndex(xmUint nPalInd)
// 			{
// 				return m_arrBone[nPalInd];
// 			}
// 
// 
// 			void SetBoneByIndex(xmUint nPalInd,xmUint nBoneInd)
// 			{
// 				m_arrBone[nPalInd] = (xmUint16)nBoneInd;
// 			}
// 		};
// 
// 		class MeshAdapter : public MeshUtil::UpdateInuseBoneHelper::Mesh
// 		{
// 		public:
// 
// 			VertexType0* m_pVertex;
// 			xmUint				m_nVNum;
// 
// 			xmUint			GetVertexNumber() const
// 			{
// 				return m_nVNum;
// 			}
// 
// 
// 
// 			xmUint	GetVertexInfluenceNumber(xmUint nVInd) const
// 			{
// 				return 4;
// 			}
// 
// 
// 			xmUint	GetVertexInfluenceBoneByIndex(xmUint nVInd,xmUint nInfInd) const
// 			{
// 				SSERT(nVInd < m_nVNum);
// 				return ((xmUint8*)&m_pVertex[nVInd].m_nBoneIndice)[nInfInd];
// 			}
// 
// 
// 			float	GetVertexInfluenceWeightByIndex(xmUint nVInd,xmUint nInfInd) const
// 			{
// 				xmUint8 nWeight = ((xmUint8*)&m_pVertex[nVInd].m_nBoneWeight)[nInfInd];
// 				return (float)nWeight / 255.0f;
// 			}
// 
// 
// 			void	ResizeVertexInfluence(xmUint nVInd,xmUint nInfSize)
// 			{
// 				m_pVertex[nVInd].m_nBoneIndice = 0;
// 				m_pVertex[nVInd].m_nBoneWeight = 0;
// 			}
// 
// 
// 			void	SetVertexInfluenceBoneByIndex(xmUint nVInd,xmUint nInfInd,xmUint nBoneInd)
// 			{
// 				SSERT_MSG(nInfInd < 4,"Fail to set bone influence : nInfInd out of range");
// 				SSERT_MSG(nBoneInd <= 255, "Failt set bone influence : nBoneInd out of range");
// 				((xmUint8*)(&m_pVertex[nVInd].m_nBoneIndice))[nInfInd] = nBoneInd;
// 			}
// 
// 
// 			void	SetVertexInfluenceWeightByIndex(xmUint nVInd,xmUint nInfInd,float fWeight)
// 			{
// 				SSERT_MSG(nInfInd < 4,"Fail to set bone influence : nInfInd out of range");
// 				SSERT_MSG(fWeight <= 1.0f, "Failt set bone influence : nBoneInd out of range");
// 
// 				xmUint8 nWeight = (xmUint8)(fWeight * 255.0f);
// 				((xmUint8*)(&m_pVertex[nVInd].m_nBoneWeight))[nInfInd] = nWeight;
// 			}
// 		};
// 	}

 	bool UpdateBonePalette(MeshData* pMeshData)
 	{
// 
// 		VertexType0* pVertexBuffer = pMeshData->GetVertexBuffer();
// 		SSERT(NULL != pVertexBuffer);
// 
// 
// 		UpdateInuseBoneAdapter::MeshAdapter meshAdapter;
// 		UpdateInuseBoneAdapter::BonePalatteAdapter bonePalatteAdatper;
// 
// 		xmUint								nMaxBoneNum;
// 		std::vector<xmUint16> arrBonePalatte;
// 
// 
// 		meshAdapter.m_pVertex	= pVertexBuffer;
// 		meshAdapter.m_nVNum		= pMeshData->GetVertexNumber();
// 
// 		nMaxBoneNum = MeshUtil::FindMaxBoneIndex(&meshAdapter) + 1;
// 		arrBonePalatte.resize(nMaxBoneNum);
// 
// 		const xmUint nSubMeshNum	= pMeshData->GetSubMeshNumber();
// 		for (xmUint nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
// 		{
// 			ISubMeshData* pSubmesh = pMeshData->GetSubMesh(nSubMeshCnt);
// 
// 			if (pSubmesh->GetBonePaletteSize() == 0)
// 			{
// 
// 				meshAdapter.m_pVertex = pVertexBuffer + pSubmesh->GetVertexStart();
// 				meshAdapter.m_nVNum = pSubmesh->GetVertexCount();
// 
// 
// 
// 				MeshUtil::UpdateInuseBone(&bonePalatteAdatper,&meshAdapter);
// 
// 				if (bonePalatteAdatper.m_arrBone.size() > 0)
// 				{
// 					pSubmesh->ResetBonePalatte(bonePalatteAdatper.m_arrBone.size(),&bonePalatteAdatper.m_arrBone[0]);
// 				}
// 
// 			}
// 		}
// 
 		return true;
 	}

// 	void UpdateBounding(ExpMeshData* pMesh)
// 	{
// 		SSERT( EXP_VT_SKIN_VERTEX_0 == pMesh->m_nVertexType);
// 		const ExpVertexType0* arrVertex = reinterpret_cast<const ExpVertexType0*>(& pMesh->m_arrVertexBuffer[0]);
// 		std::vector<ExpSubMesh>& arrSubmesh = pMesh->m_arrMeshLOD[0].m_arrSubMesh;
// 
// 		ExpAABBShape meshAABB;
// 		meshAABB.Init();
// 
// 		for (xmUint nSubMeshCnt = 0; nSubMeshCnt < arrSubmesh.size(); ++nSubMeshCnt)
// 		{
// 			ExpSubMesh& submesh = arrSubmesh[nSubMeshCnt];
// 
// 			ExpAABBShape subMeshAABB;
// 			subMeshAABB.Init();
// 
// 			for (xmUint nVerCnt = 0; nVerCnt < submesh.m_nVertexCount; ++nVerCnt)
// 			{
// 				const ExpVertexType0& vertex = *(arrVertex + submesh.m_nVertexStart+nVerCnt);
// 				subMeshAABB.AddPoint(vertex.p);
// 			}
// 
// 			meshAABB.Merge(subMeshAABB.m_vMin,subMeshAABB.m_vMax);
// 
// 			submesh.m_subMeshBound.SetAABB(subMeshAABB.m_vMin,subMeshAABB.m_vMax);
// 		}
// 
// 		pMesh->m_meshBound.SetAABB(meshAABB.m_vMin,meshAABB.m_vMax);
// 	}



// 	bool CreateMeshData(MeshData* pMesh,const ExpMesh* pMeshExp)
// 	{
// 
// 		const std::vector<ExpSubMesh>& arrSrcSubMesh = pMeshExp->m_mesh.m_arrMeshLOD[0].m_arrSubMesh;
// 		const xmUint nSubMeshNum = (xmUint)arrSrcSubMesh.size();
// 
// 		pMesh->SetSource(pMeshExp->m_header.m_strMaxFile.c_str());
// 		pMesh->SetGlobalSkeletonID(pMeshExp->m_header.m_nSkelGUID);
// 		pMesh->ResetBuffer(pMeshExp->m_mesh.m_nIndexType,pMeshExp->m_header.m_nIndexNum
// 			,pMeshExp->m_header.m_nVertexNum
// 			,nSubMeshNum
// 			,pMeshExp->m_header.m_nBoneNum
// 			);
// 
// 		//------------------------------------------------------------------------------
// 		//fill stream source
// 		//------------------------------------------------------------------------------
// 		const std::vector<xmUint8>& arrSrcIndexBuffer = pMeshExp->m_mesh.m_arrIndexBuffer;
// 		const std::vector<xmUint8>& arrSrcVertexBuffer = pMeshExp->m_mesh.m_arrVertexBuffer;
// 		memcpy(pMesh->GetIndexBuffer(),arrSrcIndexBuffer.size() > 0 ? &arrSrcIndexBuffer[0] : NULL,arrSrcIndexBuffer.size());
// 		memcpy(pMesh->GetVertexBuffer(),arrSrcVertexBuffer.size() > 0 ? &arrSrcVertexBuffer[0] : NULL,arrSrcVertexBuffer.size());
// 
// 		//------------------------------------------------------------------------------
// 		//fill sub mesh
// 		//------------------------------------------------------------------------------
// 		for (xmUint nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
// 		{
// 			const ExpSubMesh& srcSubMesh = arrSrcSubMesh[nSubMeshCnt];
// 			ISubMeshData* pSubMesh = pMesh->GetSubMesh(nSubMeshCnt);
// 			pSubMesh->Reset(srcSubMesh.m_nIndexStart,srcSubMesh.m_nIndexCount
// 				,srcSubMesh.m_nVertexStart,srcSubMesh.m_nVertexCount
// 				,0 //Unknown material id
// 				,&srcSubMesh.m_subMeshBound
// 				,srcSubMesh.m_arrBonePalette.size() > 0 ? &srcSubMesh.m_arrBonePalette[0] : NULL
// 				,srcSubMesh.m_arrBonePalette.size()
// 				,srcSubMesh.m_name.c_str()
// 				,srcSubMesh.m_submeshTag.c_str());
// 		}
// 
// 		//------------------------------------------------------------------------------
// 		//fill skin info
// 		//------------------------------------------------------------------------------
// 		for (xmUint nBoneCnt = 0; nBoneCnt < pMeshExp->m_mesh.m_arrBoneName.size(); ++ nBoneCnt)
// 		{
// 			pMesh->SetBoneName(nBoneCnt,pMeshExp->m_mesh.m_arrBoneName[nBoneCnt].c_str());
// 			if (pMeshExp->m_mesh.m_arrBoneBound.size() > 0)
// 			{
// 				const ExpBounding&  bound = pMeshExp->m_mesh.m_arrBoneBound[nBoneCnt];
// 				SSERT(bound.m_nShapeType == EXP_BS_BOX);
// 				pMesh->SetBoneBoundOBB(nBoneCnt,&bound.m_vPos,&bound.m_qRot,bound.m_boxShape.m_fXSize
// 					,bound.m_boxShape.m_fYSize
// 					,bound.m_boxShape.m_fZSize);
// 			}
// 
// 		}
// 
// 
// 		//------------------------------------------------------------------------------
// 		//fill bounding
// 		//------------------------------------------------------------------------------
// 		xmVector3 vMin;
// 		xmVector3 vMax;
// 		pMeshExp->m_mesh.m_meshBound.GetAABB(vMin,vMax);
// 		pMesh->SetBoundingAABB(&vMin,&vMax);
// 
// 		return true;
// 
// 	}

	bool LoadMesh(MeshData* pMesh,SerializeListener* pSL)
	{
		//S3_TRY
		//{
			xmUint nMeshBegin = pSL->Tell();

			xmUint32		nIdent;	 // File Identity = 'S3MD'
			xmUint32		nVersion;	 // ¸ñÊ½°æ±¾ºÅ;
			bool bFileIdenOK = pSL->SerializeByte((xmUint8*)&nIdent,sizeof(xmUint32));

			if (!bFileIdenOK || nIdent != 'S3MD')
			{
				return false;
			}

			pSL->SerializeByte((xmUint8*)&nVersion,sizeof(xmUint32));
			pSL->Seek(nMeshBegin);



			bool bLoadOK = true;

			//
			//

			//if (nVersion >= EXP_MESH_VER_EMBED_MESH_DATA)
			//{
				pMesh->Serialize(pSL,"Mesh");

			//}
			//else{
// 
// 				ExpMesh meshExp;
// 
// 				if (nVersion >= EXP_MESH_VER_USE_SERIALZIE)
// 				{
// 					Serialize(*pSL,meshExp,"Mesh");
// 				}else{
// 					bLoadOK = LoadMeshFromFileVersion1(&meshExp,pSL);
// 					UpdateBounding(&meshExp.m_mesh);
// 
// 					if (!bLoadOK)
// 					{
// 						LogError(_ERR_INVALID_CALL,"Fail to load mesh from version1 file");
// 					}
// 				}
// 
// 				if (bLoadOK)
// 				{
// 					bLoadOK = CreateMeshData(pMesh,&meshExp);
// 				}					
			//}


			//if (bLoadOK)
			//{
			//	bLoadOK = UpdateBonePalette(pMesh);
			//}

			return true;

		//}
		//S3_CATCH(...)
		//{
		//	LogError(_ERR_INVALID_CALL,"Fail to serialize mesh");
		//}

		return false;

	}


	bool	SaveMesh(MeshData* pMesh, SerializeListener* pSL)
	{
		//S3_TRY
		//{

			pMesh->Serialize(pSL, "Mesh");

			return true;

		//}
		//S3_CATCH(...)
		//{
		//	LogError(_ERR_INVALID_CALL,"Fail to serialize mesh");
		///}

		return false;

	}

	MeshData* LoadMeshFromBinaryFile(const char* filename)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return NULL;
		}

		MeshData* pMeshData = new MeshData;
		pMeshData->Serialize(&ar,"Mesh");
		//bLoadOK = ResourceBuilder::LoadMesh(pMeshData,&ar);
		UpdateBonePalette(pMeshData);
		pMeshData->RemoveDegenerateTriangleInplace();
// 		if (!bLoadOK)
// 		{
// 			xmSafeDelete(pMeshData);
// 			LogError(_ERR_INVALID_CALL,"Fail to load mesh from file %s:",filename);
// 			return NULL;
// 		}

		return pMeshData;
	}

	SkeletonData* LoadSkeletonFromBinaryFile(const char* filename)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return NULL;
		}
	
		//SkeletonHeader skelHeader;
		//ar.Serialize(skelHeader,"Header");

		SkeletonData* pSkelData = new SkeletonData();
		pSkelData->Serialize(ar, "Skeleton");

		return pSkelData;
	}

	AnimationData* LoadAnimationFromBinaryFile(const char* fileName)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(fileName);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",fileName);
			return NULL;
		}

		AnimationData* pAnimationData = new AnimationData();
		pAnimationData->Serialize(ar, "Animation");

		return pAnimationData;
	}
}