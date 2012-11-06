
#ifndef  C_SIN_MESH__H__
#define  C_SIN_MESH__H__

#include <d3dx9.h>

#include "Res.h"
#include "bounding.h"

#include "public.h"


class CTexture;
class CSkinMesh;

typedef unsigned short BoneIndex;

struct CNodeTransform
{
	D3DXVECTOR3		m_vPos;
	D3DXQUATERNION	m_qRot;
	float			m_fScale;

	static CNodeTransform Identity();
};



class CNodePose
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
	mutable std::vector<CNodeTransform> m_arrTSF_PS;
	mutable std::vector<CNodeTransform>	m_arrTSF_OS;
	std::vector<BoneIndex>		m_arrParentIndice;

	void InitWithParentIndice(const std::vector<BoneIndex>& arrParentIndice);

	void InitParentSpace(const std::vector<CNodeTransform>& arrTSF_PS, const std::vector<BoneIndex>& arrParentIndice);

	void InitObjectSpace(const std::vector<CNodeTransform>& arrTSF_OS, const std::vector<BoneIndex>& arrParentIndice);

	void InitLocalSpace(const std::vector<CNodeTransform>& arrTSF_LS, const std::vector<BoneIndex>& arrParentIndice);

	void SetTransformPSAll(const std::vector<CNodeTransform>& arrTSF_PS);

	void SetTransformOSAll(const std::vector<CNodeTransform>& arrTSF_OS);

	void SyncParentSpace() const;

	void SyncObjectSpace() const;

	void SetTransformPS(const CNodeTransform* pTSF, BoneIndex nBoneInd);

	void SetTransformOS(const CNodeTransform* pTSF, BoneIndex nBoneInd);

	const CNodeTransform& GetTransformOS(BoneIndex nBoneInd) const;

	const CNodeTransform& GetTransformPS(BoneIndex nBoneInd) const;

protected:

	void UpdateTransformPS(BoneIndex nBoneInd) const;

	void UpdateTransformOS(BoneIndex nBoneInd) const;

	void SyncAllChildPS(BoneIndex nAncestorInd) const;

	bool IsAncestor(BoneIndex nAncestorInd,BoneIndex nDescendantInd) const;

};

class CSkeleton
{
public:
	CSkeleton();
	~CSkeleton();

	void CSkeleton::Init(const std::vector<std::string>& arrBoneName,
						 const std::vector<BoneIndex>& arrParentInd,
						 const std::vector<CNodeTransform>& arrNodeOS);

	const D3DXMATRIX& GetBoneMatrixOSInv(BoneIndex nBoneID);

private:
	std::string m_name;

	std::vector<std::string> m_arrBoneName;
	std::vector<BoneIndex>	 m_arrParentInd;

	CNodePose				 m_refPose;

	// D3DXMATRIX
 	std::vector<D3DXMATRIX> m_arrRefPosePS;
 	std::vector<D3DXMATRIX> m_arrRefPoseOS;
 	std::vector<D3DXMATRIX> m_arrRefPoseOSInv;

};


class CAnimatedSkeleton
{
public:
	void ComputeSingleBoneSkinMatrix(D3DXMATRIX* mat, UINT nBoneID);


private:
	CSkeleton* m_pSkel;
	
	CNodePose*	m_pRefPose;
	CNodePose	m_pose;
};



void DecomposeFrame(float frame,const std::vector<UINT>& vkeyFrame,UINT& key0,UINT& key1,float& factor);


template<class T>
class CDataTrack
{
public:
	void SampleFrame(float frame, T& val) const
	{
		UINT nKey0, nKey1;
		float fFactor;
		DecomposeFrame(frame,m_arrFrame,nKey0,nKey1,fFactor);
		if (nKey0 != nKey1)
		{
			Lerp(val,m_arrValue[nKey0],m_arrValue[nKey1],fFactor);
		}
		else
		{
			val = m_arrValue[nKey0];
		}
	}

public:
	std::vector<UINT> m_arrFrame;
	std::vector<T>	  m_arrValue;
};

typedef CDataTrack<float> CFloatTrack;

class CVector3Track
{
public:
	void SampleFrame(float frame,D3DXVECTOR3& val) const
	{
		UINT key0,key1;
		float factor;

		DecomposeFrame(frame, m_arrFrame, key0, key1, factor);
		D3DXVec3Lerp(&val, &m_arrValue[key0], &m_arrValue[key1], factor);
	}

public:
	std::vector<UINT>		 m_arrFrame;
	std::vector<D3DXVECTOR3> m_arrValue;
};

class CQuaternionTrack
{
public:
	void SampleFrame(float frame, D3DXQUATERNION& val) const
	{
		UINT key0,key1;
		float factor;

		DecomposeFrame(frame, m_arrFrame, key0, key1, factor);
		D3DXQuaternionSlerp(&val, &m_arrValue[key0], &m_arrValue[key1], factor);
	}

public:
	std::vector<UINT>			m_arrFrame;
	std::vector<D3DXQUATERNION> m_arrValue;
};

struct CAnimationTracks
{
	std::vector<CVector3Track*>		m_scale;
	std::vector<CQuaternionTrack*>	m_rot;
	std::vector<CVector3Track*>		m_pos;

	std::vector<CFloatTrack*>		m_float;
};

class CAnimation
{
public:

// 	void Create(UINT nFrameNum,
// 		UINT nTransfromTrackNum,
// 		UINT nBoneNum)

	void SampleAndAddSingleTrackByFrame(CNodeTransform* pTSF, BoneIndex nTrackID, float fWeight, float fFrame) const;

	void SampleSingleTrackByFrame(CNodeTransform* pTSF, BoneIndex nTrackID, float fFrame) const;

	float GetFrameRate();

	UINT GetTotalFrame();


private:
	std::string m_name;

	//std::vector<std::string> m_arrr
	UINT	m_nBoneNum;

	UINT	m_nSocketNum;

	UINT	m_nFrameNumber;

	CAnimationTracks* m_pRawTracks;

public:
	std::vector<CVector3Track>		m_scale;
	std::vector<CQuaternionTrack>	m_rot;
	std::vector<CVector3Track>		m_pos;
	//std::vector<CFloatTrack>		m_float;

};

enum ePlaybackMode
{
	PLAYBACK_ONCE,
	PLAYBACK_LOOP,
};

enum ePlayerStatus
{
	PLAYER_STOP,
	PLAYER_PLAYING,
	PLAYER_PAUSE,
	PLAYER_STATUS_NUM,
};

struct CAnimEvalContext
{
	const CNodePose* m_pRefPose;
	
	CNodePose*		 m_pPose;

	bool			 m_bLSAnimVaild;
	std::vector<CNodeTransform>	m_arrTSFLS;
	std::vector<float>			m_arrAnimWeight;

	CNodeTransform	 m_rootMotionLS;

	float			 m_rootWeight;
};

class CBoneSet
{
public:
	std::vector<BoneIndex> m_arrBone;
};


class CSkeletonContext;
class CBoneSet;
class CAnimationInst
{
public:
	CAnimationInst(CAnimation* pAnim);
	~CAnimationInst();

	void AdvanceTime(float fTimeElapsed);

	void EvaluateAnimation(CAnimEvalContext* pContext, float fWeight, CBoneSet* pBoneSet);

	void WrapLocalFrame();

	void Instantiate(CSkeletonContext* pSkelContext, CBoneSet* pBoneSet);


private:
	CAnimation* m_pAnimtion;

	float m_fLocalFrame;
	float m_fPlaySpeed;

	ePlaybackMode m_playbackMode;
	ePlayerStatus m_playerStatus;
};

class CSkeletonContext;
class CAnimTreeNode
{
public:
	virtual bool Instantiate(CSkeletonContext* pSkelContext) = 0;

	virtual void AdvanceTime(float fTimeElapsed) = 0;

	virtual void EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight) = 0;
};

class CClipNode : public CAnimTreeNode
{
public:
	bool Instantiate(CSkeletonContext* pSkelContext);

	virtual void AdvanceTime(float fTimeElapsed);

	virtual void EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight) = 0;

private:
	CBoneSet* m_pBoneSet;

	CAnimationInst* m_pAnimInst;
};

class CSkeletonContext;

class CLayerNode : public CAnimTreeNode
{
public:
	void AddLayer(CAnimTreeNode* pAnimNode);

	void RemoveLayer(CAnimTreeNode* pAnimNode);

	bool Instantiate(CSkeletonContext* pSkelContext);

	virtual void AdvanceTime(float fTimeElapsed);

	virtual void EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight) = 0;

private:
	std::vector<CAnimTreeNode*> m_arrChildLayer;
};

class CBlendNode : public CAnimTreeNode
{
public:
	CBlendNode();
		
	bool Instantiate(CSkeletonContext* pSkelContext);

	virtual void AdvanceTime(float fTimeElapsed);

	virtual void EvaluateAnimation(CAnimEvalContext* pAnimContext, float fWeight);

private:
	CAnimTreeNode* m_pSrcNode;
	CAnimTreeNode* m_pDestNode;
	float		   m_fWeight;
};

class CSkeletonContext 
{

private:
	CAnimatedSkeleton* m_pSkelInst;


	void BeginAnimEval(CAnimEvalContext* pAnimContext);

	void BlendClip(CAnimEvalContext* pAnimContext, CAnimationInst* pAnimInst, float fWeight);

	void BlendAnimTree(CAnimationInst* pAnimContext, CAnimTreeNode* pTree, float fWeight);

	void EndAnimEval(CAnimEvalContext* pAnimContext);
};






//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;//组合变换矩阵
};

//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	//LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified
	CTexture** ppTextures;

	// SkinMesh info             
	LPD3DXMESH           pOrigMesh;					//原始网格 
	LPD3DXATTRIBUTERANGE pAttributeTable;			//属性表
	DWORD                NumAttributeGroups;		//属性组数量,即子网格数量
	DWORD                NumInfl;					//每个顶点最多受多少骨骼的影响
	LPD3DXBUFFER         pBoneCombinationBuf;		//骨骼结合表
	D3DXMATRIX**         ppBoneMatrixPtrs;			//存放骨骼的组合变换矩阵
	D3DXMATRIX*          pBoneOffsetMatrices;		//存放骨骼的初始变换矩阵
	DWORD                NumPaletteEntries;			//骨骼数量上限
	bool                 UseSoftwareVP;				//标识是否使用软件顶点处理
	DWORD                iAttributeSW;				//used to denote the split between SW and HW if necessary for non-indexed skinning
};




//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//-----------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy //做骨骼的矩阵，作用把骨骼的
{
public:
	CAllocateHierarchy(CSkinMesh *pSkinMesh):m_pSkinMesh(pSkinMesh) {}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	HRESULT GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer);

public:
	CSkinMesh*		m_pSkinMesh;
};

class CSkinMesh : public CRes
{
	friend class CResMeshMngr;

public:
	CSkinMesh();
	~CSkinMesh();

	HRESULT LoadFromXFile(const char* strFileName);
	void Render(LPD3DXMATRIX transWorld);//渲染
	void Upadte();
	bool SetAnimationName(const char *strAnimName); //设置动作的名字

	const CAABB& GetBoundingBox() {return m_modelAABB;}
	const D3DXMATRIX& GetRootMatrix() {return m_pFrameRoot->TransformationMatrix;}
	const D3DXVECTOR3& GetCenter() {return m_vObjectCenter;}

private: 
	void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void DrawFrame(LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );

	void CalculateBondingBox(LPD3DXFRAME pFrameBase);

private:
	D3DXMATRIXA16*				m_pBoneMatrices;
	UINT						m_NumBoneMatricesMax;
	bool						m_bMoving;
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	LPD3DXFRAME					m_pFrameRoot;

	D3DXVECTOR3 m_vObjectCenter;
	float m_fObjectRadius;

	//D3DXVECTOR3 m_vMin;		   
	//D3DXVECTOR3 m_vMax;
	CAABB m_modelAABB;
};


#endif 