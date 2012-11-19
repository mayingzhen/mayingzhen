//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/8/9
// Describe: S3 Animation
//----------------------------------------------------------------------

#ifndef __S3AMeshData_H__
#define __S3AMeshData_H__

#include "Common/S3AExpMeshData.h"
#include "Common/IS3AMesh.h"

struct ID3DXMesh;


void S3ASerialize(IS3ASerializeListener& sl,S3AExpVertexType0 &val,const char* pszLable );

void S3ASerialize(IS3ASerializeListener& sl,S3AExpBoxShape &val,const char* pszLable = "BoxShape");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpCylinderShape& val,const char* pszLable = "CylinderShape");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpBounding &val,const char* pszLable = "MeshBounding");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpSubMesh &val,const char* pszLable = "SubMesh");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshLOD &val,const char* pszLable = "MeshLOD");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshData &val,const char* pszLable = "MeshData");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMeshHeader &val,const char* pszLable = "MeshHeader");

void S3ASerialize(IS3ASerializeListener& sl,S3AExpMesh &val,const char* pszLable = "Mesh");



class S3ASubMeshData : public IS3ASubMeshData
{
	xmUint m_nIndexStart;
	xmUint m_nIndexCount;
	xmUint m_nVertexStart;
	xmUint m_nVertexCount;

	xmUint m_nMateiralID;

	S3AExpBounding				m_subMeshBound;
	std::vector<S3ABoneIndex>	m_arrBonePalette;
	std::string					m_name;
	std::string					m_submeshTag;
	

public:

	S3ASubMeshData();

	void	Reset(xmUint nIndexStart,xmUint nIndexCount
		,xmUint nVertexStart,xmUint nVertexCount
		,xmUint nMtlID
		,const S3AExpBounding* pBounding
		,const S3ABoneIndex* arrBonePalatte
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

	S3AExpBounding          &GetMeshBounding() { return m_subMeshBound; }

	S3ABoneIndex			GetBonePaletteSize() const;

	const S3ABoneIndex*		GetBonePalette() const;

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

	void	Serialize(IS3ASerializeListener& sl,const char* pszLabel);

};


class S3AMeshLODData : public IS3AMeshLODData
{
	std::vector<S3ASubMeshData*> m_arrSubMesh;

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

	S3AMeshLODData();
	~S3AMeshLODData();

	xmUint					GetSubMeshNumber() const;

	const IS3ASubMeshData*	GetSubMesh(xmUint nSubMeshInd) const;

	void					ClearSubMesh();

	void					ResizeSubMesh(xmUint nSubMeshNum);

	IS3ASubMeshData*		CreateSubMesh();

	IS3ASubMeshData*		GetSubMesh(xmUint nSubMeshInd);

	int						GetBoneLOD() const;

	void					SetBoneLOD(int nBoneLOD);

	xmUint					GetMeshDataFlag() const;

	void					SetMeshDataFlag(xmUint uMeshDataFlag);

	xmUint					GetIndexNumber() const;

	bool					GetIsIndex32() const;

	void*					GetIndexBuffer() { return &m_arrIndexBuffer[0]; }

	S3ASkinVertexType0*		GetVertexBufferSkinVertexType0();

	int						GetVertexNumber() const;

	void					ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum);

	void					Serialize(IS3ASerializeListener& sl,const char* pszLabel);
};

class S3AMeshData : public IS3AMeshData
{
	S3AExpMeshHeader			m_header;

	xmUint m_nIndexType;
	xmUint m_nVertexType;

	std::vector<xmUint8>			m_arrIndexBuffer;
	std::vector<xmUint8>			m_arrVertexBuffer;

	S3AExpBounding					m_meshBound;
	std::vector<S3AMeshLODData*>	m_arrMeshLOD;

	std::vector<std::string>		m_arrBoneName;
	std::vector<S3AExpBounding>		m_arrBoneBound;
	

private:

	void					Clear();

	void RemoveDegenerateTriangleInplace(std::vector<xmUint8> &arrIndexBuffer, xmUint nIndexType, std::vector<unsigned int> &subIndexMove);

	void DegenerateSubMeshData( S3AMeshLODData * pLODData, std::vector<unsigned int> &subIndexMove );

public:

	S3AExpMeshHeader &GetExpMeshHeader(void)
	{
		return m_header;
	}

	S3AMeshData();

	~S3AMeshData();


	void				Release();

	//------------------------------------------------------------------------------
	//File Header
	//------------------------------------------------------------------------------
	const char*			GetSource() const;

	S3AGUID				GetGlobalSkeletonID() const;

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


	S3ASkinVertexType0*	GetVertexBufferSkinVertexType0();

	const S3ASkinVertexType0*	GetVertexBufferSkinVertexType0() const;

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
	IS3AMeshLODData*		GetLODMesh(int nLOD);
	const IS3AMeshLODData*	GetLODMesh(int nLOD) const;
	int						GetBoneLOD(int nMeshLOD) const;
	void					SetBoneLOD(int nMeshLOD, int nBoneLOD);
	IS3AMeshLODData*		IncLOD();
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

	void					SetGlobalSkeletonID(S3AGUID nSkelGUID);


	void*					GetIndexBuffer();

	S3ASkinVertexType0*		GetVertexBuffer();	


	//------------------------------------------------------------------------------
	//Sub mesh
	//------------------------------------------------------------------------------
	xmUint					GetSubMeshNumber() const;

	const IS3ASubMeshData*	GetSubMesh(xmUint nSubMeshInd, int nLOD = 0) const;

	IS3ASubMeshData*		GetSubMesh(xmUint nSubMeshInd, int nLOD = 0);

	IS3ASubMeshData*		GetSubMeshByName(const char* pszName, int nLOD = 0);


	void					ResetBone(xmUint32 nBoneNum);

	void					SetBoneName(S3ABoneIndex nBoneInd,const char* pszBoneName);

	void					SetBoneBoundOBB(S3ABoneIndex nBoneInd,const xmVector3* pPos,const xmQuaternion* pRot,float fXSize,float fYSize,float fZSize);

	void					SetBoundingAABB(const xmVector3* vMin,const xmVector3* vMax);

	void					Serialize(IS3ASerializeListener* pSL,const char* pszLabel);

};

#endif// __S3AMeshData_H__