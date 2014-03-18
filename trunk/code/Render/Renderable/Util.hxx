#include "Util.h"



namespace ma
{
	GameObjectPtr CreateMeshGameObject(const char* pSknPath,const char* pTexPath)
	{
		GameObjectPtr pGameObj = GetEntitySystem()->CreateGameObject("");

		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(pSknPath,pTexPath);

		return pGameObj;
	}

	GameObjectPtr CreateCollisionGameObject(const char* pSknPath,const char* pTexPath)
	{
		GameObjectPtr pGameObj = GetEntitySystem()->CreateGameObject("");

		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(pSknPath,pTexPath);
		
		AABB aabb = pMeshComp->GetAABBWS();
		NodeTransform tsf;
		TransformSetIdentity(&tsf);
		tsf.m_vPos = aabb.Center();
		
		IBoxCollisionShapePtr pBoxCollisionShape = pGameObj->CreateComponent<IBoxCollisionShape>();
		pBoxCollisionShape->SetSize(aabb.Size());
		pBoxCollisionShape->SetTransformLS(tsf);
		
		return pGameObj;
	}

	GameObjectPtr CreateAnimationGameObject(const char* psknPath,const char* pTexPath,
		const char* pSkePth,const char* pAnimSetPath)
	{
		GameObjectPtr pGameObj = GetEntitySystem()->CreateGameObject("");

		MeshComponentPtr pMeshComp = pGameObj->CreateComponent<MeshComponent>();
		pMeshComp->Load(psknPath,pTexPath);

		IAnimationObjectPtr pAnimComp = pGameObj->CreateComponent<IAnimationObject>();
		pAnimComp->Load(pAnimSetPath,pSkePth);

		return pGameObj;
	}
}


