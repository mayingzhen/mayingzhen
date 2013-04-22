#ifndef  _SKELETON__H__
#define  _SKELETON__H__

//#include "Animation/SkeletonPose.h"



namespace ma
{
	struct SkeletonData;

	class SkeletonPose;
	class BoneSet;

	class ANIMATION_API Skeleton : public ISkeleton
	{
	public:
		Skeleton();

		~Skeleton();

		bool				InitWithData(const SkeletonData* pSkelData);

		const SkeletonData*	GetSkeletonData() {return m_pSkeleonData;}

		//bool				InitWithPSData(SkeletonData* pSkelData);

		const Matrix4x4&	GetBoneMatrixOSInv(BoneIndex nBoneID) {return m_arrRefPoseOSInv[nBoneID];}

		const SkeletonPose* GetResPose() {return m_refPose;} 

		UINT				GetBoneNumer() {return m_arrBoneName.size();}

		const char*			GetBoneNameByIndex(UINT uIndex) {return m_arrBoneName[uIndex].c_str();}

		BoneIndex			GetBoneIdByName(const char* pszBoneName);

		BoneIndex			GetParentIndice(BoneIndex nBoneID);

		BoneSet*			GetBoneSetByName(const char* pszBoneSetName);

		bool				IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID);

		void				InitUpLowerBoneSet(const char* pszSplitBone = "Bip01 Spine1",const char* pszUpBody = "UpBody", const char* pszLowerBody = "LowerBody");

	private:
		std::string m_name;

		std::vector<std::string> m_arrBoneName;
		std::vector<BoneIndex>	 m_arrParentInd;

		SkeletonPose*			 m_refPose;

		// Matrix4x4
		std::vector<Matrix4x4> m_arrRefPosePS;
		std::vector<Matrix4x4> m_arrRefPoseOS;
		std::vector<Matrix4x4> m_arrRefPoseOSInv;

		// ¹Ç÷À¼¯
		std::vector<BoneSet*>	m_arrBoneSet;

		const SkeletonData*		m_pSkeleonData;
	};
}



#endif