#ifndef __SkeletonData_H__
#define __SkeletonData_H__

namespace ma
{
	enum SkelVersion
	{
		EXP_SKEL_VER_INITIAL = 1,
		EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_INITIAL,
	};


	struct ENGINE_API SkeletonHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonHeader" );
	};


	struct ENGINE_API SkeletonData : public Resource
	{
		GUID						m_nGlobalSkeletonID;
		UINT						m_nBoneNum;
		std::vector<std::string>	m_arrBoneName;
		std::vector<UINT>			m_arrParentIndice;
		std::vector<D3DXVECTOR3>	m_arrScaleOS;
		std::vector<D3DXQUATERNION>	m_arrRotOS;
		std::vector<D3DXVECTOR3>	m_arrPosOS;

		void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonData" );
	};
}

#endif// __ExpSkeletonData_H__
