#ifndef  _SKELETON__H__
#define  _SKELETON__H__

//#include "Animation/NodePose.h"



namespace ma
{
	struct SkeletonData;

	class NodePose;
	class BoneSet;

	//typedef std::vector<BoneIndex> BoneSet;

	class ANIMATION_API Skeleton
	{
	public:
		Skeleton();

		~Skeleton();

		//bool Load(const char* pszPath);
		bool InitWithData(const SkeletonData& skelData);

		bool InitWithPSData(const SkeletonData& skelData);

		const D3DXMATRIX& GetBoneMatrixOSInv(BoneIndex nBoneID) {return m_arrRefPoseOSInv[nBoneID];}

		const NodePose* GetResPose() {return m_refPose;} 

		UINT GetBoneNumer() {return m_arrBoneName.size();}

		const char* GetBoneNameByIndex(UINT uIndex) {return m_arrBoneName[uIndex].c_str();}

		BoneIndex GetBoneIdByName(const char* pszBoneName);

		BoneIndex GetParentIndice(BoneIndex nBoneID);

		BoneSet* GetBoneSetByName(const char* pszBoneSetName);

		bool IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID);

		void InitUpLowerBoneSet(const char* pszSplitBone = "Bip01 Spine1",const char* pszUpBody = "UpBody", const char* pszLowerBody = "LowerBody");

	private:
		std::string m_name;

		std::vector<std::string> m_arrBoneName;
		std::vector<BoneIndex>	 m_arrParentInd;

		NodePose*				 m_refPose;

		// D3DXMATRIX
		std::vector<D3DXMATRIX> m_arrRefPosePS;
		std::vector<D3DXMATRIX> m_arrRefPoseOS;
		std::vector<D3DXMATRIX> m_arrRefPoseOSInv;

		// ¹Ç÷À¼¯
		std::vector<BoneSet*> m_arrBoneSet;
	};
}



#endif