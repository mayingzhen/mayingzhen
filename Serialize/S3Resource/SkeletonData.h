#ifndef __S3SkeletonData_H__
#define __S3SkeletonData_H__

namespace S3
{



enum SkelVersion
{
	EXP_SKEL_VER_INITIAL = 0,
	EXP_SKEL_VER_ANIM_RETARGET,//Use  SkelGUID
	EXP_SKEL_VER_BONE_LOD_MASK,
	EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_BONE_LOD_MASK,
};


struct ENGINE_API SkeletonHeader
{
	UINT m_nIden;
	UINT m_nVersion;
	std::string m_strMaxFile;

	void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonHeader" );
};

struct ENGINE_API SocketData
{
	std::string		m_strBoneName;
	UINT			m_nBoneID;
	D3DXMATRIX		m_matTransformBS;

	void Serialize(SerializeListener& sl, const char* pszLable = "SocketData" );
};

struct ENGINE_API SkeletonData /*: public ISkeletonData*/
{
	GUID	m_nGlobalSkeletonID;
	UINT	m_nBoneNum;
	std::vector<std::string>	m_arrBoneName;
	std::vector<UINT>	m_arrParentIndice;
	std::vector<D3DXVECTOR3>	m_arrScaleOS;
	std::vector<D3DXQUATERNION>	m_arrRotOS;
	std::vector<D3DXVECTOR3>	m_arrPosOS;
	std::vector<SocketData> m_arrSocket;
	//typedef std::vector<xmUint8>   BoolList;
	std::vector<std::string>  m_arrLODBoneMask;

	void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonData" );
};

}

#endif// __ExpSkeletonData_H__