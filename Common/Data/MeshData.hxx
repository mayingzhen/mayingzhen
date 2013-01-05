#include "Common/Data/MeshData.h"


namespace ma
{



void VertexType0::Serialize(SerializeListener& sl,const char* pszLabel)
{
	sl.BeginSection(pszLabel);

	sl.Serialize(p,"Position");
	sl.Serialize(b,"BoneIndice");
	sl.Serialize(w,"BoneWeight");
	sl.Serialize(n,"Normal");
	sl.Serialize(uv,"UV");
	sl.Serialize(tgn,"Tangent");
	sl.Serialize(vc,"VertexColor");
	sl.Serialize(vi,"VertexIllumniation");

	sl.EndSection();

}

void BoxShape::Serialize(SerializeListener& sl,const char* pszLabel )
{
	sl.BeginSection(pszLabel);
	sl.Serialize(m_fXSize,"XSize");
	sl.Serialize(m_fYSize,"YSize");
	sl.Serialize(m_fZSize,"ZSize");
	sl.EndSection();
}

void AABBShape::Init()
{
	m_vMin = D3DXVECTOR3(xm_FMAX,xm_FMAX,xm_FMAX);
	m_vMax = D3DXVECTOR3(-xm_FMAX,-xm_FMAX,-xm_FMAX);
}

void AABBShape::Merge(const D3DXVECTOR3& vMin,const D3DXVECTOR3& vMax)
{
	xmVec3Min(&m_vMin,&vMin,&m_vMin);
	xmVec3Max(&m_vMax,&vMax,&m_vMax);
}

void AABBShape::AddPoint(const D3DXVECTOR3& v)
{
	xmVec3Min(&m_vMin,&v,&m_vMin);
	xmVec3Max(&m_vMax,&v,&m_vMax);
}

void CylinderShape::Serialize(SerializeListener& sl, const char* pszLable )
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_fRadius,"Radius");
	sl.Serialize(m_fHeight,"Height");
	sl.EndSection();
}

void Bounding::Serialize(SerializeListener& sl, const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_nShapeType,"ShapeType");
	sl.Serialize(m_vPos,"Position");
	sl.Serialize(m_qRot,"Rotation");
	if (BS_BOX == m_nShapeType)
	{
		sl.Serialize(m_boxShape,"BoxShape");
	}
	else if (BS_CYLINDER == m_nShapeType)
	{
		sl.Serialize(m_cylinderShape,"CylinderShape");
	}
	else{
		SSERT(false && "unknown bounding shape type");
	}

	sl.EndSection();
}


void MeshHeader::Serialize(SerializeListener& sl,const char* pszLable)
{
	sl.BeginSection(pszLable);

	sl.Serialize(m_nIdent,"FileIden");
	sl.Serialize(m_nVersion,"Version");
	sl.Serialize(m_strMaxFile,"SourceFile");
	if (m_nVersion <= MESH_VER_EMBED_MESH_DATA)
	{
		sl.Serialize(m_nSkelGUID.m_a,"SkeletonGUID");
		m_nSkelGUID.m_b = 0;
	}else{
		sl.Serialize(m_nSkelGUID,"SkeletonGUID");
	}

	sl.Serialize(m_nBoneNum,"BoneNumber");
	sl.Serialize(m_nIndexNum,"IndexNumber");
	sl.Serialize(m_nVertexNum,"VertexNumber");

	sl.EndSection();
}



/*!
param val
Null element is not allowed in array.
T must be a simple struct or class which do not has derived class.
*/
template<class T>
void Serialize(SerializeListener& sl,std::vector<T*>& val,const char* pszLable = "array")
{
	sl.BeginSection(pszLable);

	xmUint nSize = (xmUint)val.size();
	sl.Serialize(nSize,"size");

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

		sl.Serialize(*val[nCnt],buf);
	}
	sl.EndSection();

	sl.EndSection();
}

template<class T>
void PointerArrayClear(T& arr)
{
	for (xmUint nCnt = 0; nCnt < (xmUint)arr.size(); ++nCnt)
	{
		xmSafeDelete(arr[nCnt]);
	}
	arr.clear();
};


template<class ElementType >
void PointerArrayResize(std::vector<ElementType*>& arr,xmUint nNewSize)
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
void SerializeRawData(SerializeListener& sl,std::vector<xmUint8>& val,const char* pszLable)
{
	sl.BeginSection(pszLable);

	xmUint nSize = val.size();
	sl.Serialize(nSize,"size");

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
		sl.Serialize((DataType&)val[nCnt*sizeof(DataType)],buf);
	}
	sl.EndSection();

	sl.EndSection();
}

SubMeshData::SubMeshData()
:m_nIndexStart(0)
,m_nIndexCount(0)
,m_nVertexStart(0)
,m_nVertexCount(0)
,m_nMateiralID(0)
{
	
}

void	SubMeshData::Reset(xmUint nIndexStart,xmUint nIndexCount
			  ,xmUint nVertexStart,xmUint nVertexCount
			  ,xmUint nMtlID
			  ,const Bounding* pBounding
			  ,const BoneIndex* arrBonePalatte
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
	}
	else
	{
		m_subMeshBound.SetIdentity();
	}
	m_arrBonePalette.resize(nBonePalatteSize);

	if (arrBonePalatte)
	{
		for (xmUint nPalCnt = 0; nPalCnt < nBonePalatteSize; ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = arrBonePalatte[nPalCnt];
		}
	}
	else
	{
		for (xmUint nPalCnt = 0; nPalCnt < nBonePalatteSize; ++nPalCnt)
		{
			m_arrBonePalette[nPalCnt] = InvalidID<BoneIndex>();
		}
	}

	m_name = pszName;
	m_submeshTag = pszTag;
}


const char* SubMeshData::GetName() const
{
	return m_name.c_str();
}


const char*	SubMeshData::GetTag() const
{
	return m_submeshTag.c_str();
}


xmUint	SubMeshData::GetIndexStart() const
{
	return m_nIndexStart;
}


xmUint	SubMeshData::GetIndexCount() const
{
	return m_nIndexCount;
}


xmUint	SubMeshData::GetVertexStart() const
{
	return m_nVertexStart;
}


xmUint	SubMeshData::GetVertexCount() const
{
	return m_nVertexCount;
}

xmUint	SubMeshData::GetMaterialID() const
{
	return m_nMateiralID;
}

BoneIndex	SubMeshData::GetBonePaletteSize() const
{
	return m_arrBonePalette.size();
}

const BoneIndex*	SubMeshData::GetBonePalette() const
{
	return m_arrBonePalette.size() > 0 ? &m_arrBonePalette[0] : NULL;
}


//------------------------------------------------------------------------------
//For Modification
//------------------------------------------------------------------------------
void	SubMeshData::SetName(const char* pszName)
{
	m_name = pszName;
}


void	SubMeshData::SetTag(const char* pszTag)
{
	m_submeshTag = pszTag;
}



void	SubMeshData::SetIndexStart(xmUint32 nIndStart)
{
	m_nIndexStart = nIndStart;
}


void	SubMeshData::SetIndexCount(xmUint32 nIndCnt)
{
	m_nIndexCount = nIndCnt;
}


void	SubMeshData::SetVertexStart(xmUint32 nVStart)
{
	m_nVertexStart = nVStart;
}


void	SubMeshData::SetVertexCount(xmUint32 nVCnt)
{
	m_nVertexCount = nVCnt;
}

void	SubMeshData::SetMaterialID(xmUint nMtlID)
{
	m_nMateiralID = nMtlID;
}

/*!\brief
\param arrBoneIndice array of bone indices, if null, default bone will be filled
*/
void	SubMeshData::ResetBonePalatte(xmUint32 nPalatteSize,xmUint16* arrBoneIndice)
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


void	SubMeshData::SetBoneByPalatteIndex(xmUint16 nPalatteInd,xmUint16 nBoneInd)
{
	m_arrBonePalette[nPalatteInd] = nBoneInd;
}


xmUint16	SubMeshData::GetBoneByPalatteIndex(xmUint16 nPalatteInd)
{
	return m_arrBonePalette[nPalatteInd];
}


void	SubMeshData::Serialize(SerializeListener& sl,const char* pszLabel)
{
	sl.BeginSection(pszLabel);

	sl.Serialize(m_nIndexStart,"IndexStart");
	sl.Serialize(m_nIndexCount,"IndexCount");
	sl.Serialize(m_nVertexStart,"VertexStart");
	sl.Serialize(m_nVertexCount,"VertexCount");
	sl.Serialize(m_subMeshBound,"SubMeshBound");
	sl.Serialize(m_arrBonePalette,"BonePalette");
	sl.Serialize(m_name,"SubMeshName");
	sl.Serialize(m_submeshTag,"SubMeshTag");
    if (sl.GetVersion() >= MESH_VER_EMBED_MESH_DATA)
    {
		sl.Serialize(m_nMateiralID,"MaterialID");
    }

	sl.EndSection();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MeshLODData::MeshLODData()
: m_nMeshDataFlag(DATA_PARENT_DATA)
, m_nBoneLOD(0)
,m_nVertexType(VT_SKIN_VERTEX_0)
{
}

MeshLODData::~MeshLODData()
{
	ClearSubMesh();
}


// const VertexType0*	MeshLODData::GetVertexBufferVertexType0() const
// {
// 	const xmUint8* pData = m_arrVertexBuffer.size() > 0
// 		? &m_arrVertexBuffer[0]
// 	: NULL;
// 
// 	return _VT_SKIN_VERTEX_0 == m_nVertexType 
// 		? reinterpret_cast<const VertexType0*>(pData)
// 		: NULL;
// }

VertexType0*	MeshLODData::GetVertexBufferVertexType0()
{
	xmUint8* pData = m_arrVertexBuffer.size() > 0 ? &m_arrVertexBuffer[0] : NULL;

	return VT_SKIN_VERTEX_0 == m_nVertexType ? reinterpret_cast<VertexType0*>(pData) : NULL;
}

const SubMeshData*	MeshLODData::GetSubMesh(xmUint nSubMeshInd) const
{
	return m_arrSubMesh[nSubMeshInd];
}


void					MeshLODData::ResizeSubMesh(xmUint nSubMeshNum)
{
	PointerArrayResize(m_arrSubMesh,nSubMeshNum);
}

SubMeshData*			MeshLODData::CreateSubMesh()
{
	SubMeshData* pSubMesh = new SubMeshData;
	m_arrSubMesh.push_back(pSubMesh);
	return pSubMesh;
}

xmUint					MeshLODData::GetSubMeshNumber() const
{
	return m_arrSubMesh.size();
}

SubMeshData*			MeshLODData::GetSubMesh(xmUint nSubMeshInd)
{
	return m_arrSubMesh[nSubMeshInd];
}

int						MeshLODData::GetBoneLOD() const
{
	return m_nBoneLOD;
}

void					MeshLODData::SetBoneLOD(int nBoneLOD)
{
	m_nBoneLOD = nBoneLOD;
}

xmUint					MeshLODData::GetMeshDataFlag() const
{
	return m_nMeshDataFlag;
}

void					MeshLODData::SetMeshDataFlag(xmUint uMeshDataFlag)
{
	m_nMeshDataFlag = uMeshDataFlag;
}

xmUint					MeshLODData::GetIndexNumber() const
{
	return m_arrIndexBuffer.size() / (GetIsIndex32() ? sizeof(xmUint32) : sizeof(xmUint16));
}

bool					MeshLODData::GetIsIndex32() const
{
	return m_nIndexType == INDEX_TYPE_U32;
}

int						MeshLODData::GetVertexNumber() const
{
	return m_arrVertexBuffer.size() / sizeof(VertexType0);
}

void					MeshLODData::ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum)
{
	m_nIndexType = bIsIndex32 ? INDEX_TYPE_U32 : INDEX_TYPE_U16;
	m_nVertexType = VT_SKIN_VERTEX_0;

	const xmUint nIndexStride = m_nIndexType == INDEX_TYPE_U16 ? sizeof(xmUint16) : sizeof(xmUint32);
	const xmUint nVertexStride = sizeof(VertexType0);

	m_arrIndexBuffer.resize(nIndexNum * nIndexStride);
	m_arrVertexBuffer.resize(nVertexNum * nVertexStride);
}

void					MeshLODData::ClearSubMesh()
{
	for (xmUint nSubMeshCnt = 0; nSubMeshCnt < (xmUint)m_arrSubMesh.size(); ++nSubMeshCnt)
	{
		xmSafeDelete(m_arrSubMesh[nSubMeshCnt]);
	}
	m_arrSubMesh.clear();
}

void	MeshLODData::Serialize(SerializeListener& sl,const char* pszLabel)
{
	sl.BeginSection(pszLabel);
	if(sl.GetVersion() >= MESH_VER_LOD_MESH_DATA)
	{
		sl.Serialize( m_nMeshDataFlag, "MeshDataFrom");
	}

	if (m_nMeshDataFlag & DATA_HAS_OWN_INDEX)
	{
		sl.Serialize(m_nIndexType,"IndexType");

		if (INDEX_TYPE_U16 == m_nIndexType)
		{
			sl.SerializeRawData<BoneIndex>(m_arrIndexBuffer,"IndexBuffer");
		}
		else if (INDEX_TYPE_U32 == m_nIndexType)
		{
			sl.SerializeRawData<xmUint32>(m_arrIndexBuffer,"IndexBuffer");
		}
		else
		{
			LogError(_ERR_INVALID_CALL,"Fail to serialize mesh data : invalid index type");
		}
	}

	if (m_nMeshDataFlag & DATA_HAS_OWN_VERTEX)
	{
		sl.Serialize(m_nVertexType,"VertexType");

		if (VT_SKIN_VERTEX_0 == m_nVertexType)
		{
			sl.SerializeRawData<VertexType0>(m_arrVertexBuffer,"VertexBuffer");
		}
	}

	if (m_nMeshDataFlag & DATA_HAS_BONE_LOD)
	{
		sl.Serialize( m_nBoneLOD, "BoneLOD");
	}

	sl.Serialize(m_arrSubMesh,"SubMeshArray");

	sl.EndSection();
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
MeshData::MeshData()
:m_nIndexType(INDEX_TYPE_U16)
,m_nVertexType(VT_SKIN_VERTEX_0)
{
	//Profiler::GetCounterProfiler()->OnCreateMesh();

	m_header.m_nIdent		= 'S3MD';
	m_header.m_nVersion 	= MESH_VER_CURRENT;
	m_header.m_strMaxFile	= "Unknown";
	m_header.m_nSkelGUID.Clear();
	m_header.m_nBoneNum		= 0;
	m_header.m_nIndexNum	= 0;
	m_header.m_nVertexNum	= 0;
}

MeshData::~MeshData()
{
	Clear();

	//Profiler::GetCounterProfiler()->OnDeleteMesh();
}

void MeshData::Clear()
{
	PointerArrayClear(m_arrMeshLOD);
}


void				MeshData::Release()
{
	delete this;
}


GUID				MeshData::GetGlobalSkeletonID() const
{
	return m_header.m_nSkelGUID;
}


xmUint				MeshData::GetIndexNumber() const
{
	return m_arrIndexBuffer.size() / (GetIsIndex32() ? sizeof(xmUint32) : sizeof(xmUint16));
}


bool				MeshData::GetIsIndex32() const
{
	return m_nIndexType == INDEX_TYPE_U32;
}


const void*			MeshData::GetIndexBuffer() const
{
	return m_arrIndexBuffer.size() > 0
		? &m_arrIndexBuffer[0]
	: 0;
}

xmUint				MeshData::GetVertexSize() const
{
	xmUint nSize = sizeof(xmUint8);
	if (m_nVertexType == VT_SKIN_VERTEX_0)
	{
		nSize = sizeof(VertexType0);
	}
	return nSize;
}

xmUint				MeshData::GetVertexStride() const
{
	return sizeof(VertexType0);
}


xmUint				MeshData::GetBoneIndexOffset() const
{
	VertexType0* pData = NULL;
	xmUint nOffset = (xmUint8*)(&pData->b) - (xmUint8*)(pData);
	return nOffset;
}

xmUint				MeshData::GetBoneWeightOffset() const
{
	VertexType0* pData = NULL;
	xmUint nOffset = (xmUint8*)(&pData->w) - (xmUint8*)(pData);
	return nOffset;
}

xmUint						MeshData::GetVertexNumber() const
{
	return m_arrVertexBuffer.size() / GetVertexSize();
}

VertexType0*	MeshData::GetVertexBufferVertexType0()
{
	xmUint8* pData = m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
	: NULL;

	return VT_SKIN_VERTEX_0 == m_nVertexType 
		? reinterpret_cast<VertexType0*>(pData)
		: NULL;
}

const VertexType0*	MeshData::GetVertexBufferVertexType0() const
{
	const xmUint8* pData = m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
		: NULL;

	return VT_SKIN_VERTEX_0 == m_nVertexType 
		? reinterpret_cast<const VertexType0*>(pData)
		: NULL;
}


void MeshData::RemoveDegenerateTriangleInplace(void)
{
	std::vector<unsigned int> subIndexMove;
	RemoveDegenerateTriangleInplace(m_arrIndexBuffer, m_nIndexType, subIndexMove);
	if (subIndexMove.size())
	{
		for (std::vector<MeshLODData*>::iterator itr = m_arrMeshLOD.begin(); itr != m_arrMeshLOD.end(); ++itr)
		{
			MeshLODData *pLODData = *itr;
			DegenerateSubMeshData(pLODData, subIndexMove);
		}
	}
	else
	{
		for (std::vector<MeshLODData*>::iterator itr = m_arrMeshLOD.begin(); itr != m_arrMeshLOD.end(); ++itr)
		{
			MeshLODData *pLODData = *itr;
			RemoveDegenerateTriangleInplace(pLODData->m_arrIndexBuffer, pLODData->m_nIndexType, subIndexMove);
			DegenerateSubMeshData(pLODData, subIndexMove);
		}
	}
}

void MeshData::DegenerateSubMeshData( MeshLODData * pLODData, std::vector<unsigned int> &subIndexMove ) 
{
	for (int i = 0; i < (int)pLODData->GetSubMeshNumber(); ++i)
	{
		SubMeshData *pSubMeshData = pLODData->GetSubMesh(i);
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

void MeshData::RemoveDegenerateTriangleInplace(std::vector<xmUint8> &arrIndexBuffer, xmUint nIndexType, std::vector<unsigned int> &subIndexMove)
{
	if (arrIndexBuffer.size() == 0)
		return;

	std::vector<xmUint8> arrTmpIndexBuffer;
	arrTmpIndexBuffer.resize(arrIndexBuffer.size());

	if (nIndexType == INDEX_TYPE_U16)
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
	else if (nIndexType == INDEX_TYPE_U32)
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

void				MeshData::GetBoundingAABB(D3DXVECTOR3* vMin,D3DXVECTOR3* vMax)
{
	m_meshBound.GetAABB(*vMin,*vMax);
}



xmUint				MeshData::GetBoneNumber() const
{
	return m_arrBoneName.size();
}

const char*			MeshData::GetBoneName(xmUint nBoneInd) const
{
	return m_arrBoneName[nBoneInd].c_str();
}

//------------------------------------------------------------------------------
//For LOD Mesh
//------------------------------------------------------------------------------
xmUint                MeshData::GetLODMeshNumber() const
{
	return m_arrMeshLOD.size();
}

MeshLODData      *MeshData::GetLODMesh(int nLOD)
{
	if (0 <= nLOD && nLOD < (int)m_arrMeshLOD.size())
		return m_arrMeshLOD[nLOD];
	else
		return NULL;
}

const MeshLODData *MeshData::GetLODMesh(int nLOD) const
{
	if (0 <= nLOD && nLOD < (int)m_arrMeshLOD.size())
		return m_arrMeshLOD[nLOD];
	else
		return NULL;
}

int                 MeshData::GetBoneLOD(int nMeshLOD) const
{
	return GetLODMesh(nMeshLOD)->GetBoneLOD();
}

void                 MeshData::SetBoneLOD(int nMeshLOD, int nBoneLOD)
{
	GetLODMesh(nMeshLOD)->SetBoneLOD(nBoneLOD);// m_nBoneLOD = nBoneLOD;
}

MeshLODData*		MeshData::IncLOD(/*IMeshLODData* pMeshLodData*/)
{
	MeshLODData *pMeshLODData = new MeshLODData;
	m_arrMeshLOD.push_back(pMeshLODData);
	return pMeshLODData;

// 	if (pMeshLodData == NULL)
// 		return;
// 
// 	m_arrMeshLOD.push_back(pMeshLodData);
// 	MeshLODData *pMeshLODData = new MeshLODData;
// 	MeshDataHelperDX::CopyIndexBufferRev(pSourceD3DMesh, pMeshLODData, GetLODMesh(0));
// 	m_arrMeshLOD.push_back(pMeshLODData);
// 	return pMeshLODData;
}

void                MeshData::DecLOD(void)
{
	MeshLODData *pMeshLODData = *m_arrMeshLOD.rbegin();
	delete pMeshLODData;
	m_arrMeshLOD.pop_back();
}

void				MeshData::SetLodMeshVersion()
{
	GetMeshHeader().m_nVersion = MESH_VER_LOD_MESH_DATA;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
xmUint				MeshData::GetSubMeshNumber() const
{
	return GetLODMesh(0)->GetSubMeshNumber();
}


const SubMeshData*	MeshData::GetSubMesh(xmUint nSubMeshInd, int nLOD) const
{
	const MeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;
	else
		return pLODMesh->GetSubMesh(nSubMeshInd);
}


void					MeshData::ResetBuffer(xmUint nIndexType,xmUint32 nIndexNum
									,xmUint32 nVertexNum
									,xmUint32 nSubMeshNum
									,xmUint32 nBoneNum
									)
{
	m_nIndexType = nIndexType;
	m_nVertexType = VT_SKIN_VERTEX_0;


	const xmUint nIndexStride = nIndexType == INDEX_TYPE_U16 ? sizeof(xmUint16) : sizeof(xmUint32);

	m_arrIndexBuffer.resize(nIndexNum * nIndexStride);
	
	m_arrVertexBuffer.resize(nVertexNum * sizeof(VertexType0));

	PointerArrayResize(m_arrMeshLOD,1);

	m_arrMeshLOD[0]->ResizeSubMesh(nSubMeshNum);


	m_meshBound.SetIdentity();

	ResetBone(nBoneNum);

	m_header.m_nIndexNum = nIndexNum;
	m_header.m_nVertexNum = nVertexNum;
		


}


void					MeshData::SetGlobalSkeletonID(GUID nSkelGUID)
{
	m_header.m_nSkelGUID = nSkelGUID;
}



void*					MeshData::GetIndexBuffer()
{
	return m_arrIndexBuffer.size() > 0
		? &m_arrIndexBuffer[0]
	: 0;
}


VertexType0*		MeshData::GetVertexBuffer()
{
	return reinterpret_cast<VertexType0*>(m_arrVertexBuffer.size() > 0
		? &m_arrVertexBuffer[0]
	: 0);
}
	


SubMeshData*		MeshData::GetSubMesh(xmUint nSubMeshInd, int nLOD)
{
	MeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;

	return pLODMesh->GetSubMesh(nSubMeshInd);
}


SubMeshData*		MeshData::GetSubMeshByName(const char* pszName, int nLOD)
{
	MeshLODData *pLODMesh = GetLODMesh(nLOD);
	if (pLODMesh == NULL)
		return NULL;

	const xmUint nSubMeshNum = GetSubMeshNumber();
	for (xmUint32 nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
	{
		SubMeshData* pSubMesh = pLODMesh->GetSubMesh(nSubMeshCnt);
		if (_stricmp(pSubMesh->GetName(),pszName) == 0)
		{
			return pSubMesh;
		}
	}
	return NULL;
	
}



void					MeshData::ResetBone(xmUint32 nBoneNum)
{

	Bounding boundIden;
	boundIden.SetIdentity();
	
	m_arrBoneBound.resize(nBoneNum,boundIden);
	m_arrBoneName.resize(nBoneNum);

	m_header.m_nBoneNum = nBoneNum;
}


void					MeshData::SetBoneName(BoneIndex nBoneInd,const char* pszBoneName)
{
	m_arrBoneName[nBoneInd] = pszBoneName;
}

void					MeshData::SetBoneBoundOBB(BoneIndex nBoneInd
	,const D3DXVECTOR3* pPos,const D3DXQUATERNION* pRot
	,float fXSize,float fYSize,float fZSize)
{
	m_arrBoneBound[nBoneInd].SetOBB(pPos,pRot,fXSize,fYSize,fZSize);
}

void					MeshData::SetBoundingAABB(const D3DXVECTOR3* vMin,const D3DXVECTOR3* vMax)
{
	m_meshBound.SetAABB(*vMin,*vMax);
}

void					MeshData::Serialize(SerializeListener* pSL,const char* pszLabel)
{
	pSL->BeginSection(pszLabel);

	pSL->Serialize(m_header,"MeshHeader");

	pSL->PushVersion(m_header.m_nVersion);
	pSL->Serialize(m_nIndexType,"IndexType");
	pSL->Serialize(m_nVertexType,"VertexType");

	if (INDEX_TYPE_U16 == m_nIndexType)
	{
		pSL->SerializeRawData<BoneIndex>(m_arrIndexBuffer,"IndexBuffer");
	}else if (INDEX_TYPE_U32 == m_nIndexType)
	{
		pSL->SerializeRawData<xmUint32>(m_arrIndexBuffer,"IndexBuffer");
	}else{
		LogError(_ERR_INVALID_CALL,"Fail to serialize mesh data : invalid index type");
	}

	if (VT_SKIN_VERTEX_0 == m_nVertexType)
	{
		pSL->SerializeRawData<VertexType0>(m_arrVertexBuffer,"VertexBuffer");
	}

	pSL->Serialize(m_meshBound,"MeshBound");
 	pSL->Serialize(m_arrMeshLOD,"MeshLOD");
	pSL->Serialize(m_arrBoneName,"BoneName");
	pSL->Serialize(m_arrBoneBound,"BoundBound");

	pSL->PopVersion();

	pSL->EndSection();
}


void					MeshData::SetSource(const char* pszSource)
{
	m_header.m_strMaxFile = pszSource;
}

const char*				MeshData::GetSource() const
{
	return m_header.m_strMaxFile.c_str();
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------



void Bounding::SetInvalid()
{
	m_nShapeType = BS_UNKNOWN;
}

void Bounding::SetIdentity()
{
	D3DXVECTOR3 vMin(-0.5f,-0.5f,-0.5f);
	D3DXVECTOR3 vMax(0.5f,0.5f,0.5f);
	SetAABB(vMin,vMax);
}

void Bounding::SetAABB(const D3DXVECTOR3& vMin,const D3DXVECTOR3& vMax)
{
	m_nShapeType = BS_BOX;
	m_vPos = 0.5f*(vMin + vMax);
	//xmQuaternionIdentity(&m_qRot);
	D3DXQuaternionIdentity(&m_qRot);
	m_boxShape.m_fXSize = vMax.x - vMin.x;
	m_boxShape.m_fYSize = vMax.y - vMin.y;
	m_boxShape.m_fZSize = vMax.z - vMin.z;
}

void Bounding::SetOBB(const D3DXVECTOR3* pPos,const D3DXQUATERNION* pRot
			,float fXSize,float fYSize,float fZSize)
{
	m_nShapeType = BS_BOX;
	//m_vPos = pPos ? *pPos : xmVec3Zero();
	//m_qRot = pRot ? *pRot : xmQuaternionIden();
	m_boxShape.m_fXSize = fXSize;
	m_boxShape.m_fYSize = fYSize;
	m_boxShape.m_fZSize = fZSize;

}

void Bounding::GetAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax) const
{

	if (m_nShapeType == BS_BOX)
	{
		D3DXVECTOR3 vSize(m_boxShape.m_fXSize,m_boxShape.m_fYSize,m_boxShape.m_fZSize);
		vSize = vSize * 0.5f;
		vMin = m_vPos - vSize;
		vMax = m_vPos + vSize;
	}else{
		//LogError(0,"Fail to get AABB");
		vMin = m_vPos;
		vMax = m_vPos;
	}
}

void Serialize(SerializeListener& sl,VertexType0 &val,const char* pszLable )
{
	sl.BeginSection(pszLable);

	sl.Serialize(val.p,"Position");
	sl.Serialize(val.b,"BoneIndice");
	sl.Serialize(val.w,"BoneWeight");
	sl.Serialize(val.n,"Normal");
	sl.Serialize(val.uv,"UV");
	sl.Serialize(val.tgn,"Tangent");
	sl.Serialize(val.vc,"VertexColor");
	sl.Serialize(val.vi,"VertexIllumniation");

	sl.EndSection();

}

void Serialize(SerializeListener& sl,BoxShape &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.m_fXSize,"XSize");
	sl.Serialize(val.m_fYSize,"YSize");
	sl.Serialize(val.m_fZSize,"ZSize");
	sl.EndSection();
}

void Serialize(SerializeListener& sl,CylinderShape &val,const char* pszLable )
{
	sl.BeginSection(pszLable);
	sl.Serialize(val.m_fRadius,"Radius");
	sl.Serialize(val.m_fHeight,"Height");
	sl.EndSection();
}

// void Serialize(SerializeListener& sl,Bounding &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 	sl.Serialize(val.m_nShapeType,"ShapeType");
// 	sl.Serialize(val.m_vPos,"Position");
// 	sl.Serialize(val.m_qRot,"Rotation");
// 	if (_BS_BOX == val.m_nShapeType)
// 	{
// 		sl.Serialize(val.m_boxShape,"BoxShape");
// 	}else if (_BS_CYLINDER == val.m_nShapeType)
// 	{
// 		sl.Serialize(val.m_cylinderShape,"CylinderShape");
// 	}else{
// 		SSERT(false && "unknown bounding shape type");
// 	}
// 
// 	sl.EndSection();
// }

// void Serialize(SerializeListener& sl,SubMesh &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 
// 	sl.Serialize(val.m_nIndexStart,"IndexStart");
// 	sl.Serialize(val.m_nIndexCount,"IndexCount");
// 	sl.Serialize(val.m_nVertexStart,"VertexStart");
// 	sl.Serialize(val.m_nVertexCount,"VertexCount");
// 	sl.Serialize(val.m_subMeshBound,"SubMeshBound");
// 	sl.Serialize(val.m_name,"SubMeshName");
// 	sl.Serialize(val.m_submeshTag,"SubMeshTag");
// 	sl.Serialize(val.m_arrBonePalette,"BonePalette");
// 
// 
// 	sl.EndSection();
// }

// void Serialize(SerializeListener& sl,MeshLOD &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 	sl.Serialize(val.m_arrSubMesh,"SubMesh");
// 	sl.EndSection();
// }




// void Serialize(SerializeListener& sl,MeshData &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 
// 	sl.Serialize(val.m_nIndexType,"IndexType");
// 	sl.Serialize(val.m_nVertexType,"VertexType");
// 
// 	if (_INDEX_TYPE_U16 == val.m_nIndexType)
// 	{
// 		SerializeRawData<BoneIndex>(sl,val.m_arrIndexBuffer,"IndexBuffer");
// 	}else if (_INDEX_TYPE_U32 == val.m_nIndexType)
// 	{
// 		SerializeRawData<xmUint32>(sl,val.m_arrIndexBuffer,"IndexBuffer");
// 	}
// 
// 	if (_VT_SKIN_VERTEX_0 == val.m_nVertexType)
// 	{
// 		SerializeRawData<VertexType0>(sl,val.m_arrVertexBuffer,"VertexBuffer");
// 	}
// 
// 	sl.Serialize(val.m_arrBoneName,"BoneName");
// 
// 	sl.Serialize(val.m_arrMeshLOD,"MeshLOD");
// 
// 	sl.Serialize(val.m_meshBound,"MeshBound");
// 
// 	sl.EndSection();
// }

// void Serialize(SerializeListener& sl,MeshHeader &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 
// 	sl.Serialize(val.m_nIdent,"FileIden");
// 	sl.Serialize(val.m_nVersion,"Version");
// 	sl.Serialize(val.m_strMaxFile,"SourceFile");
// 	if (val.m_nVersion <= _MESH_VER_EMBED_MESH_DATA)
// 	{
// 		sl.Serialize(val.m_nSkelGUID.m_a,"SkeletonGUID");
// 		val.m_nSkelGUID.m_b = 0;
// 	}else{
// 		sl.Serialize(val.m_nSkelGUID,"SkeletonGUID");
// 	}
// 	
// 	sl.Serialize(val.m_nBoneNum,"BoneNumber");
// 	sl.Serialize(val.m_nIndexNum,"IndexNumber");
// 	sl.Serialize(val.m_nVertexNum,"VertexNumber");
// 
// 	sl.EndSection();
// }

// void Serialize(SerializeListener& sl,Mesh &val,const char* pszLable )
// {
// 	sl.BeginSection(pszLable);
// 
// 	sl.Serialize(val.m_header,"MeshHeader");
// 
// 	sl.PushVersion(val.m_header.m_nVersion);
// 	sl.Serialize(val.m_mesh,"MeshData");
// 	sl.PopVersion();
// 
// 	sl.EndSection();
// }

}

