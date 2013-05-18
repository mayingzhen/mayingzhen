#ifndef __SkeletonData_H__
#define __SkeletonData_H__

namespace ma
{

	struct ENGINE_API SkeletonHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		SkeletonHeader();
	};


	struct ENGINE_API SkeletonData : public Resource
	{
		maGUID						m_nGlobalSkeletonID;
		UINT						m_nBoneNum;
		std::vector<std::string>	m_arrBoneName;
		std::vector<UINT>			m_arrParentIndice;
		std::vector<Vector3>		m_arrScaleOS;
		std::vector<Quaternion>		m_arrRotOS;
		std::vector<Vector3>		m_arrPosOS;

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "SkeletonData");
	};
}

#endif// __ExpSkeletonData_H__
