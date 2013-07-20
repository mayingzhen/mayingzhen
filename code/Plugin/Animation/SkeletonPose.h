#ifndef  _SkeletonPose__H__
#define  _SkeletonPose__H__

namespace ma
{
	class SkeletonPose
	{
	public:
		struct DirtyByte
		{
			bool m_bPSDirty;
			bool m_bOSDirty;
		};

		mutable bool m_bPSSynced;
		mutable bool m_bOSSynced;

		mutable std::vector<DirtyByte>		m_arrDirtyByte;
		mutable std::vector<NodeTransform> m_arrTSF_PS;
		mutable std::vector<NodeTransform>	m_arrTSF_OS;
		std::vector<BoneIndex>		m_arrParentIndice;

		UINT GetNodeNumber()const {return m_arrParentIndice.size();}

		SkeletonPose* Clone() const;

		void InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice);

		void InitParentSpace(const std::vector<NodeTransform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice);

		void InitObjectSpace(const std::vector<NodeTransform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice);

		void InitLocalSpace(const std::vector<NodeTransform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice);

		void InitLocalSpace(const std::vector<NodeTransform>& arrTSF_LS,const SkeletonPose* pRefPose);
		
		void SetTransformPSAll(const std::vector<NodeTransform>& arrTSF_PS);

		void SetTransformOSAll(const std::vector<NodeTransform>& arrTSF_OS);

		void ApplyTransformLS(const NodeTransform* pTSF, BoneIndex nBoneInd);

		void SyncParentSpace() const;

		void SyncObjectSpace() const;

		void SetTransformPS(const NodeTransform* pTSF, BoneIndex nBoneInd);

		void SetTransformOS(const NodeTransform* pTSF, BoneIndex nBoneInd);

		const NodeTransform& GetTransformOS(BoneIndex nBoneInd) const;

		const NodeTransform& GetTransformPS(BoneIndex nBoneInd) const;

	protected:

		void UpdateTransformPS(BoneIndex nBoneInd) const;

		void UpdateTransformOS(BoneIndex nBoneInd) const;

		void SyncAllChildPS(BoneIndex nAncestorInd) const;

		bool IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const;

	};

}


#endif