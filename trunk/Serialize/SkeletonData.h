#ifndef __SkeletonData_H__
#define __SkeletonData_H__

enum SkelVersion
{
	EXP_SKEL_VER_INITIAL = 0,
	EXP_SKEL_VER_ANIM_RETARGET,//Use  SkelGUID
	EXP_SKEL_VER_BONE_LOD_MASK,
	EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_BONE_LOD_MASK,
};


struct SERIALIZE_API SkeletonHeader
{
	xmUint m_nIden;
	xmUint m_nVersion;
	std::string m_strMaxFile;

	void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonHeader" );
};

struct SERIALIZE_API SocketData
{
	std::string		m_strBoneName;
	xmUint			m_nBoneID;
	D3DXMATRIX		m_matTransformBS;

	void Serialize(SerializeListener& sl, const char* pszLable = "SocketData" );
};

struct SERIALIZE_API SkeletonData /*: public ISkeletonData*/
{
	GUID	m_nGlobalSkeletonID;
	xmUint	m_nBoneNum;
	std::vector<std::string>	m_arrBoneName;
	std::vector<xmUint>	m_arrParentIndice;
	std::vector<D3DXVECTOR3>	m_arrScaleOS;
	std::vector<D3DXQUATERNION>	m_arrRotOS;
	std::vector<D3DXVECTOR3>	m_arrPosOS;
	std::vector<SocketData> m_arrSocket;
	//typedef std::vector<xmUint8>   BoolList;
	std::vector<std::string>  m_arrLODBoneMask;

	void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonData" );
};


#endif// __ExpSkeletonData_H__