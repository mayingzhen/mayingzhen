#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>

// ��������
enum ENUM_MESH_TYPE
{
	ENUM_MESH_NULL	  = 0x100, // ������
	ENUM_MESH_DYNAMIC = 0x101, // ��̬����
	ENUM_MESH_STATIC  = 0x102, // ��̬����
};

#define MESH_VERTEXFVF_DEFAULT	D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1

//���������������
struct SMeshDataBase
{
	SMeshDataBase():m_dwVertexStride(0),m_dwVertexFVF(MESH_VERTEXFVF_DEFAULT),
		m_nPrimitiveType(D3DPT_TRIANGLELIST),m_dwPrimitiveCount(0)
		,m_nMeshType(ENUM_MESH_NULL),m_dwIndicesNum(0)
		,m_dwVerticesNum(0),m_IdxFormat(D3DFMT_INDEX16)
		,m_dwStartIndex(0)
	{
	}

	DWORD				m_dwVertexStride;		// ������
	DWORD				m_dwVertexFVF;			// �����ʽ
	D3DPRIMITIVETYPE 	m_nPrimitiveType;		// ͼԪ����
	DWORD				m_dwVerticesNum;		// ������Ŀ
	DWORD				m_dwIndicesNum;			// ������Ŀ
	D3DFORMAT			m_IdxFormat;			// ������ʽ
	DWORD				m_dwStartIndex;			// ��ʼ����
	DWORD				m_dwPrimitiveCount;		// ͼԪ����
	ENUM_MESH_TYPE		m_nMeshType;			// ��������:��̬,��̬
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

	DWORD m_dwVerBufSize;	// ���㻺���С
	char* m_pVertexData;	// �ڴ涥�㻺��ָ��
	char* m_pVertexDataTemp; //
	DWORD m_dwIdxBufSize;	// ���������С
	char* m_pIndexData;     // �ڴ���������ָ��
	DWORD m_dwBonesPerVer;	// ÿ������Ĺ�����Ŀ
	DWORD* m_pdwVertexInflunece;	//���������������ָ���СΪm_dwVerticesNum*sizeof(DWORD)*m_dwBonesPerVer
	float* m_pfVertexWeight;		//����Ȩ�ػ���ָ��  ��СΪm_dwVerticesNum*sizeof(FLOAT)*m_dwBonesPerVer
	DWORD m_dwVerRedundant;		//���ඥ�����
	DWORD* m_pdwVertexRedundant;	//���ඥ���б�����Ϊm_dwVerticesNum��������Ϊi�Ķ���

	DWORD m_dwMaterialNum;		//��������
	std::vector<CMaterialData> m_vMateria;	

	DWORD* m_pdwPrimitveMatList;	// ͼԪ�����б�����Ϊm_dwPrimitiveCount
	DWORD* m_pdwMaterialFaceOffset;//���ʶ�ӦͼԪ(��)ƫ��(��������б�)
	DWORD* m_pdwMaterialFaceCount; //���ʶ�ӦͼԪ����

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
	int m_nActionID;                // ����ID
	int m_nFrame;					// ֡��
	int m_nBoneNum;
	std::vector<std::string>	m_listBoneName;		//���������б�

	float  m_fTotalTime;			// ֡ʱ���ܺ�
	float* m_pfFrameTime;			// ÿ֡ʱ���б�����nFrame��
	float* m_pfOriginFrameTime;	    // ԭʼÿ֡ʱ���б����ڱ༭������Maxԭʼʱ��

	D3DXMATRIX*	m_pMatrixList;		// �ö����漰���Ĺ�ͷ���Ծ����б�����nFrame * nBoneNumber��

	DWORD GetPositionByTime(float& fRefTime);
	void Update(DWORD nCurFrame);
};

enum CHA_PART
{
	PART_INVALID  = -1,				// �κ�λ�ö�����
	PART_FIRST	  = 0,
	PART_HEAD     = 0,				// ͷ��
	PART_BODY     = 1,				// ���壨����
	PART_WEAPON   = 2,				// ����
	PART_FOOT     = 3,				// �Ų�
	PART_CAP	  = 4,				// ñ��
	PART_HAND	  = 5,				// ��
	PART_LAST	  = PART_HAND,
	PART_NUM	  = 6,				// = 8
};

struct CBone
{
	int mHandle;
	std::string sName;
	D3DXVECTOR3 vBoxMin;
	D3DXVECTOR3 vBoxMax;
	D3DXMATRIX matRoot;  	//����ֲ�����ԭ�������ģ����������ԭ��ı任����
	D3DXMATRIX matOffset; //����ֲ�����ԭ������ڸ���������ԭ��ı任����
	BOOL bIsVisible;
};


typedef	std::map<std::string,CBone>	BoneMapByName;

class CChaSkinMesh // Animate Skeleton ����������
{

public:
	//void SetPartMesh(CHA_PART chaPart,CMesh mesh) {m_chaMesh[chaPart] = mesh;}

	void Build();
	void Update(FLOAT fElapsedTime);
	void Render();


	std::vector<CSubMesh>/*CMesh*/ m_chaMesh[PART_NUM];  // mesh ����
	BoneMapByName m_allBone;    // �������
	std::vector<CAction> m_vAct; // ����
	CAction m_nCurAct;
};

extern CChaSkinMesh chaSkinMesh;