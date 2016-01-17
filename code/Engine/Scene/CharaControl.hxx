#include "GamePlayCommon.h"
#include "CharaControl.h"
#include "AttributeManager.h"
#include "Game.h"
#include "AnimationSet.h"
#include "ObjectFactory.h"

 namespace ma
 {
	CharaControl::CharaControl()
	{
		m_fMoveSpeed = 480.0f;
		m_fCharScale = 1.6f;
		
		m_fMoveFrameLength = m_fMoveSpeed  / m_fCharScale;	

		m_strRunAnim = "105";
		m_strIdleAnim = "100";
		m_fCharBaseScale = 0.01f;

		m_vMoveTo = Vector3::ZERO;
		m_bMoveing = false;

		Game::getInstance()->mkeyEvent.notify(this,&CharaControl::keyEvent);
		Game::getInstance()->mMouseEvent.notify(this,&CharaControl::mouseEvent);
	}

	CharaControl::~CharaControl()
	{
		Game::getInstance()->mkeyEvent.remove(this,&CharaControl::keyEvent);
		Game::getInstance()->mMouseEvent.remove(this,&CharaControl::mouseEvent);
	}

	void CharaControl::RegisterAttribute()
	{
		REGISTER_OBJECT(CharaControl,CreateCharaControl); 
		
		ACCESSOR_ATTRIBUTE(CharaControl, "CharaScale", GetCharScale, SetCharScale, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CharaControl, "MoveSpeed", GetMoveSpeed, SetMoveSpeed, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CharaControl, "MoveFrameLength", GetMoveFrameLength, SetMoveFrameLength, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CharaControl, "RunAnim", GetRunAnim, SetRunAnim, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CharaControl, "IdleAnim",GetIdleAnim,SetIdleAnim,const char*, NULL, AM_DEFAULT);
	}

	void CharaControl::OnAddToSceneNode(CNode* pGameObj)
	{
		SetCharScale(m_fCharScale);
	}

	float CharaControl::GetCharScale() const
	{
		return m_fCharScale;
	}

	void CharaControl::SetCharScale(float fScale)
	{
		m_fCharScale = fScale;
		this->GetSceneNode()->SetScale(Vector3(fScale * m_fCharBaseScale));	
	}

	float CharaControl::GetMoveFrameLength() const
	{
		return m_fMoveFrameLength;
	}

	void CharaControl::SetMoveFrameLength(float fMoveFrameLength)
	{
		m_fMoveFrameLength = fMoveFrameLength;

		CSkin* pSkin = dynamic_cast<CSkin*>(this->GetSceneNode());
		if (pSkin == NULL)
			return;

		if (pSkin->GetAnimationSet() == NULL)
			return;

		CAnimNode* pAnimNode = pSkin->GetAnimationSet()->GetAnimationByName(m_strRunAnim.c_str());
		if (pAnimNode == NULL)
			return;

		float fAnimSpeed = m_fMoveSpeed / fMoveFrameLength;
		fAnimSpeed = fAnimSpeed / m_fCharScale;

		pAnimNode->SetPlaySpeed(fAnimSpeed);
	}


	void CharaControl::keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key)
	{

	}

	void CharaControl::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
	{
		switch(evt)
		{
		case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
			{
				Ray ray = GetCamera()->MakeRayFromViewport(x, y);
				CNode* node = NULL;
				if(!GetSceneManager()->ProbeRender(ray, 0xFFFFffff, 100000.f, m_vMoveTo, node))
				{
					break;
				}
				
				if (node == NULL || node != GetTerrain())
					return;

				CNode* pCharNode = this->GetSceneNode();
				if (GetTerrain())
				{
					Vector3 vPosWS = pCharNode->GetWorldPos();
					vPosWS.z = GetTerrain()->GetHeightMap()->GetHeight(vPosWS.x,vPosWS.y);
					pCharNode->SetWorldPos(vPosWS);
				}
				
				Vector3 curPos = pCharNode->GetWorldPos();
				Vector3 vAxisFrom = -Vector3::UNIT_Y;//pCharNode->GetWorldMatrix().GetColumn(1);
				Vector3 vAxisTo = m_vMoveTo - curPos;

				Quaternion qRot = vAxisFrom.getRotationTo(vAxisTo);

				pCharNode->SetWorldRotate(qRot);

				m_bMoveing = true;	
	
				CSkin* pSkin = dynamic_cast<CSkin*>(pCharNode);
				if (pSkin)
				{
					pSkin->SetAnimation(m_strRunAnim.c_str());
				}
			}
			break;
		default:
			break;
		}
	}

	void CharaControl::Process()
	{
		if (!m_bMoveing)
			return;

		CNode* pCharNode = this->GetSceneNode();
		
		Vector3 vCurPosWS = pCharNode->GetWorldPos();
		Vector3 vMoveDir = -pCharNode->GetWorldMatrix().GetColumn(1);
		vMoveDir.normalise();
		
		Plane testPlae(vMoveDir,vCurPosWS);
		float fDistance = testPlae.getDistance(m_vMoveTo);
		if ( fDistance < 0.0001f ) 
		{
			m_bMoveing = false;

			CSkin* pSkin = dynamic_cast<CSkin*>(pCharNode);
			if (pSkin)
			{
				pSkin->SetAnimation(m_strIdleAnim.c_str());
			}
		}
		else
		{
			float fElapsed = (float)GetTimer()->GetElapsed() / 1000.0f;
			float fStepMoveLeng =  (m_fMoveSpeed * 0.01f)* fElapsed;

			Vector3 vNewPosWS = vMoveDir * fStepMoveLeng + vCurPosWS;
			
			if (GetTerrain())
			{
				vNewPosWS.z = GetTerrain()->GetHeightMap()->GetHeight(vNewPosWS.x,vNewPosWS.y);
			}
	
			pCharNode->SetWorldPos(vNewPosWS);
		}
	}

	const char* CharaControl::GetRunAnim() const
	{
		return m_strRunAnim.c_str();
	}

	void CharaControl::SetRunAnim(const char* pszAnimPath)
	{
		m_strRunAnim = pszAnimPath ? pszAnimPath : "";
	}

	const char* CharaControl::GetIdleAnim() const
	{
		return m_strIdleAnim.c_str();
	}

	void CharaControl::SetIdleAnim(const char* pszAnimPath)
	{
		m_strIdleAnim = pszAnimPath ? pszAnimPath : "";
	}


	ref_ptr<CharaControl> CreateCharaControl()
	{
		return new CharaControl();
	}
 }