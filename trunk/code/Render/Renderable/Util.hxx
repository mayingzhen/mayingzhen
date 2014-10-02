#include "Util.h"



namespace ma
{
	SceneNodePtr CreateMeshGameObject(const char* pSknPath,const char* pTexPath)
	{
		ASSERT(false);
		return NULL;

		//SceneNodePtr pGameObj = GetEntitySystem()->CreateGameObject("");

		//MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		//pMeshComp->Load(pSknPath,pTexPath);

		//return pGameObj;
	}

	SceneNodePtr CreateCollisionGameObject(const char* pSknPath,const char* pTexPath)
	{
		ASSERT(false);
		return NULL;

// 		SceneNodePtr pGameObj = GetEntitySystem()->CreateGameObject("");
// 
// 		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
// 		pMeshComp->Load(pSknPath,pTexPath);
// 		
// 		AABB aabb = pMeshComp->GetAABBWS();
// 		Transform tsf;
// 		tsf.m_vPos = aabb.getCenter();
// 		
// 		IBoxCollisionShapePtr pBoxCollisionShape = pGameObj->CreateComponent<IBoxCollisionShape>();
// 		pBoxCollisionShape->SetSize(aabb.getSize());
// 		pBoxCollisionShape->SetTransformLS(tsf);
// 		
// 		return pGameObj;
	}

// 	SceneNodePtr CreateAnimationGameObject(const char* psknPath,const char* pTexPath,
// 		const char* pSkePth,const char* pAnimSetPath)
// 	{
// 		SceneNodePtr pGameObj = GetEntitySystem()->CreateGameObject("");
// 
// 		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
// 		pMeshComp->Load(psknPath,pTexPath);
// 
// 		AnimationObjectPtr pAnimComp = pGameObj->CreateComponent<AnimationComponent>();
// 		pAnimComp->Load(pAnimSetPath,pSkePth);
// 
// 		return pGameObj;
// 	}
}


