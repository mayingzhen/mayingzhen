#include "SampleRagdoll.h"
#include "Ragdoll.hxx"

namespace ma
{

	SampleRagdoll::SampleRagdoll()
	{
		m_bStart = false;
		m_pRagdoll = NULL;
	}


	void SampleRagdoll::Load()
	{
		m_bStart = false;
		GetInput()->AddKeyListener(this);

		Vector3 vEyePos = Vector3(100, -400, 300);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		GetPhysicsSystem()->SetGravity(Vector3(0,0,-98));

		{
			SceneNode* pGameObj = m_pScene->CreateNode("magician");

 			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
 			pMeshComp->Load("magician/Body.skn","magician/Body.mat");

			m_pAnimationComp = pGameObj->CreateComponent<AnimationComponent>();
			m_pAnimationComp->Load("magician/body.Aniset","magician/body.ske");
		}

		m_pAnimationComp->PlayAnimation(1);

		{
			RefPtr<SceneNode> pGameObj = m_pScene->CreateNode("Terrain");
		
			IBoxCollisionShape* pBoxCollisionShape = pGameObj->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(800,800,10));

			pGameObj->Translate(Vector3(0,0,-50));
		}
		
		std::vector<std::string> arrBoneName;
		arrBoneName.push_back("Bip01 Pelvis");
		arrBoneName.push_back("Bip01 Spine");
		arrBoneName.push_back("Bip01 Head");
		arrBoneName.push_back("Bip01 L UpperArm");
		arrBoneName.push_back("Bip01 R UpperArm");
		arrBoneName.push_back("Bip01 L Forearm");
		arrBoneName.push_back("Bip01 R Forearm");
		arrBoneName.push_back("Bip01 L Thigh");
		arrBoneName.push_back("Bip01 R Thigh");
		arrBoneName.push_back("Bip01 L Calf");
		arrBoneName.push_back("Bip01 R Calf");

		std::vector<Vector2> arrSize;
		arrSize.push_back(Vector2(15,5));
		arrSize.push_back(Vector2(15,5));
		arrSize.push_back(Vector2(10,5));
		arrSize.push_back(Vector2(5,20));
		arrSize.push_back(Vector2(5,20));
		arrSize.push_back(Vector2(4,20));
		arrSize.push_back(Vector2(4,20));
		arrSize.push_back(Vector2(7,30));
		arrSize.push_back(Vector2(7,30));
		arrSize.push_back(Vector2(5,30));
		arrSize.push_back(Vector2(5,30));

		m_pRagdoll = new Ragdoll();
		m_pRagdoll->Init(m_pAnimationComp,arrBoneName,arrSize);

	}

	void SampleRagdoll::UnLoad()
	{
		GetInput()->RemoveKeyListener(this);
	}

	void SampleRagdoll::Update()
	{
		if (m_bStart)
		{
			m_pRagdoll->SyncFromPhysics();
		}
		else
		{
			m_pRagdoll->SyncToPhysics();
		}
	}

	void SampleRagdoll::Render()
	{
		m_pAnimationComp->DebugRender(false);
	}

	bool SampleRagdoll::keyPressed(const OIS::KeyEvent &arg)
	{
		if (arg.key == OIS::KC_R)
		{
			m_bStart = true;
			
			m_pRagdoll->Start();
		}

		return true;
	}

}


