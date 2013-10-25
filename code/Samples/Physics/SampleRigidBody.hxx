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

		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f * 10,0));

		GameObject* pGameObjA = NULL;
		{
			GameObject* pGameObj = GetEntitySystem()->CreateGameObject("physics");
			pGameObjA = pGameObj;

			IBoxCollisionShape* pBoxCollisionComp = pGameObj->CreateComponent<IBoxCollisionShape>();
 			pBoxCollisionComp->SetSize(Vector3(5,5,5));

			m_pRigidBody = pGameObj->CreateComponent<IRigidBody>();
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
				ISphereCollisionShape* pSphereComp = pGameObjSphere->CreateComponent<ISphereCollisionShape>();
				pSphereComp->SetRadius(20);
				pGameObjSphere->CreateComponent<IRigidBody>();

				pGameObjSphere->GetSceneNode()->Translate(Vector3(10,80,0));
			}

		}

		GameObject* pGameObjB = NULL;
		{
			GameObject* pGameObj = GetEntitySystem()->CreateGameObject("Terrain");
			pGameObjB = pGameObj;

			IBoxCollisionShape* pBoxCollisionShape = pGameObj->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(100,5,100));

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


