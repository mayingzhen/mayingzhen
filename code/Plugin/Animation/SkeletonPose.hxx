#include "Animation/SkeletonPose.h"

namespace ma
{
	SkeletonPose* SkeletonPose::Clone() const
	{
		SkeletonPose* pCloneNodePos = new SkeletonPose();
		pCloneNodePos->m_bPSSynced = m_bOSSynced;
		pCloneNodePos->m_bOSSynced = m_bOSSynced;

		UINT nodeNumber = m_arrParentIndice.size();
 		pCloneNodePos->m_arrDirtyByte.resize(nodeNumber);
		pCloneNodePos->m_arrTSF_PS.resize(nodeNumber);
		pCloneNodePos->m_arrTSF_OS.resize(nodeNumber);
		pCloneNodePos->m_arrParentIndice.resize(nodeNumber);
 		for (UINT i = 0; i < m_arrDirtyByte.size();++i)
		{
			pCloneNodePos->m_arrDirtyByte[i] = m_arrDirtyByte[i];
			pCloneNodePos->m_arrTSF_PS[i] = m_arrTSF_PS[i];
			pCloneNodePos->m_arrTSF_OS[i] =	m_arrTSF_OS[i];
			pCloneNodePos->m_arrParentIndice[i] = m_arrParentIndice[i];
 		}
		return pCloneNodePos;
	}

	void SkeletonPose::InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice)
	{
		UINT uBoneNum = arrParentIndice.size();
		m_arrDirtyByte.resize(uBoneNum);
		m_arrParentIndice.resize(uBoneNum);
		m_arrTSF_OS.resize(uBoneNum);
		m_arrTSF_PS.resize(uBoneNum);

		for (UINT uBoneCnt = 0; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			m_arrDirtyByte[uBoneCnt].m_bOSDirty = false;
			m_arrDirtyByte[uBoneCnt].m_bPSDirty = false;
			m_arrParentIndice[uBoneCnt] = arrParentIndice[uBoneCnt];
			TransformSetIdentity(&m_arrTSF_OS[uBoneCnt]);
			TransformSetIdentity(&m_arrTSF_PS[uBoneCnt]);
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void SkeletonPose::InitParentSpace(const std::vector<NodeTransform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformPSAll(arrTSF_PS);
	}

	void SkeletonPose::InitObjectSpace(const std::vector<NodeTransform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformOSAll(arrTSF_OS);
	}

	// void SkeletonPose::InitLocalSpace(const std::vector<NodeTransform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice)
	// {
	// 
	// }
// 	void  TransformMulLocalScale(NodeTransform* pOut,const NodeTransform* pTSFA,const NodeTransform* pTSFB)
// 	{
// 		Vector3 vOPos = pTSFA->m_vPos*pTSFB->m_fScale;
// 		QuaternionTransformVector(&vOPos,&vOPos,&pTSFB->m_qRot);
// 		pOut->m_vPos = vOPos + pTSFB->m_vPos;
// 
// 		QuaternionMultiply(&pOut->m_qRot,&pTSFA->m_qRot,&pTSFB->m_qRot);
// 
// 		pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale; 
// 
// 		//Vec3Mul(&pOut->m_vLocalScale,&pTSFA->m_vLocalScale,&pTSFB->m_vLocalScale);
// 
// 	}

	void SkeletonPose::InitLocalSpace(const std::vector<NodeTransform>& arrTSF_LS,const SkeletonPose* pRefPose)
	{
		std::vector<NodeTransform> arrTSF_PS;
		const UINT nBoneNum = pRefPose->GetNodeNumber();
		arrTSF_PS.resize(nBoneNum);

		for (UINT nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			TransformMulLocalScale(&arrTSF_PS[nBoneCnt],&arrTSF_LS[nBoneCnt],&pRefPose->GetTransformPS(nBoneCnt));
		}
		SetTransformPSAll(arrTSF_PS);
	}

	void SkeletonPose::SetTransformPSAll(const std::vector<NodeTransform>& arrTSF_PS)
	{
		ASSERT(arrTSF_PS.size() == m_arrParentIndice.size());
		UINT uBoneNum = m_arrParentIndice.size();
		for (UINT uBoneCnt = 0; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			m_arrDirtyByte[uBoneCnt].m_bOSDirty = false;
			m_arrDirtyByte[uBoneCnt].m_bPSDirty = false;

			m_arrTSF_PS[uBoneCnt] = arrTSF_PS[uBoneCnt];

			BoneIndex nParentInd = m_arrParentIndice[uBoneCnt];
			if ( IsValidID(nParentInd) )
			{
				TransformMul(&m_arrTSF_OS[uBoneCnt],&arrTSF_PS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
			}	
			else 
			{
				m_arrTSF_OS[uBoneCnt] = arrTSF_PS[uBoneCnt];
			}
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void SkeletonPose::SetTransformOSAll(const std::vector<NodeTransform>& arrTSF_OS)
	{
		ASSERT(arrTSF_OS.size() == m_arrParentIndice.size());
		UINT uBoneNum = m_arrParentIndice.size();
		for (UINT uBoneCnt = 0; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			m_arrDirtyByte[uBoneCnt].m_bOSDirty = false;
			m_arrDirtyByte[uBoneCnt].m_bPSDirty = false;

			m_arrTSF_OS[uBoneCnt] = arrTSF_OS[uBoneCnt];

			BoneIndex nParentInd = m_arrParentIndice[uBoneCnt];
			if ( IsValidID(nParentInd) )
			{
				TransformInvMul(&m_arrTSF_PS[uBoneCnt],&arrTSF_OS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
			}	
			else 
			{
				m_arrTSF_PS[uBoneCnt] = arrTSF_OS[uBoneCnt];
			}
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}


	void SkeletonPose::SyncParentSpace() const
	{
		if (!m_bPSSynced)
		{
			for (UINT nBoneCnt = 0; nBoneCnt < m_arrParentIndice.size(); ++nBoneCnt)
			{
				if (m_arrDirtyByte[nBoneCnt].m_bPSDirty)
				{
					UpdateTransformPS(nBoneCnt);
				}
			}
			m_bPSSynced = true;
		}
	}

	void SkeletonPose::SyncObjectSpace() const
	{
		if (!m_bOSSynced)
		{
			for (UINT nBoneCnt = 0; nBoneCnt < m_arrParentIndice.size(); ++nBoneCnt)
			{
				if (m_arrDirtyByte[nBoneCnt].m_bOSDirty)
				{
					UpdateTransformOS(nBoneCnt);
				}
			}
			m_bOSSynced = true;
		}
	}

	void SkeletonPose::SetTransformPS(const NodeTransform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_PS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = true;

		m_bOSSynced = false;
	}

	void SkeletonPose::SetTransformOS(const NodeTransform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_OS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = true;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = false;

		m_bOSSynced = false;
	}

	const NodeTransform& SkeletonPose::GetTransformOS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
		{
			UpdateTransformOS(nBoneInd);
		}
		return m_arrTSF_OS[nBoneInd];
	}

	const NodeTransform& SkeletonPose::GetTransformPS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
		{
			UpdateTransformPS(nBoneInd);
		}
		return m_arrTSF_PS[nBoneInd];
	}

	void SkeletonPose::UpdateTransformPS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
		{
			ASSERT(!m_arrDirtyByte[nBoneInd].m_bOSDirty);

			BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
			if ( IsValidID(nParentInd) )
			{
				const NodeTransform& parentTSF = GetTransformOS(nParentInd);
				TransformInvMul(&m_arrTSF_PS[nBoneInd], &m_arrTSF_OS[nBoneInd], &parentTSF);
			}
			else
			{
				m_arrTSF_PS[nBoneInd] = m_arrTSF_OS[nBoneInd];
			}

			m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
		}
	}

	void SkeletonPose::UpdateTransformOS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
		{
			ASSERT(!m_arrDirtyByte[nBoneInd].m_bPSDirty);

			BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
			if ( IsValidID(nParentInd) )
			{
				const NodeTransform& patentTSF = GetTransformOS(nParentInd);
				TransformMul(&m_arrTSF_OS[nBoneInd], &m_arrTSF_PS[nBoneInd], &patentTSF);

				QuaternionNormalize(&m_arrTSF_OS[nBoneInd].m_qRot, &m_arrTSF_OS[nBoneInd].m_qRot);
			}
			else 
			{
				m_arrTSF_OS[nBoneInd] = m_arrTSF_PS[nBoneInd];
			}

			m_arrDirtyByte[nBoneInd].m_bOSDirty = false;
		}
	}

	void SkeletonPose::SyncAllChildPS(BoneIndex nAncestorInd) const
	{
		UINT uBoneNum = m_arrParentIndice.size();
		for (UINT uBoneCnt = nAncestorInd + 1; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			if ( IsAncestor(nAncestorInd, uBoneCnt) )
			{
				if (m_arrDirtyByte[uBoneCnt].m_bPSDirty)
				{
					GetTransformPS(uBoneCnt);
				}

				m_arrDirtyByte[uBoneCnt].m_bOSDirty = true;
			}
		}
	}

	bool SkeletonPose::IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const
	{
		BoneIndex nParentInd = m_arrParentIndice[nDescendantInd];
		while ( IsValidID(nParentInd) && nAncestorInd < nParentInd )
		{
			nParentInd = m_arrParentIndice[nParentInd];
		}
		return nParentInd == nAncestorInd;
	}

	void SkeletonPose::ApplyTransformLS(const NodeTransform* pTSF, BoneIndex nBoneInd)
	{
		NodeTransform tsf = GetTransformPS(nBoneInd);
		TransformMul(&tsf,pTSF,&tsf);
		SetTransformPS(&tsf,nBoneInd);
	}

}


