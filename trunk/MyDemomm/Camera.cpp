#include "public.h"
#include "Camera.h"

CCamera g_Camera;

///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera()
{
	memset( &m_ShakeInfo, 0, sizeof ( m_ShakeInfo ) );

	m_vEyePt = D3DXVECTOR3(0,20,-110);
	m_vLookAtPt = D3DXVECTOR3(0,20,-109);
	m_fNearMin = 0.1f;
	m_fFarMax = 8000.0f;
	m_fNearClip = 0.1f;
	m_fFarClip = 8000.0f;
	m_fFOV = DegreeToRadian(45.0f);
	m_vUpVector = D3DXVECTOR3(0, 1, 0);
	ZeroMemory(&m_ControlState, sizeof(ControlState));
}

///////////////////////////////////////////////////////////////////////////////
void CCamera::SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt )
{
	m_vEyePt = vEyePt;
	m_vLookAtPt = vLookAtPt;

	//UpdateYawPitch();
	//UpdateViewMatrix();
}
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetEyePt( const D3DXVECTOR3 &vEyePt )
{
	m_vEyePt = vEyePt;

	UpdateYawPitch();
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCamera::SetLookAtPt( const D3DXVECTOR3 &vLookAtPt )
{
	m_vLookAtPt = vLookAtPt;

	UpdateYawPitch();
	UpdateViewMatrix();
}
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetProjParams( float *pFOV,float *pAspect,float *pNearClip,float *pFarClip)
{
	if (pFOV)
	{
		m_fFOV = *pFOV;
	}
	if (pAspect)
	{
		m_fAspect = *pAspect;
	}
	if (pNearClip)
	{
		m_fNearClip = *pNearClip;
	}
	if (pFarClip)
	{
		m_fFarClip = *pFarClip;
	}
	::D3DXMatrixPerspectiveFovLH(&m_mProj,m_fFOV,m_fAspect,m_fNearClip,m_fFarClip);

}


///////////////////////////////////////////////////////////////////////////////
void CCamera::SetProjParams( float fFOV, float fAspect, float fNearClip, float fFarClip )
{
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNearClip = fNearClip;
	m_fFarClip = fFarClip;

	::D3DXMatrixPerspectiveFovLH( &m_mProj, fFOV, fAspect, fNearClip, fFarClip );
}



///////////////////////////////////////////////////////////////////////////////
void CCamera::UpdateYawPitch()
{
	assert( m_vEyePt != m_vLookAtPt );

	D3DXVECTOR3 vViewDir = m_vLookAtPt - m_vEyePt;

	if ( vViewDir.x != 0.0f || vViewDir.z != 0.0f )
	{
		float fLen2d = sqrtf( vViewDir.x * vViewDir.x + vViewDir.z * vViewDir.z );

		m_fYaw = atan2f( vViewDir.z, vViewDir.x );
		m_fPitch = atanf( vViewDir.y / fLen2d );
	}
	else
	{
		//视线垂直，不改变Yaw
		if ( vViewDir.y >= 0 )
		{
			//向上
			m_fPitch = D3DX_PI / 2;
		}
		else
		{
			//向下
			m_fPitch = -D3DX_PI / 2;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetRoll(float fRadian)
{
	m_fRoll = fRadian;
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCamera::UpdateViewMatrix()
{
	float fLen2d = -sinf( m_fPitch );

	D3DXVECTOR3 vUp( fLen2d * cosf( m_fYaw ), cosf( m_fPitch ), fLen2d * sinf( m_fYaw ) );
	//在绕vLookDir绕圈m_fRoll度
	D3DXVECTOR3 vViewDir = m_vLookAtPt - m_vEyePt;
	D3DXVec3Normalize(&vViewDir,&vViewDir);
	D3DXMATRIX matRoll;
	D3DXMatrixRotationAxis(&matRoll,&vViewDir,m_fRoll);
	D3DXVec3TransformCoord(&vUp,&vUp,&matRoll);
	//vUp = D3DXVECTOR3( 0, 1, 0 ); 

	////摄像机垂直
	////if (D3DX_PI / 2.0f == m_fPitch)
	////Balder,不能判相等，而是应该判一个范围
	//if (  sinf(m_fPitch) > 0.99f)
	//{
	//	vUp = D3DXVECTOR3( -cosf( m_fYaw ), 0, -sinf( m_fYaw ) );	
	//		
	//}
	////else if (-D3DX_PI / 2.0f == m_fPitch)
	//else if ( sinf(m_fPitch) < -0.99f)
	//{
	//	vUp = D3DXVECTOR3( cosf( m_fYaw ), 0, sinf( m_fYaw ) );
	//}

	::D3DXMatrixLookAtLH( &m_mView, &m_vEyePt, &m_vLookAtPt, &vUp );
}

///////////////////////////////////////////////////////////////////////////////
void CCamera::Shake( float fRange, float fFrequency, int iNumCycle )
{
	//tyreal.qiang该处为震屏参数，暂时屏蔽
	m_ShakeInfo.fBeginTime = 0.0f;
	m_ShakeInfo.fFrequency = fFrequency;
	m_ShakeInfo.fRange = fRange;
	m_ShakeInfo.iNumCycle = iNumCycle;
}
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetShakeInfo(float *Range,float *Frequency)
{
	if (Range)
	{
		m_ShakeInfo.fRange = *Range;
	}
	if (Frequency)
	{
		m_ShakeInfo.fFrequency = *Frequency;
	}
}

///////////////////////////////////////////////////////////////////////////////
void CCamera::Update()
{
// 	if ( fTime > m_ShakeInfo.fBeginTime + m_ShakeInfo.iNumCycle / m_ShakeInfo.fFrequency )
// 	{
// 		//不在震动
// 		return;
// 	}
// 	float fRadian = float( D3DX_PI * 2 * ( fTime - m_ShakeInfo.fBeginTime ) *	m_ShakeInfo.fFrequency );
// 	float fOffset = m_ShakeInfo.fRange * sin( fRadian );
// 
// 	//temp by zr，这么写土点
// 	//D3DXVECTOR3 vTemp = m_vEyePt;
// 	m_vLookAtPt.y += fOffset;
// 	m_vEyePt.y += fOffset;
// 	UpdateViewMatrix();


	float fDeltaTime = DeltaTimeUpdate(m_ControlState.fLastUpdate) * 20;

	D3DXVECTOR3 vZ,vX,vY,vTemp;
	D3DXVec3Normalize(&vZ,&(m_vLookAtPt - m_vEyePt));
	D3DXVec3Normalize(&vX,D3DXVec3Cross(&vTemp,&m_vUpVector,&vZ));
	D3DXVec3Normalize(&vY,D3DXVec3Cross(&vTemp,&vZ,&vX));

	// Rotating with mouse left
	//
	//
	if(GetMouseDown(VK_LBUTTON))
	{
		if(!m_ControlState.bRotating)
		{
			::GetCursorPos(&m_ControlState.pntMouse);
			m_ControlState.bZooming = false;
			m_ControlState.bStrafing = false;
			m_ControlState.bRotating = true;
		}

		POINT pntMouseCurrent;
		::GetCursorPos(&pntMouseCurrent);

		D3DXVECTOR3 vTargetToSource = m_vLookAtPt - m_vEyePt;
		float fLength = D3DXVec3Length(&vTargetToSource);

		int addX = pntMouseCurrent.x - m_ControlState.pntMouse.x;
		int aady = pntMouseCurrent.y - m_ControlState.pntMouse.y;

		m_ControlState.fRotX += 0.005f * (float)addX;
		m_ControlState.fRotY += 0.005f * (float)aady;

		m_ControlState.fRotY = Clamp(m_ControlState.fRotY, DegreeToRadian(-89.9f), DegreeToRadian(89.9f));

		//D3DXMATRIX mRotation;
		//mRotation.SetIdentity();
		//mRotation.SetRotation(D3DXVECTOR3(m_ControlState.fRotX, m_ControlState.fRotY,0));
		//D3DXMatrixRotationYawPitchRoll(&mRotation,m_ControlState.fRotX,m_ControlState.fRotY,0);
		//m_vLookAtPt.x = mRotation._31 * fLength + m_vEyePt.x;
		//m_vLookAtPt.y = mRotation._32 * fLength + m_vEyePt.y;
		//m_vLookAtPt.z = mRotation._33 * fLength + m_vEyePt.z;
		float ch = cosf(m_ControlState.fRotX); 
		float sh = sinf(m_ControlState.fRotX); 
		float cp = cosf(m_ControlState.fRotY); 
		float sp = sinf(m_ControlState.fRotY); 
		m_vLookAtPt = m_vEyePt + D3DXVECTOR3(sh*cp, -sp, cp*ch) * fLength;

		m_ControlState.pntMouse = pntMouseCurrent;
	}

	// Move forward/backward
	//
	if(GetKeyDown('W'))
	{
		m_vEyePt += vZ * fDeltaTime;
		m_vLookAtPt += vZ * fDeltaTime;
	}
	else if(GetKeyDown('S'))
	{
		m_vEyePt -= vZ * fDeltaTime;
		m_vLookAtPt -= vZ * fDeltaTime;
	}

	// Strafing
	//
	if(GetKeyDown('D'))
	{
		m_vEyePt += vX * fDeltaTime;
		m_vLookAtPt += vX * fDeltaTime;
	}
	else if(GetKeyDown('A'))
	{
		m_vEyePt -= vX * fDeltaTime;
		m_vLookAtPt -= vX * fDeltaTime;
	}

	// Up/down (many control preferences here :)
	//
	if(GetKeyDown('Q') || GetKeyDown(VK_SPACE))
	{
		m_vEyePt += vY * fDeltaTime;
		m_vLookAtPt += vY * fDeltaTime;
	}
	else if(GetKeyDown('E') || GetKeyDown('C') /*|| GetKeyDown(VK_CONTROL)*/)
	{
		m_vEyePt -= vY * fDeltaTime;
		m_vLookAtPt -= vY * fDeltaTime;
	}

	CalculateMatrices();
}

// updates matrices from current settings
void CCamera::CalculateMatrices(void)
{
	// view matrix
	//m_mView = MatrixLookAtLH(m_vSource, m_vTarget, m_vUpVector);
	D3DXMatrixLookAtLH(&m_mView,&m_vEyePt,&m_vLookAtPt,&m_vUpVector);

	// projection matrix
	//m_mProj = MatrixPerspectiveFovLH(m_fFOV, GetAppBase()->GetAspectRatio(), m_fNear, m_fFar);
	D3DXMatrixPerspectiveFovLH(&m_mProj,m_fFOV,m_fAspect,m_fNearClip,m_fFarClip);
}


CFrustum CCamera::CalculateFrustum(float fNear, float fFar)
{
	D3DXVECTOR3 vX,vY,vZ,vTemp;
	D3DXVec3Normalize( &vZ, &(m_vLookAtPt - m_vEyePt) );
	D3DXVec3Cross(&vTemp, &m_vUpVector, &vZ);
	D3DXVec3Normalize(&vX, &vTemp);
	D3DXVec3Cross(&vTemp, &vZ, &vX);
	D3DXVec3Normalize(&vY, &vTemp);

	float fAspect = m_fAspect;//GetAppBase()->GetAspectRatio();

	float fNearPlaneHalfHeight = tanf(m_fFOV * 0.5f) * fNear;
	float fNearPlaneHalfWidth = fNearPlaneHalfHeight * fAspect;

	float fFarPlaneHalfHeight = tanf(m_fFOV * 0.5f) * fFar;
	float fFarPlaneHalfWidth = fFarPlaneHalfHeight * fAspect;

	D3DXVECTOR3 vNearPlaneCenter = m_vEyePt + vZ * fNear;
	D3DXVECTOR3 vFarPlaneCenter = m_vEyePt + vZ * fFar;

	CFrustum frustum;
	frustum.vPts[0] = D3DXVECTOR3(vNearPlaneCenter - vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);
	frustum.vPts[1] = D3DXVECTOR3(vNearPlaneCenter - vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
	frustum.vPts[2] = D3DXVECTOR3(vNearPlaneCenter + vX*fNearPlaneHalfWidth + vY*fNearPlaneHalfHeight);
	frustum.vPts[3] = D3DXVECTOR3(vNearPlaneCenter + vX*fNearPlaneHalfWidth - vY*fNearPlaneHalfHeight);

	frustum.vPts[4] = D3DXVECTOR3(vFarPlaneCenter - vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);
	frustum.vPts[5] = D3DXVECTOR3(vFarPlaneCenter - vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
	frustum.vPts[6] = D3DXVECTOR3(vFarPlaneCenter + vX*fFarPlaneHalfWidth + vY*fFarPlaneHalfHeight);
	frustum.vPts[7] = D3DXVECTOR3(vFarPlaneCenter + vX*fFarPlaneHalfWidth - vY*fFarPlaneHalfHeight);

	// update frustum AABB
	frustum.CalculateAABB();

	return frustum;
}

void CCamera::CalculateSplitPositions(float *pDistances,int nNumSplits)
{
	// Practical split scheme:
	//
	// CLi = n*(f/n)^(i/numsplits)
	// CUi = n + (f-n)*(i/numsplits)
	// Ci = CLi*(lambda) + CUi*(1-lambda)
	//
	// lambda scales between logarithmic and uniform
	//
	
	float g_fSplitSchemeWeight = 0.5;
	for(int i = 0; i < nNumSplits; i++)
	{
		float fIDM = i / (float)nNumSplits;
		float fLog = m_fNearClip  * powf(m_fFarClip/m_fNearClip, fIDM);
		float fUniform = m_fNearClip + (m_fFarClip - m_fNearClip) * fIDM;
		pDistances[i] = fLog * g_fSplitSchemeWeight + fUniform * (1 - g_fSplitSchemeWeight);
	}

	// make sure border values are accurate
	pDistances[0] = m_fNearClip;
	pDistances[nNumSplits] = m_fFarClip;
}

