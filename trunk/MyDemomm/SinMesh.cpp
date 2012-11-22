#include "public.h"
#include "sinmesh.h"
#include "Texture.h"
#include "TextureMngr.h"
#include "effect_file.h"
#include "GameApp.h"
#include "Camera.h"

#define F_EPS 1.0e-6f
#define F_MAX 1.0e+6f


void DecomposeFrame(float frame,const std::vector<UINT>& vkeyFrame,UINT& key0,UINT& key1,float& factor)
{
	//ASSERT(vkeyFrame.size() > 0);
	frame = Clamp(frame, (float)vkeyFrame.back());
	std::vector<UINT>::const_iterator iter = std::lower_bound(vkeyFrame.begin(), vkeyFrame.end(), (UINT)(frame + 1.0f));
	if ( iter != vkeyFrame.end() )
	{
		key1 = iter - vkeyFrame.begin();
		key0 = key1 > 0 ? key1 - 1 : key1;
		factor = key0 < key1 ? (frame - vkeyFrame[key0]) / (vkeyFrame[key1] - vkeyFrame[key0]) : 0.0f;
	}
	else
	{
		key0 = (UINT)vkeyFrame.size() - 1;
		key1 = key0;
		factor = 0.0f;
	}
}

void QuaternionTransformVector(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXQUATERNION* pQuat)
{
	D3DXVECTOR3 uv, uuv;
	D3DXVECTOR3 qvec(pQuat->x, pQuat->y, pQuat->z);
	D3DXVec3Cross(&uv, &qvec, pV);
	D3DXVec3Cross(&uuv, &qvec, &uv);
	uv *= (2.0f * pQuat->w);
	uuv *= 2.0f;
	*pOut = *pV + uv + uuv;
}

void QuaternionMad(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float fWeight)
{
	if ( D3DXQuaternionDot(pQ1,pQ2) >= 0.0f )
	{
		*pOut = *pQ1 + *pQ2 * fWeight;
	}
	else
	{
		*pOut = *pQ1 - *pQ2 * fWeight;
	}
} 


void TransformSetIdentity(CNodeTransform* pTSF)
{
	memset( &pTSF->m_vPos, 0, sizeof(D3DXVECTOR3) );
	D3DXQuaternionIdentity(&pTSF->m_qRot);
	pTSF->m_fScale = 1.0f;
}

void TransfromMul(CNodeTransform* pOut, const CNodeTransform* pTSFA, const CNodeTransform* pTSFB)
{
	D3DXVECTOR3 vOpos = pTSFA->m_vPos * pTSFB->m_fScale;
	QuaternionTransformVector(&vOpos, &vOpos, &pTSFB->m_qRot);
	pOut->m_vPos = vOpos + pTSFB->m_vPos;

	D3DXQuaternionMultiply(&pOut->m_qRot, &pTSFA->m_qRot, &pTSFB->m_qRot);
	
	pOut->m_fScale = pTSFA->m_fScale * pTSFB->m_fScale;
}

void TransformInverse(CNodeTransform* pOut, const CNodeTransform* pTSF)
{
	float fScaleInv;

	fScaleInv = pTSF->m_fScale > F_EPS ? 1.0f / pTSF->m_fScale : 1.0f;

	D3DXQuaternionInverse(&pOut->m_qRot,&pTSF->m_qRot);

	pOut->m_vPos = pTSF->m_vPos * fScaleInv;
	QuaternionTransformVector(&pOut->m_vPos, &pOut->m_vPos, &pOut->m_qRot);
	pOut->m_vPos = -pOut->m_vPos;

	pOut->m_fScale = fScaleInv;
}

void TransfromInvMul(CNodeTransform* pOut, const CNodeTransform* pTSFA, const CNodeTransform* pTSFB)
{
	CNodeTransform tsfInv;
	TransformInverse(&tsfInv, pTSFB);
	TransfromMul(pOut, pTSFA, &tsfInv);
}

void MatrixFromTransform(D3DXMATRIX* pMat, const CNodeTransform* pTSF)
{
	D3DXQUATERNION qRot;
	D3DXQuaternionNormalize(&qRot, &pTSF->m_qRot);
	
	D3DXVECTOR3 vScale(pTSF->m_fScale, pTSF->m_fScale, pTSF->m_fScale);

	D3DXMatrixTransformation(pMat, NULL, NULL, &vScale, NULL, &qRot, &pTSF->m_vPos);
}

template<class T> 
bool IsValidID(T ind)
{
	return ind != (T)(-1);
}

template<class T> 
bool IsInValidID(T ind)
{
	return ind == (T)(-1);
}


void CNodePose::InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice)
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

void CNodePose::InitParentSpace(const std::vector<CNodeTransform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice)
{
	InitWithParentIndice(arrParentIndice);
	SetTransformPSAll(arrTSF_PS);
}

void CNodePose::InitObjectSpace(const std::vector<CNodeTransform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice)
{
	InitWithParentIndice(arrParentIndice);
	SetTransformOSAll(arrTSF_OS);
}

// void CNodePose::InitLocalSpace(const std::vector<CNodeTransform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice)
// {
// 
// }

void CNodePose::SetTransformPSAll(const std::vector<CNodeTransform>& arrTSF_PS)
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
			TransfromMul(&m_arrTSF_OS[uBoneCnt],&arrTSF_PS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
		}	
		else 
		{
			m_arrTSF_OS[uBoneCnt] = arrTSF_PS[uBoneCnt];
		}
	}

	m_bPSSynced = true;
	m_bOSSynced = true;
}

void CNodePose::SetTransformOSAll(const std::vector<CNodeTransform>& arrTSF_OS)
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
			TransfromInvMul(&m_arrTSF_PS[uBoneCnt],&arrTSF_OS[uBoneCnt],&m_arrTSF_OS[nParentInd]);
		}	
		else 
		{
			m_arrTSF_PS[uBoneCnt] = arrTSF_OS[uBoneCnt];
		}
	}

	m_bPSSynced = true;
	m_bOSSynced = true;
}


void CNodePose::SyncParentSpace() const
{

}

void CNodePose::SyncObjectSpace() const
{

}

void CNodePose::SetTransformPS(const CNodeTransform* pTSF, BoneIndex nBoneInd)
{
	SyncAllChildPS(nBoneInd);
	m_arrTSF_PS[nBoneInd] = *pTSF;
	m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
	m_arrDirtyByte[nBoneInd].m_bOSDirty = true;

	m_bOSSynced = false;
}

void CNodePose::SetTransformOS(const CNodeTransform* pTSF, BoneIndex nBoneInd)
{
	SyncAllChildPS(nBoneInd);
	m_arrTSF_OS[nBoneInd] = *pTSF;
	m_arrDirtyByte[nBoneInd].m_bPSDirty = true;
	m_arrDirtyByte[nBoneInd].m_bOSDirty = false;

	m_bOSSynced = false;
}

const CNodeTransform& CNodePose::GetTransformOS(BoneIndex nBoneInd) const
{
	if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
	{
		UpdateTransformOS(nBoneInd);
	}
	return m_arrTSF_OS[nBoneInd];
}

const CNodeTransform& CNodePose::GetTransformPS(BoneIndex nBoneInd) const
{
	if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
	{
		UpdateTransformPS(nBoneInd);
	}
	return m_arrTSF_PS[nBoneInd];
}

void CNodePose::UpdateTransformPS(BoneIndex nBoneInd) const
{
	if (m_arrDirtyByte[nBoneInd].m_bPSDirty)
	{
		ASSERT(!m_arrDirtyByte[nBoneInd].m_bOSDirty);
		
		BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
		if ( IsValidID(nParentInd) )
		{
			const CNodeTransform& parentTSF = GetTransformOS(nParentInd);
			TransfromInvMul(&m_arrTSF_PS[nBoneInd], &m_arrTSF_OS[nBoneInd], &parentTSF);
		}
		else
		{
			m_arrTSF_PS[nBoneInd] = m_arrTSF_OS[nBoneInd];
		}

		m_arrDirtyByte[nBoneInd].m_bPSDirty = false;
	}
}
				
void CNodePose::UpdateTransformOS(BoneIndex nBoneInd) const
{
	if (m_arrDirtyByte[nBoneInd].m_bOSDirty)
	{
		ASSERT(!m_arrDirtyByte[nBoneInd].m_bPSDirty);

		BoneIndex nParentInd = m_arrParentIndice[nBoneInd];
		if ( IsValidID(nParentInd) )
		{
			const CNodeTransform& patentTSF = GetTransformOS(nParentInd);
			TransfromMul(&m_arrTSF_OS[nBoneInd], &m_arrTSF_PS[nBoneInd], &patentTSF);

			D3DXQuaternionNormalize(&m_arrTSF_OS[nBoneInd].m_qRot, &m_arrTSF_OS[nBoneInd].m_qRot);
		}
		else 
		{
			m_arrTSF_OS[nBoneInd] = m_arrTSF_PS[nBoneInd];
		}

		m_arrDirtyByte[nBoneInd].m_bOSDirty = false;
	}
}

void CNodePose::SyncAllChildPS(BoneIndex nAncestorInd) const
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

bool CNodePose::IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const
{
	BoneIndex nParentInd = m_arrParentIndice[nDescendantInd];
	while ( IsValidID(nParentInd) && nAncestorInd < nParentInd )
	{
		nParentInd = m_arrParentIndice[nParentInd];
	}
	return nParentInd == nAncestorInd;
}


void CSkeleton::Init(const std::vector<std::string>& arrBoneName,
					 const std::vector<BoneIndex>& arrParentInd,
					 const std::vector<CNodeTransform>& arrNodeOS)
{
	UINT uBoneNum = arrBoneName.size();

	m_arrBoneName = arrBoneName;
	m_arrParentInd = arrParentInd;
	
	m_refPose.InitObjectSpace(arrNodeOS, arrParentInd);
	
	m_arrRefPosePS.resize(uBoneNum);
	m_arrRefPoseOS.resize(uBoneNum);
	m_arrRefPoseOSInv.resize(uBoneNum);

	for (UINT uBoneCunt = 0; uBoneCunt < uBoneNum; ++uBoneCunt)
	{
		MatrixFromTransform( &m_arrRefPoseOS[uBoneCunt], &m_refPose.GetTransformOS(uBoneCunt) );
		MatrixFromTransform( &m_arrRefPosePS[uBoneCunt], &m_refPose.GetTransformPS(uBoneCunt) );
		D3DXMatrixInverse( &m_arrRefPoseOSInv[uBoneCunt], NULL, &m_arrRefPoseOS[uBoneCunt] );
	}
}	

const D3DXMATRIX& CSkeleton::GetBoneMatrixOSInv(BoneIndex nBoneID)
{
	return m_arrRefPoseOSInv[nBoneID];
}

void CAnimatedSkeleton::ComputeSingleBoneSkinMatrix(D3DXMATRIX* mat, UINT nBoneID)
{
	MatrixFromTransform( mat, &m_pose.GetTransformOS(nBoneID) );
	D3DXMatrixMultiply( mat, &m_pSkel->GetBoneMatrixOSInv(nBoneID), mat );
}

void CAnimation::SampleAndAddSingleTrackByFrame(CNodeTransform* pTSF, BoneIndex nTrackID, float fWeight, float fFrame) const
{
	CNodeTransform tsfLS;
	SampleSingleTrackByFrame(&tsfLS,nTrackID,fFrame);

	pTSF->m_fScale += tsfLS.m_fScale * fWeight;

	QuaternionMad(&pTSF->m_qRot, &pTSF->m_qRot, &tsfLS.m_qRot, fWeight);
	
	pTSF->m_vPos += tsfLS.m_vPos * fWeight;
}

void CAnimation::SampleSingleTrackByFrame(CNodeTransform* pTSF, BoneIndex nTrackID, float fFrame) const
{
	D3DXVECTOR3 vScale;
	m_pRawTracks->m_scale[nTrackID]->SampleFrame(fFrame,vScale);
	pTSF->m_fScale = ( abs(vScale.x) + abs(vScale.y) + abs(vScale.z) ) / 3.0f;

	m_pRawTracks->m_rot[nTrackID]->SampleFrame(fFrame,pTSF->m_qRot);
	m_pRawTracks->m_pos[nTrackID]->SampleFrame(fFrame,pTSF->m_vPos);
}

float CAnimation::GetFrameRate()
{
	return 30.0f;
}

UINT CAnimation::GetTotalFrame()
{
	return m_nFrameNumber;
}


CAnimationInst::CAnimationInst(CAnimation* pAnim)
{
	m_fLocalFrame = 0.0f;
	m_fPlaySpeed = 1.0f;
	m_playbackMode = PLAYBACK_LOOP;
	m_playerStatus = PLAYER_PLAYING;

	m_pAnimtion = pAnim;
}

CAnimationInst::~CAnimationInst()
{

}


void CAnimationInst::WrapLocalFrame()
{
	if (m_fLocalFrame > m_pAnimtion->GetTotalFrame())
	{
		if (m_playbackMode == PLAYBACK_LOOP)
		{
			m_fLocalFrame = 0.0f;
		}
		else
		{
			m_playerStatus = PLAYER_STOP;
		}
	}
}

void CAnimationInst::Instantiate(CSkeletonContext* pSkelContext, CBoneSet* pBoneSet)
{
	
}

void CAnimationInst::AdvanceTime(float fTimeElapsed)
{
	if (PLAYER_PLAYING == m_playerStatus)
	{
		m_fLocalFrame += fTimeElapsed * m_pAnimtion->GetFrameRate() * m_fPlaySpeed;
		WrapLocalFrame();
	}
}

void CAnimationInst::EvaluateAnimation(CAnimEvalContext* pContext, float fWeight, CBoneSet* pBoneSet)
{
	for (UINT uCnt = 0; uCnt < pBoneSet->m_arrBone.size(); ++uCnt)
	{
		BoneIndex nBoneInd = pBoneSet->m_arrBone[nBoneInd];
		if ( IsInValidID(nBoneInd) )
			continue;

		m_pAnimtion->SampleAndAddSingleTrackByFrame(&pContext->m_arrTSFLS[nBoneInd], nBoneInd,fWeight,m_fLocalFrame);
		pContext->m_arrAnimWeight[nBoneInd] += fWeight;
	}

	pContext->m_bLSAnimVaild = true;
}


bool CClipNode::Instantiate(CSkeletonContext* pSkelContext)
{
	m_pAnimInst->Instantiate(pSkelContext,m_pBoneSet);
	return true;
}

void CClipNode::AdvanceTime(float fTimeElapsed)
{
	m_pAnimInst->AdvanceTime(fTimeElapsed);
}

void CClipNode::EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight)
{
	m_pAnimInst->EvaluateAnimation(pAnimContext,fWeight,m_pBoneSet);
}



bool CLayerNode::Instantiate(CSkeletonContext* pSkelContext)
{
	for (UINT i = 0; i < m_arrChildLayer.size(); ++i)
	{
		m_arrChildLayer[i]->Instantiate(pSkelContext);
	}

	return true;
}

void CLayerNode::AdvanceTime(float fTimeElapsed)
{
	for (UINT i = 0; i < m_arrChildLayer.size(); ++i)
	{
		m_arrChildLayer[i]->AdvanceTime(fTimeElapsed);
	}	
}

void CLayerNode::EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight)
{
	for (UINT i = 0; i < m_arrChildLayer.size(); ++i)
	{
		m_arrChildLayer[i]->EvaluateAnimation(pAnimContext, fWeight/*1.0f*/);
	}	
}





CBlendNode::CBlendNode()
{
	m_fWeight = 0.0f;
}

bool CBlendNode::Instantiate(CSkeletonContext* pSkelContext)
{
	bool bInstOk = true;
	bInstOk &= m_pSrcNode->Instantiate(pSkelContext);
	bInstOk &= m_pDestNode->Instantiate(pSkelContext);
	return bInstOk;
}

void CBlendNode::AdvanceTime(float fTimeElapsed)
{
	m_pSrcNode->AdvanceTime(fTimeElapsed);
	m_pDestNode->AdvanceTime(fTimeElapsed);
}

void CBlendNode::EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight)
{	
	float fSrcWeight = fWeight * (1.0f - m_fWeight);
	float fDestWeight = fWeight * m_fWeight;
	m_pSrcNode->EvaluateAnimation(pAnimContext,fSrcWeight);
	m_pDestNode->EvaluateAnimation(pAnimContext,fDestWeight);
}


//-----------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = lstrlen(Name) + 1;
		*pNewName = new TCHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength*sizeof(TCHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Desc: 创建框架, 仅仅是分配内存和初始化,还没有对其成员赋予合适的值
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED *pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;
    e_Exit:
	delete pFrame;
	return hr;
}

//-----------------------------------------------------------------------------
// Desc: 在这里是调用了成员函数 GenerateSkinnedMesh(pMeshContainer);
//       是在这里加载了蒙皮信息
//-----------------------------------------------------------------------------
LRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCTSTR Name, 
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials, 
	CONST D3DXEFFECTINSTANCE *pEffectInstances, 
	DWORD NumMaterials, 
	CONST DWORD *pAdjacency, 
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if (pMeshData->Type != D3DXMESHTYPE_MESH)  //类型只能是D3DXMESHTYPE_MESH
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;        

	pMesh->GetDevice(&pd3dDevice);////////////////////////////////
	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if ((pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
			pMesh->GetFVF() | D3DFVF_NORMAL, 
			pd3dDevice, &pMeshContainer->MeshData.pMesh );
		if (FAILED(hr))
			goto e_Exit;

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals( pMesh, NULL );
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new CTexture*[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
	memset(pMeshContainer->ppTextures, 0, sizeof(CTexture*) * pMeshContainer->NumMaterials);

	// if materials provided, copy them
	if (NumMaterials > 0)            
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//  TCHAR strTexturePath[MAX_PATH] = _T("");
				//   DXUtil_FindMediaFileCb( strTexturePath, sizeof(strTexturePath), pMeshContainer->pMaterials[iMaterial].pTextureFilename );

				//if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, 
				//	pMeshContainer->pMaterials[iMaterial].pTextureFilename, 
				//	&pMeshContainer->ppTextures[iMaterial] ) ) )
				//{
				//	int res = GetLastError();
				//	pMeshContainer->ppTextures[iMaterial] = NULL;
				//}
				pMeshContainer->ppTextures[iMaterial] = 
					(CTexture*)TextureMngr()->Query(pMeshContainer->pMaterials[iMaterial].pTextureFilename,true,NULL);

				// don't remember a pointer into the dynamic memory, just forget the name after loading
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else // if no materials provided, use a default one
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// if there is skinning information, save off the required data and then setup for HW skinning
	if (pSkinInfo != NULL)
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		// get each of the bone offset matrices so that we don't need to get them later
		//把SkinInfo中的各骨骼的偏移矩阵保存到pMeshContainer->pBoneOffsetMatrices中
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}


		// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
		hr = GenerateSkinnedMesh(pMeshContainer);
		if (FAILED(hr))
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;
    e_Exit:
	SAFE_RELEASE(pd3dDevice);

	// call Destroy function to properly clean up the memory allocated 
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	SAFE_DELETE_ARRAY( pFrameToFree->Name );
	SAFE_DELETE( pFrameToFree );
	return S_OK; 
}

//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

	// release all the allocated textures
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
		}
	}

	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->pOrigMesh );
	SAFE_DELETE( pMeshContainer );
	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: GenerateSkinnedMesh()
// Desc: Called either by CreateMeshContainer when loading a skin mesh, or when 
//       changing methods.  This function uses the pSkinInfo of the mesh 
//       container to generate the desired drawable mesh and bone combination 
//       table.
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer)
{
	HRESULT hr = S_OK;

	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	D3DCAPS9 d3dCaps;
	if (g_pD3DDevice)
		g_pD3DDevice->GetDeviceCaps(&d3dCaps);

	//由于要重定义pMeshContainer->MeshData.pMesh，所以先释放原pMesh,原Mesh已被保存
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	//DWORD NumMaxFaceInfl;
	//DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

	//LPDIRECT3DINDEXBUFFER9 pIB;
	//hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
	//if (FAILED(hr))
	//	return hr;

	//hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl);
	//pIB->Release();
	//if (FAILED(hr))
	//	return hr;

	// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
	// can be handled
	//NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

// 	if (d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
// 	{
// 		// HW does not support indexed vertex blending. Use SW instead
// 		pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
// 		pMeshContainer->UseSoftwareVP = true;
// 		Flags |= D3DXMESH_SYSTEMMEM;
// 	}
// 	else
// 	{
// 		//pMeshContainer->NumPaletteEntries这个数值，
// 		//确定了一趟DrawSubset绘制所用到的矩阵个数，
// 		//个数越多，在一趟绘制中就可以纳入的更多顶点。
// 		//所以，当我们减少pMeshContainer->NumPaletteEntries这个数值时，
// 		//pMeshContainer->NumAttributeGroups数值就会增加。
// 		//也就是说，一趟绘制中所允许涉及的骨骼数越少，
// 		//那么子集的数量NumAttributeGroups就会增加，需要多绘几趟。
// 
// 		pMeshContainer->NumPaletteEntries = min( ( d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2, 
// 			pMeshContainer->pSkinInfo->GetNumBones() );
// 		pMeshContainer->UseSoftwareVP = false;
// 		Flags |= D3DXMESH_MANAGED;
// 	}

	// Get palette size
	// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
	// (96 - 9) /3 i.e. Maximum constant count - used constants 
	UINT MaxMatrices = 26;
	pMeshContainer->NumPaletteEntries = min( MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones() );

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	if( d3dCaps.VertexShaderVersion >= D3DVS_VERSION( 1, 1 ) )
	{
		pMeshContainer->UseSoftwareVP = false;
		Flags |= D3DXMESH_MANAGED;
	}
	else
	{
		pMeshContainer->UseSoftwareVP = true;
		//g_bUseSoftwareVP = true;
		Flags |= D3DXMESH_SYSTEMMEM;
	}

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
		pMeshContainer->pOrigMesh,
		Flags, 
		pMeshContainer->NumPaletteEntries, 
		pMeshContainer->pAdjacency, 
		NULL, NULL, NULL, 
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups, 
		&pMeshContainer->pBoneCombinationBuf, 
		&pMeshContainer->MeshData.pMesh);
	if (FAILED(hr))
		return hr;

	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH pMesh;
		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, g_pD3DDevice, &pMesh);
		if (!FAILED(hr))
		{
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMesh;
			pMesh = NULL;
		}
	}


	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))
		return hr;

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff)
	{
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))
		return hr;


	return hr;
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkinMesh::CSkinMesh()
{
	m_pd3dDevice = g_pD3DDevice;
	m_bMoving = true;
	m_pAnimController = NULL;
	m_pFrameRoot = NULL;
	m_pBoneMatrices = NULL;
	m_NumBoneMatricesMax = 0;
}


CSkinMesh::~CSkinMesh()
{
	CAllocateHierarchy Alloc(this);
	D3DXFrameDestroy(m_pFrameRoot, &Alloc);

	SAFE_RELEASE(m_pAnimController);
}

//-----------------------------------------------------------------------------
// Desc: 从文件加载蒙皮网格模型
//-----------------------------------------------------------------------------
HRESULT CSkinMesh::LoadFromXFile(const char *strFileName)
{
	D3DXLoadMeshFromX

	HRESULT hr = S_OK;

	CAllocateHierarchy Alloc(this);

	//从.X文件加载层次框架和动画数据
	hr = D3DXLoadMeshHierarchyFromX(strFileName, 
		D3DXMESH_MANAGED, g_pD3DDevice, &Alloc, 
		NULL, &m_pFrameRoot, &m_pAnimController);
	if (FAILED(hr))
	{
		int res = GetLastError();
		assert(false && ".x文件加载错误");
		return hr;
	}
	
	/// 
	UINT uAnimSetNum = m_pAnimController->GetNumAnimationSets();
	for (UINT i = 0; i < uAnimSetNum; ++i)
	{
		LPD3DXANIMATIONSET pAnimSet = NULL;
		m_pAnimController->GetAnimationSet(i, &pAnimSet);
		if (pAnimSet == NULL)
			continue;

		UINT uAnimNim = pAnimSet->GetNumAnimations();


	}



	
	//建立各级框架的组合变换矩阵
	hr = SetupBoneMatrixPointers(m_pFrameRoot);
	if (FAILED(hr))
	{
		::MessageBox(0,"SetupBoneMatrixPointers::failed",0,0);
		return hr;
	}

	 V( D3DXFrameCalculateBoundingSphere( m_pFrameRoot, &m_vObjectCenter, &m_fObjectRadius ) );

	CalculateBondingBox(m_pFrameRoot);

	return S_OK;
}


//--------------------------------------------------------------------------
// Desc: 仅在LoadFromXFile中调用。调用子函数SetupBoneMatrixPointersOnMesh()
//       安置好各级框架(实际上是各个骨骼)的组合变换矩阵。
// 注意: 在这里其实并没有计算出各个骨骼的组合变换矩阵，只是为每个矩阵开辟了相应
//       的存储空间，真正的计算是在函数SkinnedMesh::UpdateFrameMatrices()中完成的。
//---------------------------------------------------------------------------
HRESULT CSkinMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{	
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling != NULL) //邻居节点
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL) //子节点
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Desc: 仅在SetupBoneMatrixPointers()中被调用，设置每个骨骼的组合变换矩阵
//-----------------------------------------------------------------------------
HRESULT CSkinMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;
	D3DXFRAME_DERIVED *pFrame;

	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	//蒙皮动画，则设置骨骼的变换矩阵
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];	//创建骨骼
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( m_pFrameRoot, 
				pMeshContainer->pSkinInfo->GetBoneName(iBone) );

			if (pFrame == NULL)
				return E_FAIL;
			//初始化矩阵
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}

	return S_OK;
}


void CSkinMesh::Render(LPD3DXMATRIX transWorld)
{
	UpdateFrameMatrices(m_pFrameRoot,transWorld);

	DrawFrame(m_pFrameRoot);	
}


void CSkinMesh::Upadte()
{
	if( 0.00f == g_ElapsedTime )    
		return;	

	if( m_pAnimController != NULL )
		m_pAnimController->AdvanceTime( g_ElapsedTime, NULL );

}

//-----------------------------------------------------------------------------
// Desc: 绘制框架.
//       先用SkinnedMesh::DrawMeshContainer()绘制一个LPD3DXMESHCONTAINER类型
//       的变量pMeshContainer.然后递归绘制同一级框架和子一级框架。
//-----------------------------------------------------------------------------
void CSkinMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

//-----------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: Called to render a mesh in the hierarchy
//-----------------------------------------------------------------------------
void CSkinMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	//UINT iMaterial;
	//UINT NumBlend;
	UINT iAttrib;
	//DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	UINT iPaletteEntry;
	D3DXMATRIXA16 matTemp;

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		D3DXMATRIX matrixArray[50];

		// for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			D3DXMATRIX matView,matProj,matViewProj;
			g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
			g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
			//matViewProj = matView * matProj;	

			// first calculate all the world matrices
			for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
			{
				iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				if (iMatrixIndex != UINT_MAX)
				{
					D3DXMATRIX matTemp;
					D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], 
						pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
					D3DXMatrixMultiply( &matrixArray[iPaletteEntry], &matTemp, &matView);
				}
			}

			if ( g_pCurCEffect && g_pCurCEffect->effect() )
			{
				V( g_pCurCEffect->effect()->SetMatrixArray( "mWorldMatrixArray", matrixArray, pMeshContainer->NumPaletteEntries ) );
				V( g_pCurCEffect->effect()->SetMatrix( "viewprojection", &matProj ) );
				V( g_pCurCEffect->effect()->SetInt( "CurNumBones", pMeshContainer->NumInfl - 1 ) );
			}

			// setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
			//g_pD3DDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
			CTexture *texTemp  = pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId];
			if ( texTemp && texTemp->IsValid() )
			{
				if ( g_pCurCEffect && g_pCurCEffect->effect() )
					g_pCurCEffect->SetTexture( "g_TextureSrcDiffuse", texTemp->GetD3DTexture() );
				else
					g_pD3DDevice->SetTexture( 0, texTemp->GetD3DTexture() );
			}

			if (g_pCurCEffect)
				g_pCurCEffect->CommitChanges(); 

			// finally draw the subset with the current world matrix palette and material state
			pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );	

		}
	}
	else  // standard mesh, just draw it after setting material properties
	{
 		ID3DXMesh* mesh = pMeshContainer->MeshData.pMesh;
		IDirect3DIndexBuffer9* indexBuffer = NULL;
		IDirect3DVertexBuffer9* vertexBuffer = NULL;
		DWORD fvf;
		mesh->GetIndexBuffer(&indexBuffer);
		mesh->GetVertexBuffer(&vertexBuffer);
		fvf = mesh->GetFVF();
		
		g_pD3DDevice->SetFVF(fvf);
		g_pD3DDevice->SetIndices(indexBuffer);
		g_pD3DDevice->SetStreamSource( 0, vertexBuffer, 0, mesh->GetNumBytesPerVertex() );

 		DWORD attribTableSize = 0;
 		D3DXATTRIBUTERANGE attrribTable[20];
 		mesh->GetAttributeTable(NULL,&attribTableSize);
 		mesh->GetAttributeTable( (D3DXATTRIBUTERANGE*)attrribTable, &attribTableSize );

		for (int i = 0; i < /*pMeshContainer->NumAttributeGroups*/attribTableSize; i++)
		{
			g_pD3DDevice->SetMaterial( &pMeshContainer->pMaterials[i].MatD3D );

			CTexture *texTemp  = pMeshContainer->ppTextures[i];
			if ( texTemp && texTemp->IsValid() )
			{
				if (g_pCurCEffect)
					g_pCurCEffect->SetTexture( "g_TextureSrcDiffuse", texTemp->GetD3DTexture() );
				else
					g_pD3DDevice->SetTexture( 0, texTemp->GetD3DTexture() );
			}

			if (g_pCurCEffect)
			{
				D3DXMATRIX matView,matProj;
				g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
				g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
				D3DXMATRIX matWorldView = pFrame->CombinedTransformationMatrix * matView;
				D3DXMATRIX matWorldViewProj = matWorldView * matProj;
				g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
				g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
				g_pCurCEffect->setMatrix(CEffectFile::k_worldMatrix,&pFrame->CombinedTransformationMatrix);

			}
			else
			{
				g_pD3DDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
				g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_Camera.m_mView);
				g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.m_mProj);
			}

			// if set effect parm,must be use before every DrawIndexedPrimitive
			if (g_pCurCEffect)
				g_pCurCEffect->CommitChanges(); 

			//pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );	
			HRESULT hr = g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,
				attrribTable[i].VertexStart,
				attrribTable[i].VertexCount,
				attrribTable[i].FaceStart * 3,
				attrribTable[i].FaceCount );
			assert( SUCCEEDED(hr) );
 
 			/// memory leak;
			SAFE_RELEASE(indexBuffer);
			SAFE_RELEASE(vertexBuffer);
		}

		/*
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			m_pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );

			CTexture *texTemp  = pMeshContainer->ppTextures[iMaterial];
			if (!texTemp || !texTemp->IsValid() )
				continue;

			m_pd3dDevice->SetTexture( 0, texTemp->GetD3DTexture() );
			
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
		*/
	}
}

// 因为 pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
// 所以更新pFrame->CombinedTransformationMatrix 也就更新了pMeshContainer->ppBoneMatrixPtrs
void CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED *pFrame =static_cast<D3DXFRAME_DERIVED*>(pFrameBase);

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, 
			&pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

bool CSkinMesh::SetAnimationName(const char *strAnimName)
{
	if(!m_bMoving || !m_pAnimController  ) 
		return false;

	DWORD nAnimSet;
	char strTempName[256];
	nAnimSet=m_pAnimController->GetNumAnimationSets();
	LPD3DXANIMATIONSET pAnimSet;

	for(DWORD i=0;i<nAnimSet;i++)
	{	
		m_pAnimController->GetAnimationSet(i,&pAnimSet);
		strcpy(strTempName,	pAnimSet->GetName());
		if(strcmp(strAnimName,strTempName)==0) 
		{
			m_pAnimController->SetTrackAnimationSet(0,pAnimSet);
			return true;
		}
	}
	return false;
}


void CSkinMesh::CalculateBondingBox(LPD3DXFRAME pFrameBase)
{
	LPVOID pV = NULL; 
	LPD3DXMESH pMesh = NULL;
	LPD3DXMATRIX pMat = NULL;
	D3DXVECTOR3 vMin(0,0,0),vMax(0,0,0); 
	D3DXVECTOR3 vTransFormed1(0,0,0),vTransFormed2(0,0,0);

	if(pFrameBase->pMeshContainer)
	{
		pMesh=pFrameBase->pMeshContainer->MeshData.pMesh;

		pMat=&(((D3DXFRAME_DERIVED*)pFrameBase)->CombinedTransformationMatrix);//变换矩阵

		pMesh->LockVertexBuffer(0,&pV); 

		D3DXComputeBoundingBox( (LPD3DXVECTOR3)pV,
			pMesh->GetNumVertices(),
			pMesh->GetNumBytesPerVertex(),&vMin,&vMax); 
		
		//D3DXVec3TransformCoord(&vTransFormed1,&vMin,pMat); //将得到的坐标转换到根骨骼世界坐标下
		//D3DXVec3TransformCoord(&vTransFormed2,&vMax,pMat);

// 		vTransFormed1.x=pMat->_11*vMin.x+pMat->_21*vMin.y+pMat->_31*vMin.z+pMat->_41;
// 		vTransFormed1.y=pMat->_12*vMin.x+pMat->_22*vMin.y+pMat->_32*vMin.z+pMat->_42;
// 		vTransFormed1.z=pMat->_13*vMin.x+pMat->_23*vMin.y+pMat->_33*vMin.z+pMat->_43;
// 
// 		vTransFormed2.x=pMat->_11*vMax.x+pMat->_21*vMax.y+pMat->_31*vMax.z+pMat->_41;
// 		vTransFormed2.y=pMat->_12*vMax.x+pMat->_22*vMax.y+pMat->_32*vMax.z+pMat->_42;
// 		vTransFormed2.z=pMat->_13*vMax.x+pMat->_23*vMax.y+pMat->_33*vMax.z+pMat->_43;
// 
// 		if(vTransFormed1.x<m_vMin.x) m_vMin.x=vTransFormed1.x;
// 		if(vTransFormed1.y<m_vMin.y) m_vMin.y=vTransFormed1.y;
// 		if(vTransFormed1.z<m_vMin.z) m_vMin.z=vTransFormed1.z;
// 
// 		if(vTransFormed2.x<m_vMin.x) m_vMin.x=vTransFormed2.x;
// 		if(vTransFormed2.y<m_vMin.y) m_vMin.y=vTransFormed2.y;
// 		if(vTransFormed2.z<m_vMin.z) m_vMin.z=vTransFormed2.z;
// 
// 		if(vTransFormed2.x>m_vMax.x) m_vMax.x=vTransFormed2.x;
// 		if(vTransFormed2.y>m_vMax.y) m_vMax.y=vTransFormed2.y;
// 		if(vTransFormed2.z>m_vMax.z) m_vMax.z=vTransFormed2.z;
// 
// 		if(vTransFormed1.x>m_vMax.x) m_vMax.x=vTransFormed1.x;
// 		if(vTransFormed1.y>m_vMax.y) m_vMax.y=vTransFormed1.y;
// 		if(vTransFormed1.z>m_vMax.z) m_vMax.z=vTransFormed1.z;

		//m_vMin = vTransFormed1;
		//m_vMax = vTransFormed2;

		m_modelAABB.Merge(vMin);
		m_modelAABB.Merge(vMax);

		pMesh->UnlockVertexBuffer(); 
	}

	if(pFrameBase->pFrameSibling)
		CalculateBondingBox(pFrameBase->pFrameSibling);

	if(pFrameBase->pFrameFirstChild)
		CalculateBondingBox(pFrameBase->pFrameFirstChild);
}




