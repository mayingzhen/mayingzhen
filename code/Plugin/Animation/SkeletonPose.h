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


		UINT GetNodeNumber()const {return m_arrParentIndice.size();}

		SkeletonPose* Clone() const;

		void InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice);

		void InitParentSpace(const std::vector<Transform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice);

		void InitObjectSpace(const std::vector<Transform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice);

		void InitLocalSpace(const std::vector<Transform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice);

		void InitLocalSpace(const std::vector<Transform>& arrTSF_LS,const SkeletonPose* pRefPose);
		
		void SetTransformPSAll(const std::vector<Transform>& arrTSF_PS);

		void SetTransformOSAll(const std::vector<Transform>& arrTSF_OS);

		void ApplyTransformLS(const Transform* pTSF, BoneIndex nBoneInd);

		void SyncParentSpace() const;

		void SyncObjectSpace() const;

		void SetTransformPS(const Transform* pTSF, BoneIndex nBoneInd);

		void SetTransformOS(const Transform* pTSF, BoneIndex nBoneInd);

		const Transform& GetTransformOS(BoneIndex nBoneInd) const;

		const Transform& GetTransformPS(BoneIndex nBoneInd) const;

		void DebugRender(const Matrix4& matWS,bool bDrawBoneName = false,Skeleton* pSkeleton = NULL);

	protected:

		void UpdateTransformPS(BoneIndex nBoneInd) const;

		void UpdateTransformOS(BoneIndex nBoneInd) const;

		void SyncAllChildPS(BoneIndex nAncestorInd) const;

		bool IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const;

	private:
		mutable bool						m_bPSSynced;
		mutable bool						m_bOSSynced;

		mutable std::vector<DirtyByte>		m_arrDirtyByte;
		mutable std::vector<Transform>		m_arrTSF_PS;
		mutable std::vector<Transform>		m_arrTSF_OS;
		std::vector<BoneIndex>				m_arrParentIndice;
	};

}


#endif