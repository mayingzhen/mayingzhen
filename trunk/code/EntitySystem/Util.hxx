#include "Util.h"

#include "Component/MeshComponent.h"
#include "Component/CollisionComponent.h"


namespace ma
{
	GameObject* CreateMeshGameObject(const char* pSknPath,const char* pTexPath)
	{
		GameObject* pGameObj = GetEntitySystem()->CreateGameObject("");

		MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(pSknPath,pTexPath);

		return pGameObj;
	}

	GameObject* CreateCollisionGameObject(const char* pSknPath,const char* pTexPath)
	{
		GameObject* pGameObj = GetEntitySystem()->CreateGameObject("");

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

	GameObject* CreateAnimationGameObject(const char* psknPath,const char* pTexPath,const char* pSkePth)
	{
		GameObject* pGameObj = GetEntitySystem()->CreateGameObject("");

		MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(psknPath,pTexPath);

		AnimComponent* pAnimComp = pGameObj->CreateComponent<AnimComponent>();
		pAnimComp->Load(NULL,pSkePth);

		return pGameObj;
	}
}


