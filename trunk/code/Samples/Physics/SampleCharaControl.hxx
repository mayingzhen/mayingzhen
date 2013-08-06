#include "SampleCharaControl.h"

namespace ma
{
	SampleCharaControl::SampleCharaControl()
	{
		m_pScene = NULL;
		m_pCharaObj = NULL;
		m_fMoveSpeed = 2;
	}

	void SampleCharaControl::Load()
	{
		Vector3 vEyePos = Vector3(0, 20, 30);
		Vector3 VAtPos = Vector3(0,0,0); 
		Vector3 vUp = Vector3(0,1,0);
		GetCamera()->LookAt(vEyePos,VAtPos,vUp);

		GetInput()->AddMouseListener(this);

		m_pScene = new Scene();

		m_pScene->GetPhysicsScene()->SetGravity(Vector3(0,-0.98f * 10,0));

		{
			m_pCharaObj = m_pScene->CreateGameObject("Chara");		
			CharaControlComponent* pCharComp = m_pCharaObj->CreateComponent<CharaControlComponent>();
			ICapsuleCollisionShape* pCapsule = pCharComp->GetCharaControll()->GetCollisionShape();
			pCapsule->SetHeight(1.4);
			pCapsule->SetRadius(0.2);

			m_pCharaObj->GetSceneNode()->Translate(Vector3(0,30,0));
		
// 			MeshComponent* pMeshComp = pGameObj->CreateComponent<MeshComponent>();
// 			pMeshComp->Load("magician/Body.skn","magician/Body.tga");
// 			
// 			AnimPlayComponent* pAnimComp = pGameObj->CreateComponent<AnimPlayComponent>();
// 			pAnimComp->Load(NULL,"magician/Body.ske");
// 			IAnimationSet* pAnimSet = pAnimComp->GetAnimPlay()->GetAnimationSet();
// 			pAnimSet->AddAction("gigi/210_run/bip01.ska","Run");
// 			pAnimSet->AddAction("magician/100/bip01.ska","Idle");
// 			pAnimComp->GetAnimPlay()->PlayAnimation("Idle");

			//Matrix4x4 matWorld,matRoat,matScale;
			//MatrixTranslation(&matWorld,-50,120,0);
			//MatrixRotationYawPitchRoll(&matRoat,0,PI * 1.2,0);
			//MatrixScaling(&matScale,0.1,0.1,0.1);
			//matWorld = matWorld * matRoat * matScale;
			//NodeTransform tsfWS;
			
			//pGameObj->GetSceneNode()->Translate();
		}

		{
			m_pTerrain = m_pScene->CreateGameObject("Terrain");
	
			BoxCollisionComponent* pBoxCollisionShape = m_pTerrain->CreateComponent<BoxCollisionComponent>();
			pBoxCollisionShape->GetBoxCollisionShape()->SetSize(Vector3(50,2,50));

			{
				GameObject* pObje = m_pScene->CreateGameObject("Box1");
				BoxCollisionComponent* pComp = pObje->CreateComponent<BoxCollisionComponent>();
				pComp->GetBoxCollisionShape()->SetSize(Vector3(2,2,2));
				pObje->GetSceneNode()->Translate(Vector3(0,2,0));
			}


			{
				GameObject* pObje2 = m_pScene->CreateGameObject("Box2");
				BoxCollisionComponent* pComp2 = pObje2->CreateComponent<BoxCollisionComponent>();
				pComp2->GetBoxCollisionShape()->SetSize(Vector3(2,2,2));
				RigidBodyComponent* pRigidBodyComp = pObje2->CreateComponent<RigidBodyComponent>();
				pObje2->GetSceneNode()->Translate(Vector3(4,4,0));
			}

		}


		m_pScene->Start();
	}

	void SampleCharaControl::UnLoad()
	{
		GetInput()->RemoveMouseListener(this);

		m_pScene->Stop();

		SAFE_DELETE(m_pScene);
	}


	void SampleCharaControl::Update()
	{
		m_pScene->Update(GetTimer()->GetFrameDeltaTime());

		DoMouveTo();
	}

	void SampleCharaControl::MoveToWS(Vector3 vMouveTo)
	{
		SceneNode* pCharNode = m_pCharaObj->GetSceneNode();

		Matrix4x4 matWS = pCharNode->GetWorldMatrix();
		Vector3 curPos = matWS.GetRow(3);
		Vector3 vAxisFrom = matWS.GetRow(2);
		Vector3 vAxisTo = vMouveTo - curPos;
		Vec3Normalize(vAxisTo,vAxisTo);
		Vec3Normalize(vAxisFrom,vAxisFrom);

		Vector3 pAxisRot = Vector3(0,1,0);
		float fTargetRota = 0;
		QuaternionFromAxisToAxis(&pAxisRot,&fTargetRota,&vAxisFrom,&vAxisTo);
		fTargetRota *= pAxisRot.y; // y = -1 逆时针转, y=1 顺时针转
		pCharNode->RotateYAxisLS(fTargetRota);

		m_vMoveTo = vMouveTo;
		m_bMoveing = true;

		AnimPlayComponent* pAnimComp = m_pCharaObj->GetTypeComponentFirst<AnimPlayComponent>();
		if (pAnimComp)
		{
			pAnimComp->GetAnimPlay()->PlayAnimation("Run");
		}

		Log("curPos = %f,%f,%f",curPos.x,curPos.y,curPos.z);
		Log("m_vMoveTo = %f,%f,%f",m_vMoveTo.x,m_vMoveTo.y,m_vMoveTo.z);
	}

	void SampleCharaControl::DoMouveTo()
	{
		if (!m_bMoveing)
			return;

		SceneNode* pCharNode = m_pCharaObj->GetSceneNode();

		float fStepMoveLeng =  m_fMoveSpeed * GetTimer()->GetFrameDeltaTime();

		Vector3 vCurPosWS = pCharNode->GetPosition();
		Vector3 vMoveDir = m_vMoveTo - vCurPosWS;
		float fDistance = Vec3Length(&vMoveDir);
		if (fDistance <= fStepMoveLeng)
		{
			m_bMoveing = false;
			AnimPlayComponent* pAnimComp = m_pCharaObj->GetTypeComponentFirst<AnimPlayComponent>();
			if (pAnimComp)
			{
				pAnimComp->GetAnimPlay()->PlayAnimation("Idle");
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
		m_pScene->Render(GetCamera());
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
			GameObject* pGameObj = m_pScene->GetPhysicsScene()->RayCastCloseGameObj(rayOrig,rayDir,0,hitPosWS);
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