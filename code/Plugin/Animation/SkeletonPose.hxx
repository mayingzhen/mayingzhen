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
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void SkeletonPose::InitParentSpace(const std::vector<Transform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformPSAll(arrTSF_PS);
	}

	void SkeletonPose::InitObjectSpace(const std::vector<Transform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformOSAll(arrTSF_OS);
	}


	void SkeletonPose::InitLocalSpace(const std::vector<Transform>& arrTSF_LS,const SkeletonPose* pRefPose)
	{
// 		std::vector<Transform> arrTSF_PS;
// 		const UINT nBoneNum = pRefPose->GetNodeNumber();
// 		arrTSF_PS.resize(nBoneNum);

		for (UINT nBoneCnt = 0; nBoneCnt < pRefPose->GetNodeNumber(); ++nBoneCnt)
		{
			TransformMul(&m_arrTSF_PS[nBoneCnt],&pRefPose->GetTransformPS(nBoneCnt),&arrTSF_LS[nBoneCnt]);
		}

		SetTransformPSAll(m_arrTSF_PS);
	}

	void SkeletonPose::SetTransformPSAll(const std::vector<Transform>& arrTSF_PS)
	{
		ASSERT(arrTSF_PS.size() == m_arrParentIndice.size());
		UINT uBoneNum = m_arrParentIndice.size();
		for (UINT uBoneCnt = 0; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			m_arrDirtyByte[uBoneCnt].m_bOSDirty = false;
			m_arrDirtyByte[uBoneCnt].m_bPSDirty = false;

			m_arrTSF_PS[uBoneCnt] = arrTSF_PS[uBoneCnt];

			BoneIndex nParentInd = m_arrParentIndice[uBoneCnt];
			if ( Math::IsValidID(nParentInd) )
			{
				TransformMul(&m_arrTSF_OS[uBoneCnt],&m_arrTSF_OS[nParentInd],&arrTSF_PS[uBoneCnt]);
			}	
			else 
			{
				m_arrTSF_OS[uBoneCnt] = arrTSF_PS[uBoneCnt];
			}

			m_arrTSF_OS[uBoneCnt].m_qRot.normalise();
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void SkeletonPose::SetTransformOSAll(const std::vector<Transform>& arrTSF_OS)
	{
		ASSERT(arrTSF_OS.size() == m_arrParentIndice.size());
		UINT uBoneNum = m_arrParentIndice.size();
		for (UINT uBoneCnt = 0; uBoneCnt < uBoneNum; ++uBoneCnt)
		{
			m_arrDirtyByte[uBoneCnt].m_bOSDirty = false;
			m_arrDirtyByte[uBoneCnt].m_bPSDirty = false;

			m_arrTSF_OS[uBoneCnt] = arrTSF_OS[uBoneCnt];

			BoneIndex nParentInd = m_arrParentIndice[uBoneCnt];
			if ( Math::IsValidID(nParentInd) )
			{
				Transform tsfInvParentTsfOS;
				TransformInverse(&tsfInvParentTsfOS,&m_arrTSF_OS[nParentInd]);

				TransformMul(&m_arrTSF_PS[uBoneCnt],&tsfInvParentTsfOS,&arrTSF_OS[uBoneCnt]);
			}	
			else 
			{
				m_arrTSF_PS[uBoneCnt] = arrTSF_OS[uBoneCnt];
			}

			m_arrTSF_OS[uBoneCnt].m_qRot.normalise();
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

	void SkeletonPose::SetTransformPS(const Transform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_PS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = true;

		m_bOSSynced = false;
	}

	void SkeletonPose::SetTransformOS(const Transform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_OS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = true;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = false;

		m_bOSSynced = false;
	}

	const Transform& SkeletonPose::GetTransformOS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
		{
			UpdateTransformOS(nBoneInd);
		}
		return m_arrTSF_OS[nBoneInd];
	}

	const Transform& SkeletonPose::GetTransformPS(BoneIndex nBoneInd) const
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
			if ( Math::IsValidID(nParentInd) )
			{
				const Transform& parentTsfOS = GetTransformOS(nParentInd);

				Transform tsfInvParentTsfOS;
				TransformInverse(&tsfInvParentTsfOS,&parentTsfOS);

				TransformMul(&m_arrTSF_PS[nBoneInd],&tsfInvParentTsfOS,&m_arrTSF_OS[nBoneInd]);
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
			if ( Math::IsValidID(nParentInd) )
			{
				const Transform& patentTsfOS = GetTransformOS(nParentInd);
				TransformMul(&m_arrTSF_OS[nBoneInd], &patentTsfOS, &m_arrTSF_PS[nBoneInd]);

				m_arrTSF_OS[nBoneInd].m_qRot.normalise();
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
		while ( Math::IsValidID(nParentInd) && nAncestorInd < nParentInd )
		{
			nParentInd = m_arrParentIndice[nParentInd];
		}
		return nParentInd == nAncestorInd;
	}

	void SkeletonPose::ApplyTransformLS(const Transform* pTSF, BoneIndex nBoneInd)
	{
		Transform tsf = GetTransformPS(nBoneInd);
		TransformMul(&tsf,&tsf,pTSF);
		SetTransformPS(&tsf,nBoneInd);
	}


	void SkeletonPose::DebugRender(const Matrix4& matWS,bool bDrawBoneName /*= false*/,Skeleton* pSkeleton/* = NULL*/) 
	{
		Transform tsfWS;
		TransformFromMatrix(&tsfWS,&matWS);
		LineRender::DrawTransform(tsfWS);

		//Rectangle rec = GetRenderSystem()->GetViewPort();

		UINT nBoneNumeber = m_arrParentIndice.size();
		for (UINT i = 0; i < nBoneNumeber; ++i)
		{
			Transform boneTsfWS;
			TransformMul(&boneTsfWS,&tsfWS,&this->GetTransformOS(i));
			LineRender::DrawTransform(boneTsfWS);

			Vector3 vP0 = matWS * this->GetTransformOS(i).m_vPos;

// 			if (bDrawBoneName)
// 			{
// 				Vector3 vPos = GetRenderSystem()->GetMainCamera()->GetMatViewProj() * vP0; 
// 				float srceenX = (vPos.x + 1.0f) * 0.5f * rec.width;
// 				float srceenY = (1.0f - vPos.y) * 0.5f * rec.height;
// 				const char* pBoneName = pSkeleton->GetBoneNameByIndex(i);
// 				GetStringRender()->DrawScreenString(pBoneName,srceenX,srceenY,ColourValue::Red);
// 			}
		
			BoneIndex parentID = m_arrParentIndice[i];
			if ( Math::IsValidID<BoneIndex>(parentID) )
			{
				Vector3 vP1 = matWS * this->GetTransformOS(parentID).m_vPos;

				LineRender::DrawLine(vP0,vP1,ColourValue::Red);
			}
		}
	}
}


