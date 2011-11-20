
#ifndef  C_SIN_MESH__H__
#define  C_SIN_MESH__H__

#include <d3dx9.h>

#include "Res.h"
#include "bounding.h"

class CTexture;
class CSkinMesh;


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