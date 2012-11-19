#ifndef __S3AExpMeshData_H__
#define __S3AExpMeshData_H__


enum ES3AExpMeshVersion
{
	EXP_MESH_VER_INITIAL = 1,
	EXP_MESH_VER_USE_SERIALZIE,
	EXP_MESH_VER_EMBED_MESH_DATA,
	EXP_MESH_VER_ANIMATION_RETARGET,
	EXP_MESH_VER_LOD_MESH_DATA,
	EXP_MESH_VER_CURRENT = EXP_MESH_VER_LOD_MESH_DATA
};

enum ES3AExpIndexType
{
	EXP_INDEX_TYPE_U16 = 0,
	EXP_INDEX_TYPE_U32
};

enum ES3AExpBoundingShapeType
{
	EXP_BS_BOX = 0,
	EXP_BS_CYLINDER,
	EXP_BS_NUM,
	EXP_BS_UNKNOWN = 0xffff
};

enum ES3AExpVertexType
{
	EXP_VT_SKIN_VERTEX_0 = 0,
};

struct S3AExpVertexType0
{
	xmVector3    p;				// position
	xmUint32		   b;				// bones
	xmUint32		   w;				// Weights		
	xmVector3	   n;				// normal
	xmVector2	   uv;				// Texture Coordinate
	xmVector3	   tgn;
	xmUint32          vc;				// vertex color
	xmUint32		   vi;				// vertex illumination
};

struct S3AExpBoxShape
{
	float m_fXSize;
	float m_fYSize;
	float m_fZSize;
};

struct S3AExpAABBShape
{
	xmVector3 m_vMin;
	xmVector3 m_vMax;

	void Init();

	void Merge(const xmVector3& vMin,const xmVector3& vMax);

	void AddPoint(const xmVector3& v);
};

inline void S3AExpAABBShape::Init()
{
	m_vMin = xmVector3(xm_FMAX,xm_FMAX,xm_FMAX);
	m_vMax = xmVector3(-xm_FMAX,-xm_FMAX,-xm_FMAX);
}

inline void S3AExpAABBShape::Merge(const xmVector3& vMin,const xmVector3& vMax)
{
	xmVec3Min(&m_vMin,&vMin,&m_vMin);
	xmVec3Max(&m_vMax,&vMax,&m_vMax);
}

inline void S3AExpAABBShape::AddPoint(const xmVector3& v)
{
	xmVec3Min(&m_vMin,&v,&m_vMin);
	xmVec3Max(&m_vMax,&v,&m_vMax);
}

struct S3AExpCylinderShape
{
	float m_fRadius;
	float m_fHeight;
};

struct S3AExpBounding
{
	xmUint16			m_nShapeType;
	xmVector3		m_vPos;
	xmQuaternion	m_qRot;

	union
	{
		S3AExpBoxShape		m_boxShape;
		S3AExpCylinderShape m_cylinderShape;
	};

	void SetInvalid();

	void SetIdentity();

	void SetAABB(const xmVector3& vMin,const xmVector3& vMax);

	void GetAABB(xmVector3& vMin,xmVector3& vMax) const;

	void SetOBB(const xmVector3* pPos,const xmQuaternion* pRot
		,float fXSize,float fYSize,float fZSize);
};


struct S3AExpSubMesh
{

	xmUint m_nIndexStart;
	xmUint m_nIndexCount;
	xmUint m_nVertexStart;
	xmUint m_nVertexCount;

	S3AExpBounding m_subMeshBound;

	std::string m_name;
	std::string m_submeshTag;
	std::vector<xmUint16> m_arrBonePalette;

};

struct S3AExpMeshLOD
{
	std::vector<S3AExpSubMesh> m_arrSubMesh;
};

struct S3AExpMeshData
{
	xmUint m_nIndexType;
	xmUint m_nVertexType;
	std::vector<xmUint8>	m_arrIndexBuffer;
	std::vector<xmUint8>	m_arrVertexBuffer;
	std::vector<std::string>	m_arrBoneName;

	std::vector<S3AExpMeshLOD>	m_arrMeshLOD;
	std::vector<S3AExpBounding> m_arrBoneBound;
	S3AExpBounding				m_meshBound;
};

struct S3AExpMeshHeader
{
	xmUint32		m_nIdent;	 // File Identity = 'S3MD'
	xmUint32		m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈
	std::string m_strMaxFile;
	S3AGUID		m_nSkelGUID;
	xmUint		m_nBoneNum;
	xmUint		m_nIndexNum;
	xmUint		m_nVertexNum;
};

struct S3AExpMesh
{
	S3AExpMeshHeader	m_header;
	S3AExpMeshData		m_mesh;
};

#endif// __S3AExpMeshData_H__