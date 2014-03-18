#include "Samples/Physics/SampleRigidBody.h"

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
		GameObjectPtr pGameObjA;
		{
			pGameObjA = GetEntitySystem()->CreateGameObject("physics");

			IBoxCollisionShapePtr pBoxCollisionComp = pGameObjA->CreateComponent<IBoxCollisionShape>();
 			pBoxCollisionComp->SetSize(Vector3(5,5,5));

			m_pRigidBody = pGameObjA->CreateComponent<IRigidBody>();
			m_pRigidBody->SetUseGravity(false);

			pGameObjA->GetSceneNode()->Translate(Vector3(0,40,0));


			for (int i = 0; i < 5; ++i)
			{
				std::string pName = pGameObjA->GetName();
				pName += std::string("_clone") + StringConverter::ToString(i);
				GameObjectPtr pClone = pGameObjA->Clone(pName.c_str());
				pClone->GetSceneNode()->Translate(Vector3(10 * i,10,0));
			}


			{
				GameObjectPtr pGameObjSphere = GetEntitySystem()->CreateGameObject("Sphere");
				ISphereCollisionShapePtr pSphereComp = pGameObjSphere->CreateComponent<ISphereCollisionShape>();
				pSphereComp->SetRadius(20);
				pGameObjSphere->CreateComponent<IRigidBody>();

				pGameObjSphere->GetSceneNode()->Translate(Vector3(10,80,0));
			}
		}

		GameObjectPtr pGameObjB;
		{
			GameObjectPtr pGameObj = GetEntitySystem()->CreateGameObject("Terrain");

			IBoxCollisionShapePtr pBoxCollisionShape = pGameObj->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(100,5,100));
		}

		//pGameObjA->GetPhyscisObject()->AddCollisionListener(pGameObjB->GetPhyscisObject(),this);
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


	void SampleRigidBody::collisionEvent(const CollisionData& eventData)
	{
		int i = 5;
	}

}


