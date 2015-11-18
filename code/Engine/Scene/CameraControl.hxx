#include "GamePlayCommon.h"
#include "CameraControl.h"
#include "SceneManager.h"
#include "Timer.h"
#include "RenderSystem.h"
namespace ma
{
CCameraControl::CCameraControl(void)
{
	m_fMoveSpeed = 10.f;
	m_fRotateSpeed = 0.03f;

	for (int i = 0;i< KEY_AMOUNT;++i)
	{
		m_rgKeyDown[i] = false;
	}

	m_eMode = MODE_FREE;
	m_vPivot = Vector3::ZERO;
	m_bAltDown = false;
}

CCameraControl::~CCameraControl(void)
{
}

Vector3	ProjToWorldNormal(const Vector2* pVec,float fDepth) 
{
	Vector3 vRet;
	const Matrix4& matVPInv = GetCamera()->GetViewProjInv();
	Vector3 p0(0.0f,0.0f,1.0f);
	p0 = matVPInv*p0;
	Vector3 p1(pVec->x,pVec->y,1.0f);	
	p1 = matVPInv*p1;
	vRet = p1 - p0;
	vRet *= fDepth / GetCamera()->GetZFar();
	return vRet;
}

float g_fParam = 100.0f;
bool CCameraControl::mouseEvent( Mouse::MouseEvent evt, int x, int y, int wheelDelta )
{
	static bool s_bMButtonDown = false;
	static Vector2 s_mbuttonCursorPoint;
	static Vector3 s_mbuttonPos;
	static float s_fPivotDepth = 0.f;

	static bool s_bRButtonDown = false;
	static Vector2 s_rbuttonCursorPoint;
	switch(evt)
	{
	case Mouse::MOUSE_WHEEL:
		{
			int zDelta = (int)wheelDelta;

			float fSpeed = 1.0f;
#ifdef WIN32
			if (GetKeyState(VK_SHIFT) & 0x80000)
			{
				fSpeed*=4;
			}
#endif
			m_pCamera->Forward(zDelta>0?fSpeed:-fSpeed);
		}
		break;
	case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
		{
			s_bRButtonDown = true;
			s_rbuttonCursorPoint.x = x;
			s_rbuttonCursorPoint.y = y;
		}
		break;
	case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
		{
			s_bRButtonDown = false;
		}
		break;
	case Mouse::MOUSE_PRESS_MIDDLE_BUTTON:
		{
#ifdef WIN32
			if (::GetKeyState(VK_MENU) & 0x80000)
			{
				s_bRButtonDown = true;
				s_rbuttonCursorPoint.x = x;
				s_rbuttonCursorPoint.y = y;
			}
			else
#endif
			{
				s_bMButtonDown = true;
				if (m_eMode == MODE_FREE)
				{
					s_fPivotDepth = 20.f;
				}
				else
				{
					s_fPivotDepth = (m_pCamera->GetPosWS() - m_vPivot).length();
				}
				s_mbuttonCursorPoint.x= x;
				s_mbuttonCursorPoint.y= y;
				s_mbuttonPos = m_pCamera->GetPos();
			}
			
			
		}
		break;
	case Mouse::MOUSE_RELEASE_MIDDLE_BUTTON:
		{
			s_bMButtonDown = false;
			s_bRButtonDown = false;
		}
		break;
	case Mouse::MOUSE_MOVE:
		{
			if (s_bMButtonDown)
			{
				Vector2 vScreenOffset(x - (float)s_mbuttonCursorPoint.x, y - (float)s_mbuttonCursorPoint.y);
				vScreenOffset.x = -vScreenOffset.x*2.f/GetRenderWindow()->GetWidth();
				vScreenOffset.y = vScreenOffset.y*2.f/GetRenderWindow()->GetHeight();
				Vector3 v = ProjToWorldNormal(&vScreenOffset, s_fPivotDepth);
				GetCamera()->SetPos(s_mbuttonPos + v);
			}

			if (s_bRButtonDown)
			{
				Vector2 vDir((float)s_rbuttonCursorPoint.x - x, (float)s_rbuttonCursorPoint.y - y);
				s_rbuttonCursorPoint.x = x;
				s_rbuttonCursorPoint.y = y;
				
				this->Yaw(Radian(Degree(360.f*vDir.x/GetRenderWindow()->GetWidth())));
				this->Pitch(Radian(Degree(180.f*vDir.y/GetRenderWindow()->GetHeight())));
			}
		}
		break;
    default:
        break;
	}

	return false;
}

void CCameraControl::keyEvent( Keyboard::KeyEvent evt, int key )
{
	switch(evt)
	{
	case Keyboard::KEY_PRESS:
		{
			switch(key)
			{
			case 'W':
			case 'w':
				{
					m_rgKeyDown[UP] = true;
				}
				break;
			case 'S':
			case 's':
				{
					m_rgKeyDown[DOWN] = true;
				}
				break;
			case 'A':
			case 'a':
			case Keyboard::KEY_LEFT_ARROW:
				{
					m_rgKeyDown[LEFT] = true;
				}
				break;
			case 'D':
			case 'd':
			case Keyboard::KEY_RIGHT_ARROW:
				{
					m_rgKeyDown[RIGHT] = true;
				}
				break;
			case 'Q':
			case 'q':
				{
					m_rgKeyDown[YAW_LEFT] = true;
				}
				break;
			case 'E':
			case 'e':
				{
					m_rgKeyDown[YAW_RIGHT] = true;
				}
				break;
			case 'R':
			case 'r':
				{
					m_rgKeyDown[HEIGHT_UP] = true;
				}
				break;
			case 'F':
			case 'f':
				{
					m_rgKeyDown[HEIGHT_DOWN] = true;
				}
				break;
			case 'T':
			case 't':
				{
					m_rgKeyDown[PITCH_UP] = true;
				}
				break;
			case 'G':
			case 'g':
				{
					m_rgKeyDown[PITCH_DOWN] = true;
				}
				break;
			case Keyboard::KEY_UP_ARROW:
				{
					m_rgKeyDown[FORWARD] = true;
				}
				break;
			case Keyboard::KEY_DOWN_ARROW:
				{
					m_rgKeyDown[BACKWARD] = true;
				}
				break;
			}
		}
		break;
	case Keyboard::KEY_RELEASE:
		{
			switch(key)
			{
			case 'W':
			case 'w':
				{
					m_rgKeyDown[UP] = false;
				}
				break;
			case 'S':
			case 's':
				{
					m_rgKeyDown[DOWN] = false;
				}
				break;
			case 'A':
			case 'a':
			case Keyboard::KEY_LEFT_ARROW:
				{
					m_rgKeyDown[LEFT] = false;
				}
				break;
			case 'D':
			case 'd':
			case Keyboard::KEY_RIGHT_ARROW:
				{
					m_rgKeyDown[RIGHT] = false;
				}
				break;
			case 'Q':
			case 'q':
				{
					m_rgKeyDown[YAW_LEFT] = false;
				}
				break;
			case 'E':
			case 'e':
				{
					m_rgKeyDown[YAW_RIGHT] = false;
				}
				break;
			case 'R':
			case 'r':
				{
					m_rgKeyDown[HEIGHT_UP] = false;
				}
				break;
			case 'F':
			case 'f':
				{
					m_rgKeyDown[HEIGHT_DOWN] = false;
				}
				break;
			case 'T':
			case 't':
				{
					m_rgKeyDown[PITCH_UP] = false;
				}
				break;
			case 'G':
			case 'g':
				{
					m_rgKeyDown[PITCH_DOWN] = false;
				}
				break;
			case Keyboard::KEY_UP_ARROW:
				m_rgKeyDown[FORWARD] = false;
				break;
			case Keyboard::KEY_DOWN_ARROW:
				m_rgKeyDown[BACKWARD] = false;
				break;
			}
		}
		break;
	}
}

void CCameraControl::Process(uint32 dwElapsed)
{
	uint8 nMultiple = 1;
#ifdef WIN32
	if (GetKeyState(VK_SHIFT) & 0x80000)
	{
		nMultiple = 4;
	}
#endif

	float fMoveSpeed = m_fMoveSpeed*dwElapsed*0.001f*nMultiple;
	if(m_rgKeyDown[UP])
	{
		Vector3 vPos = GetCamera()->GetPos();
		Vector3 vRight = GetCamera()->GetRightVector();
		Vector3 vY = Vector3::UNIT_Z.crossProduct(vRight);
		vPos += vY.normalisedCopy()*fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}
	if(m_rgKeyDown[DOWN])
	{
		Vector3 vPos = GetCamera()->GetPos();
		Vector3 vRight = GetCamera()->GetRightVector();
		Vector3 vY = Vector3::UNIT_Z.crossProduct(vRight);
		vPos -= vY.normalisedCopy()*fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}
	if(m_rgKeyDown[LEFT])
	{
		Vector3 vPos = GetCamera()->GetPos();
		vPos -= GetCamera()->GetRightVector().normalisedCopy()*fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}
	if(m_rgKeyDown[RIGHT])
	{
		Vector3 vPos = GetCamera()->GetPos();
		vPos += GetCamera()->GetRightVector().normalisedCopy()*fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}
	if (m_rgKeyDown[FORWARD])
	{
		GetCamera()->Forward(fMoveSpeed);
	}
	if (m_rgKeyDown[BACKWARD])
	{
		GetCamera()->Forward(-fMoveSpeed);
	}
	if(m_rgKeyDown[HEIGHT_UP])
	{
		Vector3 vPos = GetCamera()->GetPos();
		vPos.z += fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}
	if(m_rgKeyDown[HEIGHT_DOWN])
	{
		Vector3 vPos = GetCamera()->GetPos();
		vPos.z -= fMoveSpeed;
		GetCamera()->SetPos(vPos);
	}


	float fRotateSpeed = m_fRotateSpeed*dwElapsed*0.05f*nMultiple;
	if(m_rgKeyDown[YAW_LEFT])
	{
		this->Yaw(Radian(fRotateSpeed));
	}
	if(m_rgKeyDown[YAW_RIGHT])
	{
		this->Yaw(Radian(-fRotateSpeed));
	}
	if(m_rgKeyDown[PITCH_UP])
	{
		this->Pitch(Radian(fRotateSpeed));
	}
	if(m_rgKeyDown[PITCH_DOWN])
	{
		this->Pitch(Radian(-fRotateSpeed));
	}

}

void CCameraControl::Yaw( const Radian& r )
{
	switch(m_eMode)
	{
	case MODE_FREE:
		GetCamera()->Yaw(r);
		break;
	case MODE_PIVOT:
		{
			Vector3 vCameraPos = GetCamera()->GetWorldPos();
			Vector3 vEyePos = GetCamera()->GetEyeNode()->GetWorldPos();
			vCameraPos.rotateXYBy(r, m_vPivot);
			GetCamera()->SetWorldPos(vCameraPos);
			vEyePos.rotateXYBy(r, m_vPivot);
			GetCamera()->GetEyeNode()->SetWorldPos(vEyePos);
		}
		break;
	default:
		break;
	}
}

void CCameraControl::SetSpeed( float nMultiple )
{
	m_fMoveSpeed = 10.f*nMultiple;
	m_fRotateSpeed = 0.03f*nMultiple;
}

bool IsValidDir(const Vector3& dir)
{
	Vector2 xy(dir.x, dir.y);
	float angle = atan(xy.normalise()/dir.z);
	if (Math::Abs(angle) < 0.1f)
	{
		return false;
	}

	return true;
}

void CCameraControl::Pitch( const Radian& r )
{
	switch(m_eMode)
	{
	case MODE_FREE:
		GetCamera()->Pitch(r);
		break;
	case MODE_PIVOT:
		{
			Vector3 vDirEye = GetCamera()->GetEyeNode()->GetWorldPos() - m_vPivot;
			Vector3 vDirCamera = GetCamera()->GetWorldPos() - m_vPivot;
			Vector3 vLook = GetCamera()->GetLookVector()*-1.f;
			Vector3 vRight = vLook.crossProduct(Vector3::UNIT_Z);
			vRight.normalise();
			Quaternion qEye(r, vRight);
			Vector3 vDestEye = qEye*vDirEye;
			Vector3 vDestCamera = qEye*vDirCamera;

			if (IsValidDir(vDestEye - vDestCamera))
			{
				GetCamera()->SetWorldPos(vDestCamera + m_vPivot);
				GetCamera()->GetEyeNode()->SetWorldPos(vDestEye + m_vPivot);
			}
		}
		break;
	default:
		break;
	}
}


}