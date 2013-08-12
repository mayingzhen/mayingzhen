#include "SampleCharaControl.h"

namespace ma
{
	SampleCharaControl::SampleCharaControl()
	{
		m_pCharaObj = NULL;
		m_fMoveSpeed = 200;
		m_bMoveing = false;
	}

	void SampleCharaControl::Load()
	{
		Vector3 vEyePos = Vector3(0, 600, 800);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		GetInput()->AddMouseListener(this);


		GetPhysicsSystem()->SetGravity(Vector3(0,-0.98f * 10,0));

		{
			m_pCharaObj = GetEntitySystem()->CreateGameObject("Chara");		
			CharaControlComponent* pCharComp = m_pCharaObj->CreateComponent<CharaControlComponent>();
			ICapsuleCollisionShape* pCapsule = pCharComp->GetCharaControll()->GetCollisionShape();
		
			MeshComponent* pMeshComp = m_pCharaObj->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.tga");
		
			Vector3 vSize = pMeshComp->GetBoundingAABB().Size();
			float fRadius = abs(vSize.x) / 2.0f;
			pCapsule->SetHeight(abs(vSize.y) - 2 * fRadius);
			pCapsule->SetRadius(fRadius);
	
			NodeTransform tsfLS;
			TransformSetIdentity(&tsfLS);
			tsfLS.m_vPos = pMeshComp->GetBoundingAABB().Center();
			pCapsule->SetTransformLS(tsfLS);


			AnimComponent* pAnimComp = m_pCharaObj->CreateComponent<AnimComponent>();
			pAnimComp->Load(NULL,"magician/Body.ske");
			IAnimationSet* pAnimSet = pAnimComp->GetAnimObject()->GetAnimationSet();
			pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","Run");
			pAnimSet->AddAnimClip("magician/100/bip01.ska","Idle");
			pAnimComp->GetAnimObject()->PlayAnimation("Idle");

			m_pCharaObj->GetSceneNode()->Translate(Vector3(0,20,0));
			//m_pCharaObj->GetSceneNode()->RotateYAxisLS(90);
		}

		{
			m_pTerrain = GetEntitySystem()->CreateGameObject("Terrain");
	
			BoxCollisionComponent* pBoxCollisionShape = m_pTerrain->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->GetBoxCollisionShape()->SetSize(Vector3(1800,20,1800));

			{
				GameObject* pObje = GetEntitySystem()->CreateGameObject("Box1");
				BoxCollisionComponent* pComp = pObje->CreateComponent<BoxCollisionComponent>();
				pComp->GetBoxCollisionShape()->SetSize(Vector3(200,200,200));
				pObje->GetSceneNode()->Translate(Vector3(-100,20,0));
			}


			{
				GameObject* pObje2 = GetEntitySystem()->CreateGameObject("Box2");
				BoxCollisionComponent* pComp2 = pObje2->CreateComponent<BoxCollisionComponent>();
				pComp2->GetBoxCollisionShape()->SetSize(Vector3(200,200,200));
				RigidBodyComponent* pRigidBodyComp = pObje2->CreateComponent<RigidBodyComponent>();
				pObje2->GetSceneNode()->Translate(Vector3(140,40,0));
			}

		}

	}

	void SampleCharaControl::UnLoad()
	{
		GetInput()->RemoveMouseListener(this);
	}


	void SampleCharaControl::Update()
	{
		DoMouveTo();
	}

	void SampleCharaControl::MoveToWS(Vector3 vMouveTo)
	{
		SceneNode* pCharNode = m_pCharaObj->GetSceneNode();

		Vector3 curPos = pCharNode->GetTransform().m_vPos;
		Vector3 vAxisFrom = pCharNode->GetForward();
		Vector3 vAxisTo = vMouveTo - curPos;
		Vec3Normalize(vAxisTo,vAxisTo);
		Vec3Normalize(vAxisFrom,vAxisFrom);

		Vector3 pAxisRot = Vector3(0,1,0);
		float fTargetRota = 0;
		QuaternionFromAxisToAxis(&pAxisRot,&fTargetRota,&vAxisFrom,&vAxisTo);
		fTargetRota *= pAxisRot.y; // y = -1 逆时针转, y=1 顺时针转
		pCharNode->RotateYAxisLS(ToDegree(fTargetRota));

		m_vMoveTo = vMouveTo;
		m_bMoveing = true;

		AnimComponent* pAnimComp = m_pCharaObj->GetTypeComponentFirst<AnimComponent>();
		if (pAnimComp)
		{
			pAnimComp->GetAnimObject()->PlayAnimation("Run");
		}

		Log("curPos = %f,%f,%f,fTargetRota = %f",curPos.x,curPos.y,curPos.z,ToDegree(fTargetRota));
		Log("m_vMoveTo = %f,%f,%f",m_vMoveTo.x,m_vMoveTo.y,m_vMoveTo.z);
	}

	void SampleCharaControl::DoMouveTo()
	{
		if (!m_bMoveing)
			return;

		SceneNode* pCharNode = m_pCharaObj->GetSceneNode();

		float fStepMoveLeng =  m_fMoveSpeed * GetTimer()->GetFrameDeltaTime();

		Vector3 vCurPosWS = pCharNode->GetTransform().m_vPos;
		Vector3 vMoveDir = m_vMoveTo - vCurPosWS;
		float fDistance = Vec3Length(&vMoveDir);
		if (fDistance <= fStepMoveLeng)
		{
			m_bMoveing = false;
			AnimComponent* pAnimComp = m_pCharaObj->GetTypeComponentFirst<AnimComponent>();
			if (pAnimComp)
			{
				pAnimComp->GetAnimObject()->PlayAnimation("Idle");
			}
			return;
		}	

		Vector3 vMoveStepWS = vMoveDir * fStepMoveLeng / fDistance;

		//Log("vCurPosWS = %f,%f,%f \n",vCurPosWS.x,vCurPosWS.y,vCurPosWS.z);
		//Log("m_vMoveTo = %f,%f,%f \n",m_vMoveTo.x,m_vMoveTo.y,m_vMoveTo.z);

		pCharNode->Translate(vMoveStepWS);
	}

	void SampleCharaControl::Render()
	{
	}

	bool SampleCharaControl::mouseMoved( const OIS::MouseEvent &arg )
	{
		return true;
	}

	bool SampleCharaControl::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (id == OIS::MB_Left)
		{
			const OIS::MouseState& mouseState = arg.state;

			Vector2 mousePos(mouseState.X.abs,mouseState.Y.abs);
			Vector2 clientSize( (float)mouseState.width, (float)mouseState.height ); 

			Vector3 rayOrig;
			Vector3 rayDir;
			GetCamera()->GetWorldRayCast(clientSize,mousePos,rayOrig,rayDir);

			Vector3 hitPosWS;
			GameObject* pGameObj = GetPhysicsSystem()->RayCastCloseGameObj(rayOrig,rayDir,0,hitPosWS);
			if (pGameObj == m_pTerrain)
			{
				MoveToWS(hitPosWS);
			}
		}

		return true;
	}

	bool SampleCharaControl::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		return true;
	}


}