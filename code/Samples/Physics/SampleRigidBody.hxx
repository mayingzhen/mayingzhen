#include "Samples/Physics/SampleRigidBody.h"
#include "Framwork/Module.h"
#include "BulletPhysics/Module.h"

namespace ma
{
	SampleRigidBody::SampleRigidBody()
	{
	}


	void SampleRigidBody::UnLoad()
	{
	}	

	void SampleRigidBody::Load()
	{

		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f * 10,0));

		GameObject* pGameObjA = NULL;
		{
			GameObject* pGameObj = GetEntitySystem()->CreateGameObject("physics");
			pGameObjA = pGameObj;

			BoxCollisionComponent* pBoxCollisionComp = pGameObj->CreateComponent<BoxCollisionComponent>();
 			pBoxCollisionComp->GetBoxCollisionShape()->SetSize(Vector3(5,5,5));

			RigidBodyComponent* pRigidBodyComp = pGameObj->CreateComponent<RigidBodyComponent>();
			m_pRigidBody = pRigidBodyComp->GetRigidBody();
			m_pRigidBody->SetUseGravity(false);

			pGameObj->GetSceneNode()->Translate(Vector3(0,40,0));

			int nClone = 5;
			for (int i = 0; i < nClone; ++i)
			{
				std::string pName = pGameObj->GetName();
				pName += std::string("_clone") + StringConverter::ToString(i);
				GameObject* pClone = pGameObj->Clone(pName.c_str());
				pClone->GetSceneNode()->Translate(Vector3(10 * i,10,0));
			}


			{
				GameObject* pGameObjSphere = GetEntitySystem()->CreateGameObject("Sphere");
				SphereCollisionComponent* pSphereComp = pGameObjSphere->CreateComponent<SphereCollisionComponent>();
				pSphereComp->GetSphereCollisionShape()->SetRadius(20);
				pGameObjSphere->CreateComponent<RigidBodyComponent>();

				pGameObjSphere->GetSceneNode()->Translate(Vector3(10,80,0));
			}

		}

		GameObject* pGameObjB = NULL;
		{
			GameObject* pGameObj = GetEntitySystem()->CreateGameObject("Terrain");
			pGameObjB = pGameObj;

			BoxCollisionComponent* pBoxCollisionShape = pGameObj->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->GetBoxCollisionShape()->SetSize(Vector3(100,5,100));

			//pGameObj->GetSceneNode()->Translate(vCenter,TS_WORLD);
		}


		pGameObjA->GetPhyscisObject()->AddCollisionListener(pGameObjB->GetPhyscisObject(),this);
	}


	void SampleRigidBody::Update()
	{

		if (GetInput()->IsKeyDown(OIS::KC_G))
		{
			m_pRigidBody->SetUseGravity(true);
		}
		if (GetInput()->IsKeyDown(OIS::KC_K))
		{
			m_pRigidBody->SetKinematic(false);
		}
	}

	void SampleRigidBody::Render()
	{
	}

	void SampleRigidBody::collisionEvent(const CollisionData& eventData)
	{
		int i = 5;
	}

}


