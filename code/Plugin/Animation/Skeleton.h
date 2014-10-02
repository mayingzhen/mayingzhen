#ifndef  _SKELETON__H__
#define  _SKELETON__H__

namespace ma
{
	struct SkeletonData;

	class SkeletonPose;
	class BoneSet;

	class ANIMATION_API Skeleton : public Resource
	{
		DECL_OBJECT(Skeleton)

	public:
		Skeleton();

		~Skeleton();

		virtual void		Serialize(Serializer& sl, const char* pszLable = "Skeleton");

		const Matrix4&		GetBoneMatrixOSInv(BoneIndex nBoneID) {return m_arrRefPoseOSInv[nBoneID];}

		const SkeletonPose* GetResPose() const {return m_refPose;} 

		UINT				GetBoneNumer() const {return m_arrBoneName.size();} 

		const char*			GetBoneNameByIndex(UINT uIndex) {return m_arrBoneName[uIndex].c_str();}

		BoneIndex			GetBoneIdByName(const char* pszBoneName);

		BoneIndex			GetParentIndice(BoneIndex nBoneID);

		BoneSet*			GetBoneSetByName(const char* pszBoneSetName);

		bool				IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID);

		void				InitUpLowerBoneSet(const char* pszSplitBone = "Bip01 Spine1",
			const char* pszUpBody = "UpBody", const char* pszLowerBody = "LowerBody",
			const char* pszFullBoyd = "FullBody",const char* pzEmptyBody = "EmptyBody");

		void				InitResPose();

	public:
		std::vector<std::string>	m_arrBoneName;
		std::vector<BoneIndex>		m_arrParentIndice;
		std::vector<Transform>		m_arrTsfOS;

	private:
		SkeletonPose*				m_refPose;

		// Matrix4
		std::vector<Matrix4>		m_arrRefPoseOSInv;

		// ¹Ç÷À¼¯
		std::vector<BoneSet*>		m_arrBoneSet;
	};
}



#endif