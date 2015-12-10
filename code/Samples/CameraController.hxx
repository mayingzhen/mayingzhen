#include "CameraController.h"




namespace ma
{
	CameraController::CameraController(Camera* pCamera)
	{
		m_pCamera = pCamera;

		m_fMoveSpeed = 10.f;
		m_fRotateSpeed = 10.0f;

		for (int i = 0;i< KEY_AMOUNT;++i)
		{
			m_rgKeyDown[i] = false;
		}

		m_eMode = MODE_FREE;
		m_vPivot = Vector3::ZERO;
		m_bAltDown = false;

		ResetCamera();

		Game::GetInstance().mkeyEvent.notify(this,&CameraController::keyEvent);
		Game::GetInstance().mMouseEvent.notify(this,&CameraController::mouseEvent);
	}

	CameraController::~CameraController(void)
	{
		Game::GetInstance().mkeyEvent.remove(this,&CameraController::keyEvent);
		Game::GetInstance().mMouseEvent.remove(this,&CameraController::mouseEvent);
	}

	void CameraController::ResetCamera()
	{
		if (m_pCamera == NULL)
			return;

		m_pCamera->LookAt(Vector3(0, 200, -300), Vector3(0, 0, 0));

		int nWndWidth,nWndHeigh;
		Platform::GetInstance().GetWindowSize(nWndWidth,nWndHeigh);
		float fFOV = DegreesToRadians(50.0f);
		float fAspect = (float)nWndWidth / (float)nWndHeigh;
		float fNearClip = 0.10f;
		float fFarClip = 10000.0f;
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	Vector3	CameraController::ProjToWorldNormal(const Vector2* pVec,float fDepth) 
	{
		Vector3 vRet;
		const Matrix4& matVPInv = m_pCamera->GetMatViewProjInv();
		Vector3 p0(0.0f,0.0f,1.0f);
		p0 = matVPInv*p0;
		Vector3 p1(pVec->x,pVec->y,1.0f);	
		p1 = matVPInv*p1;
		vRet = p1 - p0;
		vRet *= fDepth / m_pCamera->GetFarClip();
		return vRet;
	}

	float g_fParam = 100.0f;
	void CameraController::mouseEvent( Mouse::MouseEvent evt, int x, int y, int wheelDelta )
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
					s_mbuttonPos = m_pCamera->GetPosWS();
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
					vScreenOffset.x = -vScreenOffset.x*2.f/ Game::GetInstance().m_OnWindowSizedWidth;
					vScreenOffset.y = vScreenOffset.y*2.f/ Game::GetInstance().m_OnWindowSizedHeight;
					Vector3 v = ProjToWorldNormal(&vScreenOffset, s_fPivotDepth);
					m_pCamera->SetPosWS(s_mbuttonPos + v);
				}

				if (s_bRButtonDown)
				{
					Vector2 vDir((float)s_rbuttonCursorPoint.x - x, (float)s_rbuttonCursorPoint.y - y);
					s_rbuttonCursorPoint.x = x;
					s_rbuttonCursorPoint.y = y;
					
					this->Yaw(300.f * vDir.x / Game::GetInstance().m_OnWindowSizedWidth);
					this->Pitch(80.0f * vDir.y / Game::GetInstance().m_OnWindowSizedHeight);
				}
			}
			break;
		default:
			break;
		}

		return;
	}

	void CameraController::keyEvent( Keyboard::KeyEvent evt, int key )
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

	void CameraController::Process(float dwElapsed)
	{
		uint8 nMultiple = 1;
	#ifdef WIN32
		if (GetKeyState(VK_SHIFT) & 0x80000)
		{
			nMultiple = 4;
		}
	#endif

		float fMoveSpeed = m_fMoveSpeed*dwElapsed*nMultiple;
		if(m_rgKeyDown[UP])
		{
			Vector3 vPos = m_pCamera->GetPos();
			Vector3 vRight = m_pCamera->GetRight();
			Vector3 vY = Vector3::UNIT_Z.crossProduct(vRight);
			vPos += vY.normalisedCopy()*fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}
		if(m_rgKeyDown[DOWN])
		{
			Vector3 vPos = m_pCamera->GetPos();
			Vector3 vRight = m_pCamera->GetRight();
			Vector3 vY = Vector3::UNIT_Z.crossProduct(vRight);
			vPos -= vY.normalisedCopy()*fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}
		if(m_rgKeyDown[LEFT])
		{
			Vector3 vPos = m_pCamera->GetPos();
			vPos -= m_pCamera->GetRight().normalisedCopy()*fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}
		if(m_rgKeyDown[RIGHT])
		{
			Vector3 vPos = m_pCamera->GetPos();
			vPos += m_pCamera->GetRight().normalisedCopy()*fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}
		if (m_rgKeyDown[FORWARD])
		{
			m_pCamera->Forward(fMoveSpeed);
		}
		if (m_rgKeyDown[BACKWARD])
		{
			m_pCamera->Forward(-fMoveSpeed);
		}
		if(m_rgKeyDown[HEIGHT_UP])
		{
			Vector3 vPos = m_pCamera->GetPos();
			vPos.z += fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}
		if(m_rgKeyDown[HEIGHT_DOWN])
		{
			Vector3 vPos = m_pCamera->GetPos();
			vPos.z -= fMoveSpeed;
			m_pCamera->SetPos(vPos);
		}


		float fRotateSpeed = m_fRotateSpeed*dwElapsed*nMultiple;
		if(m_rgKeyDown[YAW_LEFT])
		{
			this->Yaw(fRotateSpeed);
		}
		if(m_rgKeyDown[YAW_RIGHT])
		{
			this->Yaw(-fRotateSpeed);
		}
		if(m_rgKeyDown[PITCH_UP])
		{
			this->Pitch(fRotateSpeed);
		}
		if(m_rgKeyDown[PITCH_DOWN])
		{
			this->Pitch(-fRotateSpeed);
		}
	}

	void CameraController::Yaw( float r )
	{
		switch(m_eMode)
		{
		case MODE_FREE:
			m_pCamera->Yaw(r,true);
			break;
		case MODE_PIVOT:
			{
// 				Vector3 vCameraPos = m_pCamera->GetPosWS();
// 				Vector3 vEyePos = m_pCamera->GetEyeNode()->GetWorldPos();
// 				vCameraPos.rotateXYBy(r, m_vPivot);
// 				m_pCamera->SetWorldPos(vCameraPos);
// 				vEyePos.rotateXYBy(r, m_vPivot);
// 				m_pCamera->GetEyeNode()->SetWorldPos(vEyePos);
			}
			break;
		default:
			break;
		}
	}

	void CameraController::SetSpeed( float nMultiple )
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

	void CameraController::Pitch( float r )
	{
		switch(m_eMode)
		{
		case MODE_FREE:
			m_pCamera->Pitch(r,true);
			break;
		case MODE_PIVOT:
			{
// 				Vector3 vDirEye = m_pCamera->GetEyeNode()->GetWorldPos() - m_vPivot;
// 				Vector3 vDirCamera = m_pCamera->GetWorldPos() - m_vPivot;
// 				Vector3 vLook = m_pCamera->GetLookVector()*-1.f;
// 				Vector3 vRight = vLook.crossProduct(Vector3::UNIT_Z);
// 				vRight.normalise();
// 				Quaternion qEye(r, vRight);
// 				Vector3 vDestEye = qEye*vDirEye;
// 				Vector3 vDestCamera = qEye*vDirCamera;
// 
// 				if (IsValidDir(vDestEye - vDestCamera))
// 				{
// 					m_pCamera->SetWorldPos(vDestCamera + m_vPivot);
// 					m_pCamera->GetEyeNode()->SetWorldPos(vDestEye + m_vPivot);
// 				}
			}
			break;
		default:
			break;
		}
	}

}
