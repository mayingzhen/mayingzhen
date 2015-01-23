#include "SampleCharaControl.h"

namespace ma
{
	SampleCharaControl::SampleCharaControl()
	{
		m_fMoveSpeed = 200;
		m_bMoveing = false;
	}

	void SampleCharaControl::Load()
	{
		Vector3 vEyePos = Vector3(0, 800, 600);
		Vector3 VAtPos = Vector3(0,0,0); 
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos);

		GetInput()->AddTouchListener(this);
		GetInput()->AddMouseListener(this);

		{
			SceneNodePtr pCharaObj = m_pScene->CreateNode("Chara");		
			RefPtr<CharacterController> pCharComp = pCharaObj->CreateComponent<CharacterController>();
			CapsuleCollisionShape* pCapsule = pCharComp->GetCollisionShape();
		
			MeshComponentPtr pMeshComp = pCharaObj->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");
		
// 			AABB aabb = pMeshComp->GetAABBWS();
// 
// 			Vector3 vSize = aabb.getSize();
// 			float fRadius = abs(vSize.x) / 2.0f;
// 			pCapsule->SetHeight(abs(vSize.y) - 2 * fRadius);
// 			pCapsule->SetRadius(fRadius);
// 	
// 			Transform tsfLS;
// 			tsfLS.m_vPos = aabb.getCenter();
// 			pCapsule->SetTransformLS(tsfLS);


			m_pCharaAnim = pCharaObj->CreateComponent<AnimationComponent>();
			m_pCharaAnim->Load("magician/Body.Aniset","magician/Body.ske");
			AnimationSet* pAnimSet = m_pCharaAnim->GetAnimationSet();
			pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","Run");
			pAnimSet->AddAnimClip("magician/100/bip01.ska","Idle");
			m_pCharaAnim->PlayAnimation("Idle");

			pCharaObj->Translate(Vector3(0,20,0));
		}

		{
			m_pTerrain = m_pScene->CreateNode("Terrain");
	
			RefPtr<BoxCollisionShape> pBoxCollisionShape = m_pTerrain->CreateComponent<BoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(1800,1800,20));

			{
				SceneNodePtr pObje = m_pScene->CreateNode("Box1");
				RefPtr<BoxCollisionShape> pBox = pObje->CreateComponent<BoxCollisionShape>();
				pBox->SetSize(Vector3(200,200,200));
				pObje->Translate(Vector3(-100,0,20));
			}


			{
				SceneNodePtr pObje2 = m_pScene->CreateNode("Box2");
				RefPtr<BoxCollisionShape> pBox = pObje2->CreateComponent<BoxCollisionShape>();
				pBox->SetSize(Vector3(200,200,200));
				RefPtr<RigidBody> pRigidBodyComp = pObje2->CreateComponent<RigidBody>();
				pObje2->Translate(Vector3(140,0,40));
			}

		}

	}

	void SampleCharaControl::UnLoad()
	{
		GetInput()->RemoveTouchListener(this);
		GetInput()->RemoveMouseListener(this);

		m_pTerrain = NULL;
		m_pCharaAnim = NULL;
	}


	void SampleCharaControl::Update()
	{
		DoMouveTo();
	}

	void SampleCharaControl::DoMouveTo()
	{
		if (!m_bMoveing)
			return;

		float fStepMoveLeng =  m_fMoveSpeed * GetTimer()->GetFrameDeltaTime();

		Vector3 vMoveDir = m_vMoveTo - m_pCharaAnim->GetSceneNode()->GetPos();
		float fDistance = vMoveDir.length();
		if (fDistance <= fStepMoveLeng)
		{
			m_bMoveing = false;
			m_pCharaAnim->PlayAnimation("Idle");
			return;
		}	

		vMoveDir.normalise();
		Vector3 vMoveStepWS = vMoveDir * fStepMoveLeng / fDistance;

		m_pCharaAnim->GetSceneNode()->Translate(vMoveStepWS);
	}

	void SampleCharaControl::Render()
	{
	}


	bool SampleCharaControl::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (id == OIS::MB_Left)
		{
			Vector2 mousePos(arg.state.X.abs,arg.state.Y.abs);

			OnTouch(mousePos);
		}

		return true;
	}

	bool SampleCharaControl::touchReleased( const OIS::MultiTouchEvent &arg )
	{
		Vector2 mousePos(arg.state.X.abs,arg.state.Y.abs);

		OnTouch(mousePos);
	
		return true;
	}

	void SampleCharaControl::OnTouch(Vector2 vTouchPos)
	{
		int width,height;
		Platform::GetInstance().GetWindowSize(width,height);

		Ray worldRay = GetCamera()->GetWorldRayCast(vTouchPos);
		
		SceneNode* pCharNode = m_pCharaAnim->GetSceneNode();

		Vector3 hitPosWS;
		SceneNode* pGameObj = GetPhysicsSystem()->RayCastCloseGameObj(worldRay.getOrigin(),worldRay.getDirection(),0,hitPosWS);
		if (pGameObj == m_pTerrain)
		{
			Vector3 curPos = pCharNode->GetPos();
			Vector3 vAxisFrom = pCharNode->GetForward();
			Vector3 vAxisTo = hitPosWS - curPos;
			vAxisTo.normalise();
			vAxisFrom.normalise();

// 			Quaternion quat;
// 			quat.FromAngleAxis()
// 
// 			Vector3 pAxisRot = Vector3::UNIT_Z;
// 			float fTargetRota = 0;
// 			QuaternionFromAxisToAxis(&pAxisRot,&fTargetRota,&vAxisFrom,&vAxisTo);
// 			fTargetRota *= pAxisRot.y; // y = -1 逆时针转, y=1 顺时针转
// 			pCharNode->RotateYAxisLS(ToDegree(fTargetRota));

			m_vMoveTo = hitPosWS;
			m_bMoveing = true;

			m_pCharaAnim->PlayAnimation("Run");

			//Log("curPos = %f,%f,%f,fTargetRota = %f",curPos.x,curPos.y,curPos.z,ToDegree(fTargetRota));
			LogInfo("m_vMoveTo = %f,%f,%f",m_vMoveTo.x,m_vMoveTo.y,m_vMoveTo.z);
		}
	}

}