//----------------------------------------------------------------------
// Copyright(c) TQ Digital Entertainment, All Rights Reserved
// Author:  Xu Kaijie
// Created: 2010/3/3
// Describe:S3 Animation
//----------------------------------------------------------------------

#ifndef __IMesh_H__
#define __IMesh_H__

class SerializeListener;
class IVertexIterator;
struct ExpBounding;
class MeshLODData;
class UTMeshBase;

struct SkinVertexType0
{
	xmVector3    m_vPos;			// position
	xmUint32		   m_nBoneIndice;	// bones
	xmUint32		   m_nBoneWeight;	// Weights		
	xmVector3	   m_vNorm;			// normal
	xmVector2	   m_vUV;			// Texture Coordinate
	xmVector3	   m_vTangent;
	xmUint32          m_nColor;		// vertex color
	xmUint32		   m_nIllum;		// vertex illumination
};

class COMMON_API ISubMeshData
{
public:
	virtual const char* GetName() const = 0;

	virtual const char*	GetTag() const = 0;

	virtual xmUint	GetIndexStart() const = 0;

	virtual xmUint	GetIndexCount() const = 0;
	
	virtual xmUint	GetVertexStart() const = 0;

	virtual xmUint	GetVertexCount() const = 0;

	virtual ExpBounding  &GetMeshBounding() = 0;

	virtual xmUint16			GetBonePaletteSize() const = 0;

	virtual const xmUint16*	GetBonePalette() const = 0;

	virtual xmUint	GetMaterialID() const = 0;

	//------------------------------------------------------------------------------
	//For Modification
	//------------------------------------------------------------------------------
	virtual void	Reset(xmUint nIndexStart,xmUint nIndexCount
		,xmUint nVertexStart,xmUint nVertexCount
		,xmUint nMtlID
		,const ExpBounding* pBounding
		,const BoneIndex* arrBonePalatte
		,xmUint nBonePalatteSize = 0		
		,const char* pszName = NULL
		,const char* pszTag = NULL) = 0;

	virtual void	SetName(const char* pszName) = 0;

	virtual void	SetTag(const char* pszTag) = 0;


	virtual void	SetIndexStart(xmUint32 nIndStart) = 0;

	virtual void	SetIndexCount(xmUint32 nIndCnt) = 0;

	virtual void	SetVertexStart(xmUint32 nVStart) = 0;

	virtual void	SetVertexCount(xmUint32 nVCnt) = 0;

	/*!\brief
	\param arrBoneIndice array of bone indices, if null, invalid bone index will be filled
	*/
	virtual void	ResetBonePalatte(xmUint32 nPalatteSize,xmUint16* arrBoneIndice) = 0;

	virtual void	SetBoneByPalatteIndex(xmUint16 nPalatteInd,xmUint16 nBoneInd) = 0;

	virtual xmUint16	GetBoneByPalatteIndex(xmUint16 nPalatteInd) = 0;

	virtual void	SetMaterialID(xmUint nMtlID) = 0;



};

class COMMON_API IMeshLODData
{
public:
	enum MeshDataFrom
	{
		DATA_PARENT_DATA = 0,
		DATA_HAS_OWN_INDEX = 1,
		DATA_HAS_OWN_VERTEX = 2,
		DATA_HAS_BONE_LOD = 4
	};

	virtual xmUint					GetSubMeshNumber() const = 0;

	virtual const ISubMeshData*	GetSubMesh(xmUint nSubMeshInd) const = 0;

	virtual void					ClearSubMesh() = 0;

	virtual void					ResizeSubMesh(xmUint nSubMeshNum) = 0;

	virtual ISubMeshData*		CreateSubMesh() = 0;

	virtual ISubMeshData*		GetSubMesh(xmUint nSubMeshInd) = 0;

	virtual int						GetBoneLOD() const = 0;

	virtual void					SetBoneLOD(int nBoneLOD) = 0;

	virtual xmUint					GetMeshDataFlag() const = 0;

	virtual	void					SetMeshDataFlag(xmUint uMeshDataFlag) = 0;

	virtual xmUint					GetIndexNumber() const = 0;

	virtual bool					GetIsIndex32() const = 0;

	virtual void*					GetIndexBuffer() = 0;

	virtual SkinVertexType0*		GetVertexBufferSkinVertexType0() = 0;

	virtual int						GetVertexNumber() const = 0;

	virtual void					ResetBuffer(bool bIsIndex32, xmUint32 nIndexNum, xmUint32 nVertexNum) = 0;

	virtual void					Serialize(SerializeListener& sl,const char* pszLabel) = 0;
};

class COMMON_API IMeshData
{
public:

	virtual void						Release() = 0;

	virtual GUID						GetGlobalSkeletonID() const = 0;

	virtual const char*					GetSource() const = 0;

	virtual xmUint						GetIndexNumber() const = 0;

	virtual bool						GetIsIndex32() const = 0;

	virtual const void*					GetIndexBuffer() const = 0;

	virtual xmUint						GetVertexNumber() const = 0;

	virtual xmUint						GetVertexStride() const = 0;

	virtual xmUint						GetBoneIndexOffset() const = 0;

	virtual xmUint						GetBoneWeightOffset() const = 0;

	virtual SkinVertexType0*			GetVertexBufferSkinVertexType0() = 0;

	virtual const SkinVertexType0*	GetVertexBufferSkinVertexType0() const = 0;

	virtual xmUint						GetSubMeshNumber() const = 0;

	virtual const ISubMeshData*		GetSubMesh(xmUint nSubMesh, int nLOD = 0) const = 0;


	virtual xmUint						GetBoneNumber() const = 0;

	virtual const char*					GetBoneName(xmUint nBoneInd) const = 0;


	virtual void						GetBoundingAABB(xmVector3* vMin,xmVector3* vMax) = 0;

	//------------------------------------------------------------------------------
	//For LOD Mesh
	//------------------------------------------------------------------------------
	virtual xmUint						GetLODMeshNumber() const       = 0;
	virtual IMeshLODData*			GetLODMesh(int nLOD)          = 0;
	virtual const IMeshLODData*		GetLODMesh(int nLOD) const    = 0;
	virtual int							GetBoneLOD(int nMeshLOD) const = 0;
	virtual void						SetBoneLOD(int nMeshLOD, int nBoneLOD) = 0;
	virtual IMeshLODData*			IncLOD() = 0;
	virtual void						DecLOD(void) = 0;	
	virtual	void						SetLodMeshVersion() = 0;

	//------------------------------------------------------------------------------
	//For modification
	//------------------------------------------------------------------------------
	virtual void					SetGlobalSkeletonID(GUID nSkelGUID) = 0;

	virtual void					SetSource(const char* pszSource) = 0;

	virtual void					ResetBuffer(
		xmUint nIndexType,xmUint32 nIndexNum
		,xmUint32 nVertexNum
		,xmUint32 nSubMeshNum
		,xmUint32 nBoneNum
		) = 0;


	virtual void*					GetIndexBuffer() = 0;

	virtual SkinVertexType0*		GetVertexBuffer() = 0;	

	virtual ISubMeshData*		GetSubMesh(xmUint nSubMeshInd, int nLOD = 0) = 0;

	virtual ISubMeshData*		GetSubMeshByName(const char* pszName, int nLOD = 0) = 0;


	virtual void					SetBoneName(xmUint16 nBoneInd,const char* pszBoneName) = 0;

	virtual void					SetBoundingAABB(const xmVector3* vMin,const xmVector3* vMax) = 0;
	
};

class IMeshFile
{
public:

	virtual void Release();
};

#endif// __IMesh_H__