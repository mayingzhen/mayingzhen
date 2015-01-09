#include "Samples/Physics/SampleRigidBody.h"

namespace ma
{
	SampleRigidBody::SampleRigidBody()
	{
	}


	void SampleRigidBody::UnLoad()
	{
		m_pRigidBody = NULL;
	}	

	void SampleRigidBody::Load()
	{
		GetCamera()->GetSceneNode()->LookAt(Vector3(0, 30, 20), Vector3(0,0,0));

		SceneNodePtr pGameObjA;
		{
			pGameObjA = m_pScene->CreateNode("physics");

			RefPtr<BulletBoxCollisionShape> pBoxCollisionComp = pGameObjA->CreateComponent<BulletBoxCollisionShape>();
 			pBoxCollisionComp->SetSize(Vector3(0.5,0.5,0.5));

			m_pRigidBody = pGameObjA->CreateComponent<BulletRigidBody>();
			m_pRigidBody->SetUseGravity(false);

			pGameObjA->Translate(Vector3(0,0,4));


			for (int i = 0; i < 5; ++i)
			{
				std::string pName = pGameObjA->GetName();
				pName += std::string("_clone"); //+ StringConverter::toString(i);
				SceneNodePtr pClone = pGameObjA->Clone(pName.c_str());
				pClone->Translate(Vector3(1 * i,0,1));
			}


			{
				SceneNodePtr pGameObjSphere = m_pScene->CreateNode("Sphere");
				RefPtr<BulletSphereCollisionShape> pSphereComp = pGameObjSphere->CreateComponent<BulletSphereCollisionShape>();
				pSphereComp->SetRadius(2);
				pGameObjSphere->CreateComponent<BulletRigidBody>();

				pGameObjSphere->Translate(Vector3(1,0,8));
			}
		}

		SceneNodePtr pGameObjB;
		{
			SceneNodePtr pGameObj = m_pScene->CreateNode("Terrain");

			RefPtr<BulletBoxCollisionShape>  pBoxCollisionShape = pGameObj->CreateComponent<BulletBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(10,10,0.5));
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
	}

}


