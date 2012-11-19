#ifndef  _ExpMeshData__H__
#define  _ExpMeshData__H__

struct ExpVertexType
{
	D3DXVECTOR3 p;		// position
	UINT		b;		// bones
	UINT		w;		// weights
	D3DXVECTOR3	n;		// normal
	D3DXVECTOR2 UV;		// texture coordinate
	D3DXVECTOR2 tgn;	
	UINT		vc;     // vertex color
	UINT        vi;	    // vertex illumination
};

struct ExpBoxShape
{
	float m_fXSize;
	float m_fYSize;
	float m_fZSize;
};

struct ExpBounding
{
	unsigned short  m_nShapeType;
	D3DXVECTOR3		m_vPos;
	D3DXQUATERNION* m_qRot;

	union
	{
		ExpBox
	};

};

struct ExpSubMesh
{
	UINT m_nIndexStart;
	UINT m_nIndexCount;
	UINT m_nVertexStart;
	UINT m_nVertexCount;

	ExpBounding m_subMeshBound;

	std::string m_name;
	std::string m_submeshTag;
	std::<unsigned short> m_arrBonePalette;
};

struct ExpMeshData
{
	UINT m_nIndexType;
	UINT m_nVertexType;
	byte* m_arrIndexBuffer;
	byte* m_arrVertexBuffer;
	std::vector<std::string> m_arrBoneName;
};

struct ExpMeshHeader
{
	UINT		m_nIdent;
	UINT		m_nVersion;
	std::string m_strMaxFile;
	maGUID		m_nSkelGUID;
	UINT		m_nBoneNum;
	UINT		m_nIndexNum;
	UINT		m_nVertexNum;
}

#endif
