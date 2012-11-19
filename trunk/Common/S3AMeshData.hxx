//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/8/9
// Describe: S3 Animation
//----------------------------------------------------------------------


#include "Common/S3AMeshData.h"
#include "Common/S3AMathSerialize.h"


/*!
param val
Null element is not allowed in array.
T must be a simple struct or class which do not has derived class.
*/
template<class T>
void S3ASerialize(IS3ASerializeListener& sl,std::vector<T*>& val,const char* pszLable = "array")
{
	sl.BeginSection(pszLable);

	xmUint nSize = (xmUint)val.size();
	S3ASerialize(sl,nSize,"size");

	if (nSize != val.size())
	{
		val.resize(nSize);
	}
	sl.BeginSection("element");

	for (xmUint nCnt = 0;nCnt < nSize; ++nCnt)
	{
		char buf[32];
		sprintf_s(&buf[0],32,"Element_%u",nCnt);
		
		//Null element is not allowed in array
		if (val[nCnt] == NULL)
		{
			val[nCnt] = new T;//T must be a simple struct or class which do not has derived class
		}

		S3ASerialize(sl,*val[nCnt],buf);
	}
	sl.EndSection();

	sl.EndSection();
}

template<class T>
void S3APointerArrayClear(T& arr)
{
	for (xmUint nCnt = 0; nCnt < (xmUint)arr.size(); ++nCnt)
	{
		xmSafeDelete(arr[nCnt]);
	}
	arr.clear();
};


template<class ElementType >
void S3APointerArrayResize(std::vector<ElementType*>& arr,xmUint nNewSize)
{
	xmUint nOldSize = (xmUint)arr.size();
	for (xmUint nCnt = nNewSize; nCnt < nOldSize; ++nCnt)
	{
		xmSafeDelete(arr[nCnt]);
	}
	arr.resize(nNewSize);
	for (xmUint nCnt = nOldSize; nCnt < nNewSize; ++nCnt)
	{
		arr[nCnt] = new ElementType;
	}
}

template<class DataType>
void S3ASerializeRawData(IS3ASerializeListener& sl,std::vector<xmUint8>& val,const char* pszLable)
{
	sl.BeginSection(pszLable);

	xmUint nSize = val.size();
	S3ASerialize(sl,nSize,"size");

	if (nSize != val.size())
	{
		val.resize(nSize);
	}

	sl.BeginSection("element");

	xmUint nDataNum = nSize / sizeof(DataType);

	for (xmUint nCnt = 0;nCnt < nDataNum; ++nCnt)
	{
		char buf[64];
		sprintf_s(&buf[0],64,"Element_%u",nCnt);
		S3ASerialize(sl,(DataType&)val[nCnt*sizeof(DataType)],buf);
	}
	sl.EndSection();

	sl.EndSection();
}

S3ASubMeshData::S3ASubMeshData()
:m_nIndexStart(0)
,m_nIndexCount(0)
,m_nVertexStart(0)
,m_nVertexCount(0)
,m_nMateiralID(0)
{
	
}

void	S3ASubMeshData::Reset(xmUint nIndexStart,xmUint nIndexCount
			  ,xmUint nVertexStart,xmUint nVertexCount
			  ,xmUint nMtlID
			  ,const S3AExpBounding* pBounding
			  ,const S3ABoneIndex* arrBonePalatte
			  ,xmUint nBonePalatteSize
			  ,const char* pszName
			  ,const char* pszTag )
{
	m_nIndexStart = nIndexStart;
	m_nIndexCount = nIndexCount;
	m_nVertexStart = nVertexStart;
	m_nVertexCount = nVertexCount;

	m_nMateiralID = nMtlID;

	if (pBounding)
	{
		m_subMeshBound = *pBounding;
	}else{
		m_subMeshBound.SetIdentity();
	}
	m_arrBonePalette.resize(nBonePalatteSize);

	if (arrBonePalatte)
	{
		for (xmUint nPalCnt = 0; nPalCnt < nBonePalatteSize; ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = arrBonePalatte[nPalCnt];
		}
	}else{
		for (xmUint nPalCnt = 0; nPalCnt < nBonePalatteSize; ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = S3AInvalidID<S3ABoneIndex>();
		}
	}

	m_name = pszName;
	m_submeshTag = pszTag;
}


const char* S3ASubMeshData::GetName() const
{
	return m_name.c_str();
}


const char*	S3ASubMeshData::GetTag() const
{
	return m_submeshTag.c_str();
}


xmUint	S3ASubMeshData::GetIndexStart() const
{
	return m_nIndexStart;
}


xmUint	S3ASubMeshData::GetIndexCount() const
{
	return m_nIndexCount;
}


xmUint	S3ASubMeshData::GetVertexStart() const
{
	return m_nVertexStart;
}


xmUint	S3ASubMeshData::GetVertexCount() const
{
	return m_nVertexCount;
}

xmUint	S3ASubMeshData::GetMaterialID() const
{
	return m_nMateiralID;
}

S3ABoneIndex	S3ASubMeshData::GetBonePaletteSize() const
{
	return m_arrBonePalette.size();
}

const S3ABoneIndex*	S3ASubMeshData::GetBonePalette() const
{
	return m_arrBonePalette.size() > 0 ? &m_arrBonePalette[0] : NULL;
}


//------------------------------------------------------------------------------
//For Modification
//------------------------------------------------------------------------------
void	S3ASubMeshData::SetName(const char* pszName)
{
	m_name = pszName;
}


void	S3ASubMeshData::SetTag(const char* pszTag)
{
	m_submeshTag = pszTag;
}



void	S3ASubMeshData::SetIndexStart(xmUint32 nIndStart)
{
	m_nIndexStart = nIndStart;
}


void	S3ASubMeshData::SetIndexCount(xmUint32 nIndCnt)
{
	m_nIndexCount = nIndCnt;
}


void	S3ASubMeshData::SetVertexStart(xmUint32 nVStart)
{
	m_nVertexStart = nVStart;
}


void	S3ASubMeshData::SetVertexCount(xmUint32 nVCnt)
{
	m_nVertexCount = nVCnt;
}

void	S3ASubMeshData::SetMaterialID(xmUint nMtlID)
{
	m_nMateiralID = nMtlID;
}

/*!\brief
\param arrBoneIndice array of bone indices, if null, default bone will be filled
*/
void	S3ASubMeshData::ResetBonePalatte(xmUint32 nPalatteSize,xmUint16* arrBoneIndice)
{
	m_arrBonePalette.resize(nPalatteSize);

	if (NULL == arrBoneIndice)
	{
		for (xmUint nPalCnt = 0; nPalCnt < m_arrBonePalette.size(); ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = nPalCnt;
		}
	}else{
		for (xmUint nPalCnt = 0; nPalCnt < m_arrBonePalette.size(); ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = arrBoneIndice[nPalCnt];
		}
	}
	
}


void	S3ASubMeshData::SetBoneByPalatteIndex(xmUint16 nPalatteInd,xmUint16 nBoneInd)
{
	m_arrBonePalette[nPalatteInd] = nBoneInd;
}


xmUint16	S3ASubMeshData::GetBoneByPalatteIndex(xmUint16 nPalatteInd)
{
	return m_arrBonePalette[nPalatteInd];
}


void	S3ASubMeshData::Serialize(IS3ASerializeListener& sl,const char* pszLabel)
{
	sl.BeginSection(pszLabel);

	S3ASerialize(sl,m_nIndexStart,"IndexStart");
	S3ASerialize(sl,m_nIndexCount,"IndexCount");
	S3ASerialize(sl,m_nVertexStart,"VertexStart");
	S3ASerialize(sl,m_nVertexCount,"VertexCount");
	S3ASerialize(sl,m_subMeshBound,"SubMeshBound");
	S3ASerialize(sl,m_arrBonePalette,"BonePalette");
	S3ASerialize(sl,m_name,"SubMeshName");
	S3ASerialize(sl,m_submeshTag,"SubMeshTag");
    if (sl.GetVersion() >= EXP_MESH_VER_EMBED_MESH_DATA)
    {
		S3ASerialize(sl,m_nMateiralID,"MaterialID");
    }

	sl.EndSection();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
S3AMeshLODData::S3AMeshLODData()
: m_nMeshDataFlag(DATA_PARENT_DATA)
, m_nBoneLOD(0)
,m_nVertexType(EXP_VT_SKIN_VERTEX_0)
{
}

S3AMeshLODData::~S3AMeshLODData()
{
	ClearSubMesh();
}


// const S3ASkinVertexType0*	S3AMeshLODData::GetVertexBufferSkinVertexType0() const
// {
// 	const xmUint8* pData = m_arrVertexBuffer.size() > 0
// 		? &m_arrVertexBuffer[0]
// 	: NULL;
// 
// 	return EXP_VT_SKIN_VERTEX_0 == m_nVertexType 
// 		? reinterpret_cast<const S3ASkinVertexType0*>(pData)
// 		: NULL;
// }

S3ASkinVertexType0*	S3AMeshLODData::GetVertexBufferSkinVertexType0()
{
	xmUint8* pData = m_arrVertexBuffer.size() > 0 ? &m_arrVertexBuffer[0] : NULL;

	return EXP_VT_SKIN_VERTEX_0 == m_nVertexType ? reinterpret_cast<S3ASkinVertexType0*>(pData) : NULL;
}

const IS3ASubMeshData*	S3AMeshLODData::GetSubMesh(xmUint nSubMeshInd) const
{
	return m_arrSubMesh[nSubMeshInd];
}


void					S3AMeshLODData::ResizeSubMesh(xmUint nSubMeshNum)
{
	S3APointerArrayResize(m_arrSubMesh,nSubMeshNum);
}

IS3ASubMeshData*			S3AMeshLODData::CreateSubMesh()
{
	S3ASubMeshData* pSubMesh = new S3ASubMeshData;
	m_arrSubMesh.push_back(pSubMesh);
	return pSubMesh;
}

xmUint					S3AMeshLODData::GetSubMeshNumber() const
{
	return m_arrSubMesh.size();
}

IS3ASubMeshData*			S3AMeshLODData::GetSubMesh(xmUint nSubMeshInd)
{
	return m_arrSubMesh[nSubMeshInd];
}

int						S3AMeshLODData::GetBoneLOD() const
{
	return m_nBoneLOD;
}

void					S3AMeshLODData::SetBoneLOD(int nBoneLOD)
{
	m_nBoneLOD = nBoneLOD;
}

xmUint					S3AMeshLODData::GetMeshDataFlag() const
{
	return m_nMeshDataFlag;
}

void					S3AMeshLODData::SetMeshDataFlag(xmUint uMeshDataFlag)
{
	m_nMeshDataFlag = uMeshDataFlag;
}

xmUint					S3AMeshLODData::GetIndexNumber() const
{
	return m_arrIndexBuffer.size() / (GetIsIndex32() ? sizeof(xmUint32) : sizeof(xmUint16));
}

bool					S3AMeshLODData::GetIsIndex32() const
{
	return m_nIndexType == EXP_INDEX_TYPE_U32;
}

int						S3AMeshLODData::GetVertexNumber() const
{
	return m_arrVertexBuffer.size() / sizeof(S3ASkinVertexType0);
}

void					S3AMeshLODData::ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum)
{
	m_nIndexType = bIsIndex32 ? EXP_INDEX_TYPE_U32 : EXP_INDEX_TYPE_U16;
	m_nVertexType = EXP_VT_SKIN_VERTEX_0;

	const xmUint nIndexStride = m_nIndexType == EXP_INDEX_TYPE_U16 ? sizeof(xmUint16) : sizeof(xmUint32);
	const xmUint nVertexStride = sizeof(S3ASkinVertexType0);

	m_arrIndexBuffer.resize(nIndexNum * nIndexStride);
	m_arrVertexBuffer.resize(nVertexNum * nVertexStride);
}

void					S3AMeshLODData::ClearSubMesh()
{
	for (xmUint nSubMeshCnt = 0; nSubMeshCnt < (xmUint)m_arrSubMesh.size(); ++nSubMeshCnt)
	{
		xmSafeDelete(m_arrSubMesh[nSubMeshCnt]);
	}
	m_arrSubMesh.clear();
}

void	S3AMeshLODData::Serialize(IS3ASerializeListener& sl,const char* pszLabel)
{
	sl.BeginSection(pszLabel);
	if(sl.GetVersion() >= EXP_MESH_VER_LOD_MESH_DATA)
	{
		S3ASerialize(sl, m_nMeshDataFlag, "MeshDataFrom");
	}

	if (m_nMeshDataFlag & DATA_HAS_OWN_INDEX)
	{
		S3ASerialize(sl,m_nIndexType,"IndexType");

		if (EXP_INDEX_TYPE_U16 == m_nIndexType)
		{
			S3ASerializeRawData<S3ABoneIndex>(sl,m_arrIndexBuffer,"IndexBuffer");
		}
		else if (EXP_INDEX_TYPE_U32 == m_nIndexType)
		{
			S3ASerializeRawData<xmUint32>(sl,m_arrIndexBuffer,"IndexBuffer");
		}
		else
		{
			S3ALogError(S3A_ERR_INVALID_CALL,"Fail to serialize mesh data : invalid index type");
		}
	}

	if (m_nMeshDataFlag & DATA_HAS_OWN_VERTEX)
	{
		S3ASerialize(sl,m_nVertexType,"VertexType");

		if (EXP_VT_SKIN_VERTEX_0 == m_nVertexType)
		{
			S3ASerializeRawData<S3AExpVertexType0>(sl,m_arrVertexBuffer,"VertexBuffer");
		}
	}

	if (m_nMeshDataFlag & DATA_HAS_BONE_LOD)
	{
		S3ASerialize(sl, m_nBoneLOD, "BoneLOD");
	}

	S3ASerialize(sl,m_arrSubMesh,"SubMeshArray");

	sl.EndSection();
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
S3AMeshData::S3AMeshData()
:m_nIndexType(EXP_INDEX_TYPE_U16)
,m_nVertexType(EXP_VT_SKIN_VERTEX_0)
{
	//S3AProfiler::GetCounterProfiler()->OnCreateMesh();

	m_header.m_nIdent		= 'S3MD';
	m_header.m_nVersion 	= EXP_MESH_VER_CURRENT;
	m_header.m_strMaxFile	= "Unknown";
	m_header.m_nSkelGUID.Clear();
	m_header.m_nBoneNum		= 0;
	m_header.m_nIndexNum	= 0;
	m_header.m_nVertexNum	= 0;
}

S3AMeshData::~S3AMeshData()
{
	Clear();

	//S3AProfiler::GetCounterProfiler()->OnDeleteMesh();
}

void S3AMeshData::Clear()
{
	S3APointerArrayClear(m_arrMeshLOD);
}


void				S3AMeshData::Release()
{
	delete this;
}


S3AGUID				S3AMeshData::GetGlobalSkeletonID() const
{
	return m_header.m_nSkelGUID;
}


xmUint				S3AMeshData::GetIndexNumber() const
{
	return m_arrIndexBuffer.size() / (GetIsIndex32() ? sizeof(xmUint32) : sizeof(xmUint16));
}


bool				S3AMeshData::GetIsIndex32() const
{
	return m_nIndexType == EXP_INDEX_TYPE_U32;
}


const void*			S3AMeshData::GetIndexBuffer() const
{
	return m_arrIndexBuffer.size() > 0
		? &m_arrIndexBuffer[0]
	: 0;
}

xmUint				S3AMeshData::GetVertexSize() const
{
	xmUint nSize = sizeof(xmUint8);
	if (m_nVertexType == EXP_VT_SKIN_VERTEX_0)
	{
		nSize = sizeof(S3ASkinVertexType0);
	}
	return nSize;
}

xmUint				S3AMeshData::GetVertexStride() const
{
	return sizeof(S3ASkinVertexType0);
}


xmUint				S3AMeshData::GetBoneIndexOffset() const
{
	S3ASkinVertexType0* pData = NULL;
	xmUint nOffset = (xmUint8*)(&pData->m_nBoneIndice) - (xmUint8*)(pData);
	return nOffset;
}

xmUint				S3AMeshData::GetBoneWeightOffset() const
{
	S3ASkinVertexType0* pData = NULL;
	xmUint nOffset = (xmUint8*)(&pData->m_nBoneWeight) - (xmUint8*)(pData);
	return nOffset;
}

xmUint						S3AMeshData::GetVertexNumber() const
{
	return m_arrVertexBuffer.size() / GetVertexSize();
}

S3ASkinVertexType0*	S3AMeshData::GetVertexBufferSkinVertexType0()
{
	xmUint8* pData = m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
	: NULL;

	return EXP_VT_SKIN_VERTEX_0 == m_nVertexType 
		? reinterpret_cast<S3ASkinVertexType0*>(pData)
		: NULL;
}

const S3ASkinVertexType0*	S3AMeshData::GetVertexBufferSkinVertexType0() const
{
	const xmUint8* pData = m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
		: NULL;

	return EXP_VT_SKIN_VERTEX_0 == m_nVertexType 
		? reinterpret_cast<const S3ASkinVertexType0*>(pData)
		: NULL;
}


void S3AMeshData::RemoveDegenerateTriangleInplace(void)
{
	std::vector<unsigned int> subIndexMove;
	RemoveDegenerateTriangleInplace(m_arrIndexBuffer, m_nIndexType, subIndexMove);
	if (subIndexMove.size())
	{
		for (std::vector<S3AMeshLODData*>::iterator itr = m_arrMeshLOD.begin(); itr != m_arrMeshLOD.end(); ++itr)
		{
			S3AMeshLODData *pLODData = *itr;
			DegenerateSubMeshData(pLODData, subIndexMove);
		}
	}
	else
	{
		for (std::vector<S3AMeshLODData*>::iterator itr = m_arrMeshLOD.begin(); itr != m_arrMeshLOD.end(); ++itr)
		{
			S3AMeshLODData *pLODData = *itr;
			RemoveDegenerateTriangleInplace(pLODData->m_arrIndexBuffer, pLODData->m_nIndexType, subIndexMove);
			DegenerateSubMeshData(pLODData, subIndexMove);
		}
	}
}

void S3AMeshData::DegenerateSubMeshData( S3AMeshLODData * pLODData, std::vector<unsigned int> &subIndexMove ) 
{
	for (int i = 0; i < (int)pLODData->GetSubMeshNumber(); ++i)
	{
		IS3ASubMeshData *pSubMeshData = pLODData->GetSubMesh(i);
		for (int j = 0; j < (int)subIndexMove.size(); ++j)
		{
			xmUint indexStart = pSubMeshData->GetIndexStart();
			if (indexStart > subIndexMove[j])
			{
				pSubMeshData->SetIndexStart(indexStart - 3);
			}
			else if (indexStart + pSubMeshData->GetIndexCount() > subIndexMove[j])
			{
				pSubMeshData->SetIndexCount(pSubMeshData->GetIndexCount() - 3);
			}
		}
	}
}

void S3AMeshData::RemoveDegenerateTriangleInplace(std::vector<xmUint8> &arrIndexBuffer, xmUint nIndexType, std::vector<unsigned int> &subIndexMove)
{
	if (arrIndexBuffer.size() == 0)
		return;

	std::vector<xmUint8> arrTmpIndexBuffer;
	arrTmpIndexBuffer.resize(arrIndexBuffer.size());

	if (nIndexType == EXP_INDEX_TYPE_U16)
	{
		int nOffset = 2;
		int nSize = arrIndexBuffer.size() / nOffset / 3;
		xmUint16 *pCur = (xmUint16 *)&*arrIndexBuffer.begin();
		xmUint16 *pTmp = (xmUint16 *)&*arrTmpIndexBuffer.begin();

		for (int i = 0; i < nSize; ++i)
		{
			if (pCur[0] == pCur[1] || pCur[1] == pCur[2] || pCur[2] == pCur[0])
			{
				pCur += 3;
				subIndexMove.push_back(pTmp - (xmUint16 *)&*arrTmpIndexBuffer.begin());
				continue;
			}

			memcpy(pTmp, pCur, nOffset * 3);
			pTmp += 3;
			pCur += 3;
		}

		arrTmpIndexBuffer.resize((xmUint8*)pTmp - &*arrTmpIndexBuffer.begin());
	}
	else if (nIndexType == EXP_INDEX_TYPE_U32)
	{
		int nOffset = 4;
		int nSize = arrIndexBuffer.size() / nOffset / 3;
		xmUint32 *pCur = (xmUint32 *)&*arrIndexBuffer.begin();
		xmUint32 *pTmp = (xmUint32 *)&*arrTmpIndexBuffer.begin();

		for (int i = 0; i < nSize; ++i)
		{
			if (pCur[0] == pCur[1] || pCur[1] == pCur[2] || pCur[2] == pCur[0])
			{
				pCur += 3;
				subIndexMove.push_back(pTmp - (xmUint32 *)&*arrTmpIndexBuffer.begin());
				continue;
			}

			memcpy(pTmp, pCur, nOffset * 3);
			pTmp += 3;
			pCur += 3;
		}

		arrTmpIndexBuffer.resize((xmUint8*)pTmp - &*arrTmpIndexBuffer.begin());
	}

	arrTmpIndexBuffer.swap(arrIndexBuffer);
}

void				S3AMeshData::GetBoundingAABB(xmVector3* vMin,xmVector3* vMax)
{
	m_meshBound.GetAABB(*vMin,*vMax);
}



xmUint				S3AMeshData::GetBoneNumber() const
{
	return m_arrBoneName.size();
}

const char*			S3AMeshData::GetBoneName(xmUint nBoneInd) const
{
	return m_arrBoneName[nBoneInd].c_str();
}

//------------------------------------------------------------------------------
//For LOD Mesh
//------------------------------------------------------------------------------
xmUint                S3AMeshData::GetLODMeshNumber() const
{
	return m_arrMeshLOD.size();
}

IS3AMeshLODData      *S3AMeshData::GetLODMesh(int nLOD)
{
	if (0 <= nLOD && nLOD < (int)m_arrMeshLOD.size())
		return m_arrMeshLOD[nLOD];
	else
		return NULL;
}

const IS3AMeshLODData *S3AMeshData::GetLODMesh(int nLOD) const
{
	if (0 <= nLOD && nLOD < (int)m_arrMeshLOD.size())
		return m_arrMeshLOD[nLOD];
	else
		return NULL;
}

int                 S3AMeshData::GetBoneLOD(int nMeshLOD) const
{
	return GetLODMesh(nMeshLOD)->GetBoneLOD();
}

void                 S3AMeshData::SetBoneLOD(int nMeshLOD, int nBoneLOD)
{
	GetLODMesh(nMeshLOD)->SetBoneLOD(nBoneLOD);// m_nBoneLOD = nBoneLOD;
}

IS3AMeshLODData*		S3AMeshData::IncLOD(/*IS3AMeshLODData* pMeshLodData*/)
{
	S3AMeshLODData *pMeshLODData = new S3AMeshLODData;
	m_arrMeshLOD.push_back(pMeshLODData);
	return pMeshLODData;

// 	if (pMeshLodData == NULL)
// 		return;
// 
// 	m_arrMeshLOD.push_back(pMeshLodData);
// 	S3AMeshLODData *pMeshLODData = new S3AMeshLODData;
// 	S3AMeshDataHelperDX::CopyIndexBufferRev(pSourceD3DMesh, pMeshLODData, GetLODMesh(0));
// 	m_arrMeshLOD.push_back(pMeshLODData);
// 	return pMeshLODData;
}

void                S3AMeshData::DecLOD(void)
{
	S3AMeshLODData *pMeshLODData = *m_arrMeshLOD.rbegin();
	delete pMeshLODData;
	m_arrMeshLOD.pop_back();
}

void				S3AMeshData::SetLodMeshVersion()
{
	GetExpMeshHeader().m_nVersion = EXP_MESH_VER_LOD_MESH_DATA;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
xmUint				S3AMeshData::GetSubMeshNumber() const
{
	return GetLODMesh(0)->GetSubMeshNumber();
}


const IS3ASubMeshData*	S3AMeshData::GetSubMesh(xmUint nSubMeshInd, int nLOD) const
{
	const IS3AMeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;
	else
		return pLODMesh->GetSubMesh(nSubMeshInd);
}


void					S3AMeshData::ResetBuffer(xmUint nIndexType,xmUint32 nIndexNum
									,xmUint32 nVertexNum
									,xmUint32 nSubMeshNum
									,xmUint32 nBoneNum
									)
{
	m_nIndexType = nIndexType;
	m_nVertexType = EXP_VT_SKIN_VERTEX_0;


	const xmUint nIndexStride = nIndexType == EXP_INDEX_TYPE_U16 ? sizeof(xmUint16) : sizeof(xmUint32);

	m_arrIndexBuffer.resize(nIndexNum * nIndexStride);
	
	m_arrVertexBuffer.resize(nVertexNum * sizeof(S3ASkinVertexType0));

	S3APointerArrayResize(m_arrMeshLOD,1);

	m_arrMeshLOD[0]->ResizeSubMesh(nSubMeshNum);


	m_meshBound.SetIdentity();

	ResetBone(nBoneNum);

	m_header.m_nIndexNum = nIndexNum;
	m_header.m_nVertexNum = nVertexNum;
		


}


void					S3AMeshData::SetGlobalSkeletonID(S3AGUID nSkelGUID)
{
	m_header.m_nSkelGUID = nSkelGUID;
}



void*					S3AMeshData::GetIndexBuffer()
{
	return m_arrIndexBuffer.size() > 0
		? &m_arrIndexBuffer[0]
	: 0;
}


S3ASkinVertexType0*		S3AMeshData::GetVertexBuffer()
{
	return reinterpret_cast<S3ASkinVertexType0*>(m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
	: 0);
}
	



IS3ASubMeshData*		S3AMeshData::GetSubMesh(xmUint nSubMeshInd, int nLOD)
{
	IS3AMeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;

	return pLODMesh->GetSubMesh(nSubMeshInd);
}


IS3ASubMeshData*		S3AMeshData::GetSubMeshByName(const char* pszName, int nLOD)
{
	IS3AMeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;

	const xmUint nSubMeshNum = GetSubMeshNumber();
	for (xmUint32 nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
	{
		IS3ASubMeshData* pSubMesh = pLODMesh->GetSubMesh(nSubMeshCnt);
		if (_stricmp(pSubMesh->GetName(),pszName) == 0)
		{
			return pSubMesh;
		}
	}
	return NULL;
	
}



void					S3AMeshData::ResetBone(xmUint32 nBoneNum)
{

	S3AExpBounding boundIden;
	boundIden.SetIdentity();
	
	m_arrBoneBound.resize(nBoneNum,boundIden);
	m_arrBoneName.resize(nBoneNum);

	m_header.m_nBoneNum = nBoneNum;
}


void					S3AMeshData::SetBoneName(S3ABoneIndex nBoneInd,const char* pszBoneName)
{
	m_arrBoneName[nBoneInd] = pszBoneName;
}

void					S3AMeshData::SetBoneBoundOBB(S3ABoneIndex nBoneInd
	,const xmVector3* pPos,const xmQuaternion* pRot
	,float fXSize,float fYSize,float fZSize)
{
	m_arrBoneBound[nBoneInd].SetOBB(pPos,pRot,fXSize,fYSize,fZSize);
}

void					S3AMeshData::SetBoundingAABB(const xmVector3* vMin,const xmVector3* vMax)
{
	m_meshBound.SetAABB(*vMin,*vMax);
}

void					S3AMeshData::Serialize(IS3ASerializeListener* pSL,const char* pszLabel)
{
	pSL->BeginSection(pszLabel);

	S3ASerialize(*pSL,m_header,"MeshHeader");

	pSL->PushVersion(m_header.m_nVersion);
	S3ASerialize(*pSL,m_nIndexType,"IndexType");
	S3ASerialize(*pSL,m_nVertexType,"VertexType");

	if (EXP_INDEX_TYPE_U16 == m_nIndexType)
	{
		S3ASerializeRawData<S3ABoneIndex>(*pSL,m_arrIndexBuffer,"IndexBuffer");
	}else if (EXP_INDEX_TYPE_U32 == m_nIndexType)
	{
		S3ASerializeRawData<xmUint32>(*pSL,m_arrIndexBuffer,"IndexBuffer");
	}else{
		S3ALogError(S3A_ERR_INVALID_CALL,"Fail to serialize mesh data : invalid index type");
	}

	if (EXP_VT_SKIN_VERTEX_0 == m_nVertexType)
	{
		S3ASerializeRawData<S3AExpVertexType0>(*pSL,m_arrVertexBuffer,"VertexBuffer");
	}

	S3ASerialize(*pSL,m_meshBound,"MeshBound");
	S3ASerialize(*pSL,m_arrMeshLOD,"MeshLOD");
	S3ASerialize(*pSL,m_arrBoneName,"BoneName");
	S3ASerialize(*pSL,m_arrBoneBound,"BoundBound");

	pSL->PopVersion();

	pSL->EndSection();
}


void					S3AMeshData::SetSource(const char* pszSource)
{
	m_header.m_strMaxFile = pszSource;
}

const char*				S3AMeshData::GetSource() const
{
	return m_header.m_strMaxFile.c_str();
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------



void S3AExpBounding::SetInvalid()
{
	m_nShapeType = EXP_BS_UNKNOWN;
}

void S3AExpBounding::SetIdentity()
{
	xmVector3 vMin(-0.5f,-0.5f,-0.5f);
	xmVector3 vMax(0.5f,0.5f,0.5f);
	SetAABB(vMin,vMax);
}

void S3AExpBounding::SetAABB(const xmVector3& vMin,const xmVector3& vMax)
{
	m_nShapeType = EXP_BS_BOX;
	m_vPos = 0.5f*(vMin + vMax);
	//xmQuaternionIdentity(&m_qRot);
	D3DXQuaternionIdentity(&m_qRot);
	m_boxShape.m_fXSize = vMax.x - vMin.x;
	m_boxShape.m_fYSize = vMax.y - vMin.y;
	m_boxShape.m_fZSize = vMax.z - vMin.z;
}

void S3AExpBounding::SetOBB(const xmVector3* pPos,const xmQuaternion* pRot
			,float fXSize,float fYSize,float fZSize)
{
	m_nShapeType = EXP_BS_BOX;
	//m_vPos = pPos ? *pPos : xmVec3Zero();
	//m_qRot = pRot ? *pRot : xmQuaternionIden();
	m_boxShape.m_fXSize = fXSize;
	m_boxShape.m_fYSize = fYSize;
	m_boxShape.m_fZSize = fZSize;

}

void S3AExpBounding::GetAABB(xmVector3& vMin,xmVector3& vMax) const
{

	if (m_nShapeType == EXP_BS_BOX)
	{
		xmVector3 vSize(m_boxShape.m_fXSize,m_boxShape.m_fYSize,m_boxShape.m_fZSize);
		vSize = vSize * 0.5f;
		vMin = m_vPos - vSize;
		vMax = m_vPos + vSize;
	}else{
		//S3ALogError(0,"Fail to get AABB");
		vMin = m_vPos;
		vMax = m_vPos;
	}
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpVertexType0 &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	S3ASerialize(sl,val.p,"Position");
	S3ASerialize(sl,val.b,"BoneIndice");
	S3ASerialize(sl,val.w,"BoneWeight");
	S3ASerialize(sl,val.n,"Normal");
	S3ASerialize(sl,val.uv,"UV");
	S3ASerialize(sl,val.tgn,"Tangent");
	S3ASerialize(sl,val.vc,"VertexColor");
	S3ASerialize(sl,val.vi,"VertexIllumniation");

	sl.EndSection();

}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpBoxShape &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.m_fXSize,"XSize");
	S3ASerialize(sl,val.m_fYSize,"YSize");
	S3ASerialize(sl,val.m_fZSize,"ZSize");
	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpCylinderShape &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.m_fRadius,"Radius");
	S3ASerialize(sl,val.m_fHeight,"Height");
	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpBounding &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.m_nShapeType,"ShapeType");
	S3ASerialize(sl,val.m_vPos,"Position");
	S3ASerialize(sl,val.m_qRot,"Rotation");
	if (EXP_BS_BOX == val.m_nShapeType)
	{
		S3ASerialize(sl,val.m_boxShape,"BoxShape");
	}else if (EXP_BS_CYLINDER == val.m_nShapeType)
	{
		S3ASerialize(sl,val.m_cylinderShape,"CylinderShape");
	}else{
		S3ASSERT(false && "unknown bounding shape type");
	}

	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpSubMesh &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	S3ASerialize(sl,val.m_nIndexStart,"IndexStart");
	S3ASerialize(sl,val.m_nIndexCount,"IndexCount");
	S3ASerialize(sl,val.m_nVertexStart,"VertexStart");
	S3ASerialize(sl,val.m_nVertexCount,"VertexCount");
	S3ASerialize(sl,val.m_subMeshBound,"SubMeshBound");
	S3ASerialize(sl,val.m_name,"SubMeshName");
	S3ASerialize(sl,val.m_submeshTag,"SubMeshTag");
	S3ASerialize(sl,val.m_arrBonePalette,"BonePalette");


	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshLOD &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	S3ASerialize(sl,val.m_arrSubMesh,"SubMesh");
	sl.EndSection();
}




void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshData &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	S3ASerialize(sl,val.m_nIndexType,"IndexType");
	S3ASerialize(sl,val.m_nVertexType,"VertexType");

	if (EXP_INDEX_TYPE_U16 == val.m_nIndexType)
	{
		S3ASerializeRawData<S3ABoneIndex>(sl,val.m_arrIndexBuffer,"IndexBuffer");
	}else if (EXP_INDEX_TYPE_U32 == val.m_nIndexType)
	{
		S3ASerializeRawData<xmUint32>(sl,val.m_arrIndexBuffer,"IndexBuffer");
	}

	if (EXP_VT_SKIN_VERTEX_0 == val.m_nVertexType)
	{
		S3ASerializeRawData<S3AExpVertexType0>(sl,val.m_arrVertexBuffer,"VertexBuffer");
	}

	S3ASerialize(sl,val.m_arrBoneName,"BoneName");

	S3ASerialize(sl,val.m_arrMeshLOD,"MeshLOD");

	S3ASerialize(sl,val.m_meshBound,"MeshBound");

	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshHeader &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	S3ASerialize(sl,val.m_nIdent,"FileIden");
	S3ASerialize(sl,val.m_nVersion,"Version");
	S3ASerialize(sl,val.m_strMaxFile,"SourceFile");
	if (val.m_nVersion <= EXP_MESH_VER_EMBED_MESH_DATA)
	{
		S3ASerialize(sl,val.m_nSkelGUID.m_a,"SkeletonGUID");
		val.m_nSkelGUID.m_b = 0;
	}else{
		S3ASerialize(sl,val.m_nSkelGUID,"SkeletonGUID");
	}
	
	S3ASerialize(sl,val.m_nBoneNum,"BoneNumber");
	S3ASerialize(sl,val.m_nIndexNum,"IndexNumber");
	S3ASerialize(sl,val.m_nVertexNum,"VertexNumber");

	sl.EndSection();
}

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMesh &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	S3ASerialize(sl,val.m_header,"MeshHeader");

	sl.PushVersion(val.m_header.m_nVersion);
	S3ASerialize(sl,val.m_mesh,"MeshData");
	sl.PopVersion();

	sl.EndSection();
}

