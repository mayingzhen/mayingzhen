#ifndef  _SKELETON__H__
#define  _SKELETON__H__

namespace ma
{
	struct SkeletonData;

	class SkeletonPose;
	class BoneSet;

	static const char* pszDefaultUpBody = "UpBody";
	static const char* pszDefaultLowerBody = "LowerBody";
	static const char* pszDefaultFullBody = "FullBody";
	static const char* pszDefaultEmptyBody = "EmptyBody";

	class Skeleton : public Resource
	{
		
	public:
		Skeleton();

		~Skeleton();

		const Matrix4&		GetBoneMatrixOSInv(BoneIndex nBoneID) {return m_arrRefPoseOSInv[nBoneID];}

		const SkeletonPose* GetResPose() const {return m_refPose;} 

		UINT				GetBoneNumer() const {return m_arrBoneName.size();} 

		const char*			GetBoneNameByIndex(UINT uIndex) const; 

		BoneIndex			GetBoneIdByName(const char* pszBoneName) const;

		BoneIndex			GetParentIndice(BoneIndex nBoneID) const;

		BoneSet*			GetBoneSetByName(const char* pszBoneSetName) const;

		void				AddBone(const char* pName,UINT nParentID,const Transform& tsfOS);

		bool				IsAncestorOf(BoneIndex nAncestorBoneID,BoneIndex nChildBoneID) const;

		void				InitUpLowerBoneSet(const char* pszSplitBone = "Bip01 Spine1",
			const char* pszUpBody = pszDefaultUpBody, const char* pszLowerBody = pszDefaultLowerBody,
			const char* pszFullBoyd = pszDefaultFullBody,const char* pzEmptyBody = pszDefaultEmptyBody);

		void				InitResPose();

		bool				SaveToFile(const char* pszFile);

	private:
	
		virtual bool    InitRes();

		void			ReadDataV1();
		void			ReadDataV0();

	private:
		std::vector<std::string>	m_arrBoneName;
		std::vector<BoneIndex>		m_arrParentIndice;
		std::vector<Transform>		m_arrTsfOS;

		SkeletonPose*				m_refPose;

		// Matrix4
		std::vector<Matrix4>		m_arrRefPoseOSInv;

		// ¹Ç÷À¼¯
		typedef std::vector< RefPtr<BoneSet> > VEC_BONSET;
		VEC_BONSET					m_arrBoneSet;
	};

	extern ResourceSystem<Skeleton>* g_pSkeletonManager;

	RefPtr<Skeleton> CreateSkeleton(const char* pszFile);
	RefPtr<Skeleton> CreateSkeleton();
}



#endif