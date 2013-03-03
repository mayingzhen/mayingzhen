#include "Animation/NodePose.h"

namespace ma
{
	NodePose* NodePose::Clone() const
	{
		NodePose* pCloneNodePos = new NodePose();
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

	void NodePose::InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice)
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
			maTransformSetIdentity(&m_arrTSF_OS[uBoneCnt]);
			maTransformSetIdentity(&m_arrTSF_PS[uBoneCnt]);
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void NodePose::InitParentSpace(const std::vector<maNodeTransform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformPSAll(arrTSF_PS);
	}

	void NodePose::InitObjectSpace(const std::vector<maNodeTransform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice)
	{
		InitWithParentIndice(arrParentIndice);
		SetTransformOSAll(arrTSF_OS);
	}

	// void NodePose::InitLocalSpace(const std::vector<maNodeTransform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice)
	// {
	// 
	// }
	void  xmTransformMulLocalScale(maNodeTransform* pOut,const maNodeTransform* pTSFA,const maNodeTransform* pTSFB)
	{
		D3DXVECTOR3 vOPos = pTSFA->m_vPos*pTSFB->m_fScale;
		maQuaternionTransformVector(&vOPos,&vOPos,&pTSFB->m_qRot);
		pOut->m_vPos = vOPos + pTSFB->m_vPos;

		D3DXQuaternionMultiply(&pOut->m_qRot,&pTSFA->m_qRot,&pTSFB->m_qRot);

		pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale; 

		//xmVec3Mul(&pOut->m_vLocalScale,&pTSFA->m_vLocalScale,&pTSFB->m_vLocalScale);

	}

	void NodePose::InitLocalSpace(const std::vector<maNodeTransform>& arrTSF_LS,const NodePose* pRefPose)
	{
		//InitWithParentIndice(pRefPose->get);

		std::vector<maNodeTransform> arrTSF_PS;
		const UINT nBoneNum = pRefPose->GetNodeNumber();
		arrTSF_PS.resize(nBoneNum);
		m_arrDirtyByte.resize(nBoneNum);

		for (UINT nBoneCnt = 0; nBoneCnt < nBoneNum; ++nBoneCnt)
		{
			xmTransformMulLocalScale(&arrTSF_PS[nBoneCnt],&arrTSF_LS[nBoneCnt],&pRefPose->GetTransformPS(nBoneCnt));

		}
		SetTransformPSAll(arrTSF_PS);
	}

	void NodePose::SetTransformPSAll(const std::vector<maNodeTransform>& arrTSF_PS)
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
				maTransfromMul(&m_arrTSF_OS[uBoneCnt],&arrTSF_PS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
			}	
			else 
			{
				m_arrTSF_OS[uBoneCnt] = arrTSF_PS[uBoneCnt];
			}
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}

	void NodePose::SetTransformOSAll(const std::vector<maNodeTransform>& arrTSF_OS)
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
				maTransfromInvMul(&m_arrTSF_PS[uBoneCnt],&arrTSF_OS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
			}	
			else 
			{
				m_arrTSF_PS[uBoneCnt] = arrTSF_OS[uBoneCnt];
			}
		}

		m_bPSSynced = true;
		m_bOSSynced = true;
	}


	void NodePose::SyncParentSpace() const
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

	void NodePose::SyncObjectSpace() const
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

	void NodePose::SetTransformPS(const maNodeTransform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_PS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = true;

		m_bOSSynced = false;
	}

	void NodePose::SetTransformOS(const maNodeTransform* pTSF, BoneIndex nBoneInd)
	{
		SyncAllChildPS(nBoneInd);
		m_arrTSF_OS[nBoneInd] = *pTSF;
		m_arrDirtyByte[nBoneInd].m_bPSDirty = true;
		m_arrDirtyByte[nBoneInd].m_bOSDirty = false;

		m_bOSSynced = false;
	}

	const maNodeTransform& NodePose::GetTransformOS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
		{
			UpdateTransformOS(nBoneInd);
		}
		return m_arrTSF_OS[nBoneInd];
	}

	const maNodeTransform& NodePose::GetTransformPS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
		{
			UpdateTransformPS(nBoneInd);
		}
		return m_arrTSF_PS[nBoneInd];
	}

	void NodePose::UpdateTransformPS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
		{
			ASSERT(!m_arrDirtyByte[nBoneInd].m_bOSDirty);

			BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
			if ( IsValidID(nParentInd) )
			{
				const maNodeTransform& parentTSF = GetTransformOS(nParentInd);
				maTransfromInvMul(&m_arrTSF_PS[nBoneInd], &m_arrTSF_OS[nBoneInd], &parentTSF);
			}
			else
			{
				m_arrTSF_PS[nBoneInd] = m_arrTSF_OS[nBoneInd];
			}

			m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
		}
	}

	void NodePose::UpdateTransformOS(BoneIndex nBoneInd) const
	{
		if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
		{
			ASSERT(!m_arrDirtyByte[nBoneInd].m_bPSDirty);

			BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
			if ( IsValidID(nParentInd) )
			{
				const maNodeTransform& patentTSF = GetTransformOS(nParentInd);
				maTransfromMul(&m_arrTSF_OS[nBoneInd], &m_arrTSF_PS[nBoneInd], &patentTSF);

				D3DXQuaternionNormalize(&m_arrTSF_OS[nBoneInd].m_qRot, &m_arrTSF_OS[nBoneInd].m_qRot);
			}
			else 
			{
				m_arrTSF_OS[nBoneInd] = m_arrTSF_PS[nBoneInd];
			}

			m_arrDirtyByte[nBoneInd].m_bOSDirty = false;
		}
	}

	void NodePose::SyncAllChildPS(BoneIndex nAncestorInd) const
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

	bool NodePose::IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const
	{
		BoneIndex nParentInd = m_arrParentIndice[nDescendantInd];
		while ( IsValidID(nParentInd) && nAncestorInd < nParentInd )
		{
			nParentInd = m_arrParentIndice[nParentInd];
		}
		return nParentInd == nAncestorInd;
	}

	void NodePose::ApplyTransformLS(const maNodeTransform* pTSF, BoneIndex nBoneInd)
	{
		maNodeTransform tsf = GetTransformPS(nBoneInd);
		maTransfromMul(&tsf,pTSF,&tsf);
		SetTransformPS(&tsf,nBoneInd);
	}

}


