#pragma once

#include <string>

#include "bounding.h"
#include "SinMesh.h"
#include "orientation.h"

class COctreeNode;


#define FOB_NOT_VISIBLE			(1<<0)			
#define FOB_IS_SKIN				(1<<1)			
#define FOB_IS_TERRAIN			(1<<2)	
#define FOB_NOT_CAST_SHADOW		(1<<3)
#define FOB_NOT_RECEIVE_SHADOW  (1<<4) 


class CObject
{
public:
	CObject();
	virtual ~CObject();

	std::string GetName() {return m_sName;}
	void SetName(const std::string& sName) {m_sName = sName;}

	std::string GetHashName() {return m_sHashName;}
	void SetHashName(const std::string& sName) {m_sHashName = sName;}

	CAABB GetWorldAABB() {return m_WorldAABB;}

	COctreeNode* GetOctant() {return m_pOCtant;}
	void SetOctant(COctreeNode* pOctant) {m_pOCtant = pOctant;}

	bool IsIn(CAABB& box);

	D3DXVECTOR3 GetWordPos() {return m_wordPos;}
	void SetWordPos(const D3DXVECTOR3& wordPos) {m_wordPos = wordPos;}

	float GetScale() {return m_scale;}
	void SetScale(float scale) {m_scale = scale;}

	float GetAngle() {return m_angle;}
	void SetAngle(float angle) {m_angle = angle;}


	virtual void Render() = 0;
	virtual void Update() {}

	const D3DXMATRIX&	worldMatrix()const;
	const D3DXMATRIX&	localMatrix()const;
	const D3DXMATRIX&	inverseWorldMatrix();
	//bool isWorldMatrixNew()const;

	const D3DXVECTOR3& worldPosition();
	const D3DXVECTOR3& worldRight();
	const D3DXVECTOR3& worldUp();
	const D3DXVECTOR3& worldForward();
	void updateWorldVectors();

	static void StaticInit();
	static CObject* StaticFindObject(const char* InName);
	static int GetObjectHash(const char* InName);

	void HashObject();
	//void UnhashObject(int OuterIndex);
	
public:

	int m_ObjFlag;

	//bool m_bIsSKin;
	//bool m_bIsTerrain;
	//bool m_bOnlyReceiveShadows;
	//bool m_bIsVisible;	
	std::string m_sName;

	D3DXVECTOR3 m_wordPos; // 世界空间位置
	float m_scale;		   // 缩放比例 
	float m_angle;         // 方向  

	CAABB m_WorldAABB; // 世界空间的AABB

	COctreeNode* m_pOCtant;   // 所在的八叉树节点

	std::string m_sHashName;
	CObject* m_HashNext;	


	/////// 管理器
	enum {OBJ_ARRAY_NUM = 4096};
	static CObject*	 GObjHash[OBJ_ARRAY_NUM]; // Object hash.
};


class CMapObj : public CObject
{
public:

	CMapObj();
	~CMapObj();

	void SetSkinMesh(const std::string& modelName);
	virtual void Render();
	virtual void Update();
	void UpdateWorldAABB();

public:
	CSkinMesh *m_pSkinMesk;
};


class CCharactor : public CMapObj
{

public:
	void Render()
	{
		//SetAni(m_sCurAniName);
		CMapObj::Render();
	}

	void SetAni(const std::string& aniName) 
	{
		m_sCurAniName = aniName;

		if (m_pSkinMesk == NULL)
			return;

		m_pSkinMesk->SetAnimationName( aniName.c_str() );
	};

	void Move(const D3DXVECTOR3& addPos) //位置调整
	{
		m_wordPos += addPos;
	}

	void Turn(float addAngle) // 角度调整
	{
		m_angle += addAngle;
	}

private:
	std::string m_sCurAniName;
};
