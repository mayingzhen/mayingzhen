#include "Util.h"

#include "Component/MeshComponent.h"
#include "Component/CollisionComponent.h"


namespace ma
{
	GameObject* CreateGameObjectWithCollision(Scene* pScene,const char* pSknPath,const char* pTexPath)
	{
		GameObject* pGameObj = pScene->CreateGameObject("");

		MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(pSknPath,pTexPath);

		AABB aabb = pMeshComp->GetBoundingAABB();
		NodeTransform tsf;
		TransformSetIdentity(&tsf);
		tsf.m_vPos = aabb.Center();

		BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();
		pBoxCollisionShape->GetBoxCollisionShape()->SetSize(aabb.Size());
		pBoxCollisionShape->GetBoxCollisionShape()->SetTransformLS(tsf);

		return pGameObj;
	}
}


