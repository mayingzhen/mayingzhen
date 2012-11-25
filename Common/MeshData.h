#ifndef __MeshData_H__
#define __MeshData_H__

#include "Common/ExpMeshData.h"
#include "Common/IMesh.h"


void Serialize(SerializeListener& sl,ExpVertexType0 &val,const char* pszLable );

void Serialize(SerializeListener& sl,ExpBoxShape &val,const char* pszLable = "BoxShape");

void Serialize(SerializeListener& sl,ExpCylinderShape& val,const char* pszLable = "CylinderShape");

void Serialize(SerializeListener& sl,ExpBounding &val,const char* pszLable = "MeshBounding");

void Serialize(SerializeListener& sl,ExpSubMesh &val,const char* pszLable = "SubMesh");

void Serialize(SerializeListener& sl,ExpMeshLOD &val,const char* pszLable = "MeshLOD");

void Serialize(SerializeListener& sl,ExpMeshData &val,const char* pszLable = "MeshData");

void Serialize(SerializeListener& sl,ExpMeshHeader &val,const char* pszLable = "MeshHeader");

void Serialize(SerializeListener& sl,ExpMesh &val,const char* pszLable = "Mesh");



class SubMeshData : public ISubMeshData
{
	xmUint m_nIndexStart;
	xmUint m_nIndexCount;
	xmUint m_nVertexStart;
	xmUint m_nVertexCount;

	xmUint m_nMateiralID;

	ExpBounding				m_subMeshBound;
	std::vector<BoneIndex>	m_arrBonePalette;
	std::string					m_name;
	std::string					m_submeshTag;
	

public:

	SubMeshData();

	void	Reset(xmUint nIndexStart,xmUint nIndexCount
		,xmUint nVertexStart,xmUint nVertexCount
		,xmUint nMtlID
		,const ExpBounding* pBounding
		,const BoneIndex* arrBonePalatte
		,xmUint nBonePalatteSize = 0		
		,const char* pszName = NULL
		,const char* pszTag = NULL);

	const char* GetName() const;

	const char*	GetTag() const;

	xmUint	GetIndexStart() const;

	xmUint	GetIndexCount() const;

	xmUint	GetVertexStart() const;

	xmUint	GetVertexCount() const;

	xmUint	GetMaterialID() const;

	ExpBounding          &GetMeshBounding() { return m_subMeshBound; }

	BoneIndex			GetBonePaletteSize() const;

	const BoneIndex*		GetBonePalette() const;

	//------------------------------------------------------------------------------
	//For Modification
	//------------------------------------------------------------------------------
	 void	SetName(const char* pszName);

	 void	SetTag(const char* pszTag);


	 void	SetIndexStart(xmUint32 nIndStart);

	 void	SetIndexCount(xmUint32 nIndCnt);

	 void	SetVertexStart(xmUint32 nVStart);

	 void	SetVertexCount(xmUint32 nVCnt);

	 void	SetMaterialID(xmUint nMtlID);

	/*!\brief
	\param arrBoneIndice array of bone indices, if null, invalid bone index will be filled
	*/
	 void	ResetBonePalatte(xmUint32 nPalatteSize,xmUint16* arrBoneIndice);

	 void	SetBoneByPalatteIndex(xmUint16 nPalatteInd,xmUint16 nBoneInd);

	 xmUint16	GetBoneByPalatteIndex(xmUint16 nPalatteInd);

	void	Serialize(SerializeListener& sl,const char* pszLabel);

};


class MeshLODData : public IMeshLODData
{
	std::vector<SubMeshData*> m_arrSubMesh;

// 	enum MeshDataFrom
// 	{
// 		DATA_PARENT_DATA = 0,
// 		DATA_HAS_OWN_INDEX = 1,
// 		DATA_HAS_OWN_VERTEX = 2,
// 		DATA_HAS_BONE_LOD = 4
// 	};

public:
	xmUint m_nMeshDataFlag;  // MeshDataFrom 位组合

	// DATA_HAS_OWN_INDEX
	xmUint m_nIndexType;
	std::vector<xmUint8>	m_arrIndexBuffer;
	
	// -------------------
	// ===================

	// DATA_HAS_OWN_VERTEX
	xmUint m_nVertexType;
	std::vector<xmUint8>	m_arrVertexBuffer;

	// -------------------
	// ===================
	// 对应骨骼级别
	int m_nBoneLOD;

public:

	MeshLODData();
	~MeshLODData();

	xmUint					GetSubMeshNumber() const;

	const ISubMeshData*	GetSubMesh(xmUint nSubMeshInd) const;

	void					ClearSubMesh();

	void					ResizeSubMesh(xmUint nSubMeshNum);

	ISubMeshData*		CreateSubMesh();

	ISubMeshData*		GetSubMesh(xmUint nSubMeshInd);

	int						GetBoneLOD() const;

	void					SetBoneLOD(int nBoneLOD);

	xmUint					GetMeshDataFlag() const;

	void					SetMeshDataFlag(xmUint uMeshDataFlag);

	xmUint					GetIndexNumber() const;

	bool					GetIsIndex32() const;

	void*					GetIndexBuffer() { return &m_arrIndexBuffer[0]; }

	SkinVertexType0*		GetVertexBufferSkinVertexType0();

	int						GetVertexNumber() const;

	void					ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum);

	void					Serialize(SerializeListener& sl,const char* pszLabel);
};

class MeshData : public IMeshData
{
	ExpMeshHeader			m_header;

	xmUint m_nIndexType;
	xmUint m_nVertexType;

	std::vector<xmUint8>			m_arrIndexBuffer;
	std::vector<xmUint8>			m_arrVertexBuffer;

	ExpBounding					m_meshBound;
	std::vector<MeshLODData*>	m_arrMeshLOD;

	std::vector<std::string>		m_arrBoneName;
	std::vector<ExpBounding>		m_arrBoneBound;
	

private:

	void					Clear();

	void RemoveDegenerateTriangleInplace(std::vector<xmUint8> &arrIndexBuffer, xmUint nIndexType, std::vector<unsigned int> &subIndexMove);

	void DegenerateSubMeshData( MeshLODData * pLODData, std::vector<unsigned int> &subIndexMove );

public:

	ExpMeshHeader &GetExpMeshHeader(void)
	{
		return m_header;
	}

	MeshData();

	~MeshData();


	void				Release();

	//------------------------------------------------------------------------------
	//File Header
	//------------------------------------------------------------------------------
	const char*			GetSource() const;

	GUID				GetGlobalSkeletonID() const;

	//------------------------------------------------------------------------------
	//Stream Source
	//------------------------------------------------------------------------------
	xmUint				GetIndexNumber() const;

	bool				GetIsIndex32() const;

	const void*			GetIndexBuffer() const;

	xmUint				GetVertexNumber() const;

	xmUint				GetVertexSize() const;

	xmUint				GetVertexStride() const;

	xmUint				GetBoneIndexOffset() const;

	xmUint				GetBoneWeightOffset() const;


	SkinVertexType0*	GetVertexBufferSkinVertexType0();

	const SkinVertexType0*	GetVertexBufferSkinVertexType0() const;

	void RemoveDegenerateTriangleInplace(void);


	//------------------------------------------------------------------------------
	//Skin info
	//------------------------------------------------------------------------------
	xmUint				GetBoneNumber() const;

	const char*			GetBoneName(xmUint nBoneInd) const;


	void				GetBoundingAABB(xmVector3* vMin,xmVector3* vMax);

	//------------------------------------------------------------------------------
	//For LOD Mesh
	//------------------------------------------------------------------------------
	xmUint					GetLODMeshNumber() const;
	IMeshLODData*		GetLODMesh(int nLOD);
	const IMeshLODData*	GetLODMesh(int nLOD) const;
	int						GetBoneLOD(int nMeshLOD) const;
	void					SetBoneLOD(int nMeshLOD, int nBoneLOD);
	IMeshLODData*		IncLOD();
	void					DecLOD();
	void					SetLodMeshVersion();

	//------------------------------------------------------------------------------
	//For modification
	//------------------------------------------------------------------------------

	void					ResetBuffer(
		xmUint nIndexType,xmUint32 nIndexNum
		,xmUint32 nVertexNum
		,xmUint32 nSubMeshNum
		,xmUint32 nBoneNum
		);

	void					SetSource(const char* pszSource);

	void					SetGlobalSkeletonID(GUID nSkelGUID);


	void*					GetIndexBuffer();

	SkinVertexType0*		GetVertexBuffer();	


	//------------------------------------------------------------------------------
	//Sub mesh
	//------------------------------------------------------------------------------
	xmUint					GetSubMeshNumber() const;

	const ISubMeshData*	GetSubMesh(xmUint nSubMeshInd, int nLOD = 0) const;

	ISubMeshData*		GetSubMesh(xmUint nSubMeshInd, int nLOD = 0);

	ISubMeshData*		GetSubMeshByName(const char* pszName, int nLOD = 0);


	void					ResetBone(xmUint32 nBoneNum);

	void					SetBoneName(BoneIndex nBoneInd,const char* pszBoneName);

	void					SetBoneBoundOBB(BoneIndex nBoneInd,const xmVector3* pPos,const xmQuaternion* pRot,float fXSize,float fYSize,float fZSize);

	void					SetBoundingAABB(const xmVector3* vMin,const xmVector3* vMax);

	void					Serialize(SerializeListener* pSL,const char* pszLabel);

};

#endif// __MeshData_H__