#include "public.h"
#include "Camera.h"

CCamera g_Camera;

///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera()
{
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


void CCamera::SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt )
{
	m_vEyePt = vEyePt;
	m_vLookAtPt = vLookAtPt;
}

void CCamera::DoMouseControl()
{
	if (!m_ControlState.bRotating)
	{
		::GetCursorPos(&m_ControlState.pntMouse);
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

	float ch = cosf(m_ControlState.fRotX); 
	float sh = sinf(m_ControlState.fRotX); 
	float cp = cosf(m_ControlState.fRotY); 
	float sp = sinf(m_ControlState.fRotY); 
	m_vLookAtPt = m_vEyePt + D3DXVECTOR3(sh*cp, -sp, cp*ch) * fLength;

	m_ControlState.pntMouse = pntMouseCurrent;
}

void CCamera::DoKeyControl()
{
	float fDeltaTime = DeltaTimeUpdate(m_ControlState.fLastUpdate) * 20;

	D3DXVECTOR3 vZ,vX,vY,vTemp;
	D3DXVec3Normalize(&vZ, &(m_vLookAtPt - m_vEyePt) );
	D3DXVec3Normalize(&vX, D3DXVec3Cross(&vTemp, &m_vUpVector, &vZ) );
	D3DXVec3Normalize(&vY, D3DXVec3Cross(&vTemp, &vZ, &vX) );

	// Move forward/backward
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

	// Up/down 
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
}

void CCamera::Update()
{
	//TRACE("pntMouse = %d, %d \n ", m_ControlState.pntMouse.x, m_ControlState.pntMouse.y);

	g_Camera.DoKeyControl();

	m_frustum = CalculateFrustum(m_fNearClip, m_fFarClip);
	CalculateSplitPositions(m_fSplitPos, NUM_PSSM);

	CalculateMatrices();
}

// updates matrices from current settings
void CCamera::CalculateMatrices(void)
{
	D3DXMatrixLookAtLH(&m_mView, &m_vEyePt, &m_vLookAtPt, &m_vUpVector);
	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspect, m_fNearClip, m_fFarClip);
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

