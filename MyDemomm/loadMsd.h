#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>

// 网格类型
enum ENUM_MESH_TYPE
{
	ENUM_MESH_NULL	  = 0x100, // 空网格
	ENUM_MESH_DYNAMIC = 0x101, // 动态网格
	ENUM_MESH_STATIC  = 0x102, // 静态网格
};

#define MESH_VERTEXFVF_DEFAULT	D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1

//网格基本属性数据
struct SMeshDataBase
{
	SMeshDataBase():m_dwVertexStride(0),m_dwVertexFVF(MESH_VERTEXFVF_DEFAULT),
		m_nPrimitiveType(D3DPT_TRIANGLELIST),m_dwPrimitiveCount(0)
		,m_nMeshType(ENUM_MESH_NULL),m_dwIndicesNum(0)
		,m_dwVerticesNum(0),m_IdxFormat(D3DFMT_INDEX16)
		,m_dwStartIndex(0)
	{
	}

	DWORD				m_dwVertexStride;		// 顶点跨幅
	DWORD				m_dwVertexFVF;			// 顶点格式
	D3DPRIMITIVETYPE 	m_nPrimitiveType;		// 图元类型
	DWORD				m_dwVerticesNum;		// 顶点数目
	DWORD				m_dwIndicesNum;			// 索引数目
	D3DFORMAT			m_IdxFormat;			// 索引格式
	DWORD				m_dwStartIndex;			// 起始索引
	DWORD				m_dwPrimitiveCount;		// 图元个数
	ENUM_MESH_TYPE		m_nMeshType;			// 网格类型:静态,动态
};



struct CMaterialData
{
	D3DMATERIAL9 m_material;
	std::string	m_strTextureFileName;
};


typedef std::map<std::string,D3DXMATRIX> boneNameToMatrix;

struct CSubMesh
{
	std::string m_sSubMeshName;
	D3DXMATRIX m_matRoot;
	D3DXMATRIX m_matOffset;

	SMeshDataBase m_sMesh;

	DWORD m_dwVerBufSize;	// 顶点缓冲大小
	char* m_pVertexData;	// 内存顶点缓冲指针
	char* m_pVertexDataTemp; //
	DWORD m_dwIdxBufSize;	// 索引缓冲大小
	char* m_pIndexData;     // 内存索引缓冲指针
	DWORD m_dwBonesPerVer;	// 每个顶点的骨骼数目
	DWORD* m_pdwVertexInflunece;	//顶点骨骼索引缓冲指针大小为m_dwVerticesNum*sizeof(DWORD)*m_dwBonesPerVer
	float* m_pfVertexWeight;		//顶点权重缓冲指针  大小为m_dwVerticesNum*sizeof(FLOAT)*m_dwBonesPerVer
	DWORD m_dwVerRedundant;		//冗余顶点个数
	DWORD* m_pdwVertexRedundant;	//冗余顶点列表，长度为m_dwVerticesNum对于索引为i的顶点

	DWORD m_dwMaterialNum;		//材质数量
	std::vector<CMaterialData> m_vMateria;	

	DWORD* m_pdwPrimitveMatList;	// 图元材质列表，长度为m_dwPrimitiveCount
	DWORD* m_pdwMaterialFaceOffset;//材质对应图元(面)偏移(相对索引列表)
	DWORD* m_pdwMaterialFaceCount; //材质对应图元个数

	boneNameToMatrix m_mBoneMatrix;
	std::vector<D3DXMATRIX> m_vMatrix;
	std::vector<std::string> m_vBone;

	boneNameToMatrix m_matLocalRoot;

 	void Update(DWORD nCurFrame);
	void Render();
};

class CMesh
{
	std::vector<CSubMesh> m_vListMesh;
};


struct CAction
{
	int m_nActionID;                // 动作ID
	int m_nFrame;					// 帧数
	int m_nBoneNum;
	std::vector<std::string>	m_listBoneName;		//骨骼名称列表

	float  m_fTotalTime;			// 帧时间总和
	float* m_pfFrameTime;			// 每帧时间列表，共有nFrame个
	float* m_pfOriginFrameTime;	    // 原始每帧时间列表，用于编辑器保存Max原始时间

	D3DXMATRIX*	m_pMatrixList;		// 该动作涉及到的骨头绝对矩阵列表，共有nFrame * nBoneNumber个

	DWORD GetPositionByTime(float& fRefTime);
	void Update(DWORD nCurFrame);
};

enum CHA_PART
{
	PART_INVALID  = -1,				// 任何位置都不能
	PART_FIRST	  = 0,
	PART_HEAD     = 0,				// 头部
	PART_BODY     = 1,				// 身体（上身）
	PART_WEAPON   = 2,				// 武器
	PART_FOOT     = 3,				// 脚部
	PART_CAP	  = 4,				// 帽子
	PART_HAND	  = 5,				// 手
	PART_LAST	  = PART_HAND,
	PART_NUM	  = 6,				// = 8
};

struct CBone
{
	int mHandle;
	std::string sName;
	D3DXVECTOR3 vBoxMin;
	D3DXVECTOR3 vBoxMax;
	D3DXMATRIX matRoot;  	//物体局部坐标原点相对于模型世界坐标原点的变换矩阵
	D3DXMATRIX matOffset; //物体局部坐标原点相对于父物体坐标原点的变换矩阵
	BOOL bIsVisible;
};


typedef	std::map<std::string,CBone>	BoneMapByName;

class CChaSkinMesh // Animate Skeleton 骨骼动画类
{

public:
	//void SetPartMesh(CHA_PART chaPart,CMesh mesh) {m_chaMesh[chaPart] = mesh;}

	void Build();
	void Update(FLOAT fElapsedTime);
	void Render();


	std::vector<CSubMesh>/*CMesh*/ m_chaMesh[PART_NUM];  // mesh 数据
	BoneMapByName m_allBone;    // 骨骼框架
	std::vector<CAction> m_vAct; // 动作
	CAction m_nCurAct;
};

extern CChaSkinMesh chaSkinMesh;