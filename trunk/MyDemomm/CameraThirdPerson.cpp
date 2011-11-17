
#include "CameraThirdPerson.h"

///////////////////////////////////////////////////////////////////////////////
CCameraThirdPerson::CCameraThirdPerson()	:	m_fRadius( 1.0f ),
												m_fRadiusMin( 0.1f ),
												m_fRadiusMax( FLT_MAX )
{
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt )
{
	CCamera::SetViewParams( vEyePt, vLookAtPt );

	m_fRadius = ::D3DXVec3Length( &( m_vLookAtPt - m_vEyePt ) );
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetEyePt( const D3DXVECTOR3 &vEyePt )
{
	CCamera::SetEyePt( vEyePt );

	m_fRadius = ::D3DXVec3Length( &( m_vLookAtPt - m_vEyePt ) );
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetLookAtPt( const D3DXVECTOR3 &vLookAtPt )
{
	CCamera::SetLookAtPt( vLookAtPt );

	m_fRadius = ::D3DXVec3Length( &( m_vLookAtPt - m_vEyePt ) );
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::MoveLookAtPtTo( const D3DXVECTOR3 &vLookAtPt )
{
	m_vLookAtPt = vLookAtPt;

	UpdateEyePt();
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetYaw( float fRadian )
{
	m_fYaw = fRadian;
	//取值范围限制在 -PI ~ PI
	if ( m_fYaw > D3DX_PI )
	{
		m_fYaw = fmodf( m_fYaw + D3DX_PI, 2 * D3DX_PI ) - D3DX_PI;
	}
	else if ( m_fYaw <= -D3DX_PI )
	{
		m_fYaw = fmodf( m_fYaw + D3DX_PI, 2 * D3DX_PI ) + D3DX_PI;
	}

	UpdateEyePt();
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetPitch( float fRadian )
{
	m_fPitch = max( fRadian, m_fPitchMin );
	m_fPitch = min( m_fPitch, m_fPitchMax );

	UpdateEyePt();
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::SetRadius( float fRadius )
{
	m_fRadius = max( fRadius, m_fRadiusMin );
	m_fRadius = min( m_fRadius, m_fRadiusMax );

	UpdateEyePt();
	UpdateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
void CCameraThirdPerson::UpdateEyePt()
{
	float fLen2d = m_fRadius * cosf( m_fPitch );

	m_vEyePt.x = m_vLookAtPt.x - fLen2d * cosf( m_fYaw );
	m_vEyePt.y = m_vLookAtPt.y - m_fRadius * sinf( m_fPitch );
	m_vEyePt.z = m_vLookAtPt.z - fLen2d * sinf( m_fYaw );
}