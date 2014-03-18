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
		GetCamera()->GetSceneNode()->LookAt(vEyePos,VAtPos,vUp);

		GetInput()->AddTouchListener(this);
		GetInput()->AddMouseListener(this);

		{
			m_pCharaObj = GetEntitySystem()->CreateGameObject("Chara");		
			ICharaControllPtr pCharComp = m_pCharaObj->CreateComponent<ICharaControll>();
			ICapsuleCollisionShape* pCapsule = pCharComp->GetCollisionShape();
		
			MeshComponentPtr pMeshComp = m_pCharaObj->CreateComponent<MeshComponent>();
			pMeshComp->Load("magician/Body.skn","magician/Body.mat");
		
			AABB aabb = pMeshComp->GetAABBWS();

			Vector3 vSize = aabb.Size();
			float fRadius = abs(vSize.x) / 2.0f;
			pCapsule->SetHeight(abs(vSize.y) - 2 * fRadius);
			pCapsule->SetRadius(fRadius);
	
			NodeTransform tsfLS;
			TransformSetIdentity(&tsfLS);
			tsfLS.m_vPos = aabb.Center();
			pCapsule->SetTransformLS(tsfLS);


			IAnimationObjectPtr pAnimComp = m_pCharaObj->CreateComponent<IAnimationObject>();
			pAnimComp->Load("magician/Body.Aniset","magician/Body.ske");
			IAnimationSet* pAnimSet = pAnimComp->GetAnimationSet();
			pAnimSet->AddAnimClip("gigi/210_run/bip01.ska","Run");
			pAnimSet->AddAnimClip("magician/100/bip01.ska","Idle");
			pAnimComp->PlayAnimation("Idle");

			m_pCharaObj->GetSceneNode()->Translate(Vector3(0,20,0));
		}

		{
			m_pTerrain = GetEntitySystem()->CreateGameObject("Terrain");
	
			IBoxCollisionShapePtr pBoxCollisionShape = m_pTerrain->CreateComponent<IBoxCollisionShape>();
			pBoxCollisionShape->SetSize(Vector3(1800,20,1800));

			{
				GameObjectPtr pObje = GetEntitySystem()->CreateGameObject("Box1");
				IBoxCollisionShapePtr pBox = pObje->CreateComponent<IBoxCollisionShape>();
				pBox->SetSize(Vector3(200,200,200));
				pObje->GetSceneNode()->Translate(Vector3(-100,20,0));
			}


			{
				GameObjectPtr pObje2 = GetEntitySystem()->CreateGameObject("Box2");
				IBoxCollisionShapePtr pBox = pObje2->CreateComponent<IBoxCollisionShape>();
				pBox->SetSize(Vector3(200,200,200));
				IRigidBodyPtr pRigidBodyComp = pObje2->CreateComponent<IRigidBody>();
				pObje2->GetSceneNode()->Translate(Vector3(140,40,0));
			}

		}

	}

	void SampleCharaControl::UnLoad()
	{
		GetInput()->RemoveTouchListener(this);
		GetInput()->RemoveMouseListener(this);
	}


	void SampleCharaControl::Update()
	{
		DoMouveTo();
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
			IAnimationObjectPtr pAnimComp = m_pCharaObj->GetTypeComponentFirst<IAnimationObject>();
			if (pAnimComp)
			{
				pAnimComp->PlayAnimation("Idle");
			}
			return;
		}	

		Vector3 vMoveStepWS = vMoveDir * fStepMoveLeng / fDistance;


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
		return true;
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

	bool SampleCharaControl::touchMoved( const OIS::MultiTouchEvent &arg )
	{
		return true;
	}

	bool SampleCharaControl::touchPressed( const OIS::MultiTouchEvent &arg )
	{
		return true;
	}

	bool SampleCharaControl::touchReleased( const OIS::MultiTouchEvent &arg )
	{
		Vector2 mousePos(arg.state.X.abs,arg.state.Y.abs);

		OnTouch(mousePos);
	
		return true;
	}

	bool SampleCharaControl::touchCancelled( const OIS::MultiTouchEvent &arg )
	{
		return true;
	}

	void SampleCharaControl::OnTouch(Vector2 vTouchPos)
	{
		int width,height;
		Platform::GetInstance().GetWindowSize(width,height);
		Vector2 clientSize( (float)width, (float)height ); 

		Vector3 rayOrig;
		Vector3 rayDir;
		GetCamera()->GetWorldRayCast(clientSize,vTouchPos,rayOrig,rayDir);

		Vector3 hitPosWS;
		GameObject* pGameObj = GetPhysicsSystem()->RayCastCloseGameObj(rayOrig,rayDir,0,hitPosWS);
		if (pGameObj == m_pTerrain)
		{
			SceneNode* pCharNode = m_pCharaObj->GetSceneNode();

			Vector3 curPos = pCharNode->GetTransform().m_vPos;
			Vector3 vAxisFrom = pCharNode->GetForward();
			Vector3 vAxisTo = hitPosWS - curPos;
			Vec3Normalize(vAxisTo,vAxisTo);
			Vec3Normalize(vAxisFrom,vAxisFrom);

			Vector3 pAxisRot = Vector3(0,1,0);
			float fTargetRota = 0;
			QuaternionFromAxisToAxis(&pAxisRot,&fTargetRota,&vAxisFrom,&vAxisTo);
			fTargetRota *= pAxisRot.y; // y = -1 逆时针转, y=1 顺时针转
			pCharNode->RotateYAxisLS(ToDegree(fTargetRota));

			m_vMoveTo = hitPosWS;
			m_bMoveing = true;

			IAnimationObjectPtr pAnimComp = m_pCharaObj->GetTypeComponentFirst<IAnimationObject>();
			if (pAnimComp)
			{
				pAnimComp->PlayAnimation("Run");
			}

			Log("curPos = %f,%f,%f,fTargetRota = %f",curPos.x,curPos.y,curPos.z,ToDegree(fTargetRota));
			Log("m_vMoveTo = %f,%f,%f",m_vMoveTo.x,m_vMoveTo.y,m_vMoveTo.z);
		}
	}

}