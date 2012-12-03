#ifndef __ExpMeshData_H__
#define __ExpMeshData_H__


enum EExpMeshVersion
{
	EXP_MESH_VER_INITIAL = 1,
	EXP_MESH_VER_USE_SERIALZIE,
	EXP_MESH_VER_EMBED_MESH_DATA,
	EXP_MESH_VER_ANIMATION_RETARGET,
	EXP_MESH_VER_LOD_MESH_DATA,
	EXP_MESH_VER_CURRENT = EXP_MESH_VER_LOD_MESH_DATA
};

enum EExpIndexType
{
	EXP_INDEX_TYPE_U16 = 0,
	EXP_INDEX_TYPE_U32
};

enum EExpBoundingShapeType
{
	EXP_BS_BOX = 0,
	EXP_BS_CYLINDER,
	EXP_BS_NUM,
	EXP_BS_UNKNOWN = 0xffff
};

enum EExpVertexType
{
	EXP_VT_SKIN_VERTEX_0 = 0,
};

struct ExpVertexType0
{
	xmVector3    p;				// position
	xmUint32		   b;				// bones
	xmUint32		   w;				// Weights		
	xmVector3	   n;				// normal
	xmVector2	   uv;				// Texture Coordinate
	xmVector3	   tgn;
	xmUint32          vc;				// vertex color
	xmUint32		   vi;				// vertex illumination

	void	Serialize(SerializeListener& sl,const char* pszLabel = "Vertex");
};

struct ExpBoxShape
{
	float m_fXSize;
	float m_fYSize;
	float m_fZSize;

	void	Serialize(SerializeListener& sl,const char* pszLabel = "ExpBoxShape");

};

struct ExpAABBShape
{
	xmVector3 m_vMin;
	xmVector3 m_vMax;

	void Init();

	void Merge(const xmVector3& vMin,const xmVector3& vMax);

	void AddPoint(const xmVector3& v);
};


struct ExpCylinderShape
{
	float m_fRadius;
	float m_fHeight;

	void Serialize(SerializeListener& sl, const char* pszLable = "ExpCylinderShape" );

};

struct ExpBounding
{
	xmUint16			m_nShapeType;
	xmVector3		m_vPos;
	xmQuaternion	m_qRot;

	union
	{
		ExpBoxShape		m_boxShape;
		ExpCylinderShape m_cylinderShape;
	};

	void SetInvalid();

	void SetIdentity();

	void SetAABB(const xmVector3& vMin,const xmVector3& vMax);

	void GetAABB(xmVector3& vMin,xmVector3& vMax) const;

	void SetOBB(const xmVector3* pPos,const xmQuaternion* pRot
		,float fXSize,float fYSize,float fZSize);

	void Serialize(SerializeListener& sl, const char* pszLable = "ExpBounding" );
};


// struct ExpSubMesh
// {
// 
// 	xmUint m_nIndexStart;
// 	xmUint m_nIndexCount;
// 	xmUint m_nVertexStart;
// 	xmUint m_nVertexCount;
// 
// 	ExpBounding m_subMeshBound;
// 
// 	std::string m_name;
// 	std::string m_submeshTag;
// 	std::vector<xmUint16> m_arrBonePalette;
// 
// 	void Serialize(SerializeListener& sl, const char* pszLable = "ExpSubMesh");
// 
// };

// struct ExpMeshLOD
// {
// 	std::vector<ExpSubMesh> m_arrSubMesh;
// 
// 	void Serialize(SerializeListener& sl, const char* pszLable = "ExpMeshLOD");
// 
// };

// struct ExpMeshData
// {
// 	xmUint m_nIndexType;
// 	xmUint m_nVertexType;
// 	std::vector<xmUint8>	m_arrIndexBuffer;
// 	std::vector<xmUint8>	m_arrVertexBuffer;
// 	std::vector<std::string>	m_arrBoneName;
// 
// 	std::vector<ExpMeshLOD>	m_arrMeshLOD;
// 	std::vector<ExpBounding> m_arrBoneBound;
// 	ExpBounding				m_meshBound;
// 
// 	void Serialize(SerializeListener& sl,const char* pszLable = "ExpMeshData" );
// };

struct ExpMeshHeader
{
	xmUint32		m_nIdent;	 // File Identity = 'S3MD'
	xmUint32		m_nVersion;	 // ∏Ò Ω∞Ê±æ∫≈
	std::string m_strMaxFile;
	GUID		m_nSkelGUID;
	xmUint		m_nBoneNum;
	xmUint		m_nIndexNum;
	xmUint		m_nVertexNum;

	void Serialize(SerializeListener& sl,const char* pszLable = "ExpMeshHeader");
};

// struct ExpMesh
// {
// 	ExpMeshHeader	m_header;
// 	ExpMeshData		m_mesh;
// 
// 
// 	void Serialize(SerializeListener& sl, const char* pszLable = "ExpMesh");
// 
// };

#endif// __ExpMeshData_H__