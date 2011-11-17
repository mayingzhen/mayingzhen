#ifndef __CAMERA_THIRD_PERSON_H__
#define __CAMERA_THIRD_PERSON_H__

#include "Camera.h"


class CCameraThirdPerson : public CCamera
{
public:
	CCameraThirdPerson();

public:
	virtual void SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt );

	//保持LookAtPt不变
	virtual void SetEyePt( const D3DXVECTOR3 &vEyePt );
	//保持EyePt不变
	virtual void SetLookAtPt( const D3DXVECTOR3 &vLookAtPt );

	//移动（保持方向不变）
	void MoveLookAtPtTo( const D3DXVECTOR3 &vLookAtPt );

	//方向
	virtual void SetYaw( float fRadian );

	//俯仰
	virtual void SetPitch( float fRadian );

	inline float GetRadius() const;
	inline void	ChangeRadius( float fChange );
	void		SetRadius( float fRadius );

	inline void SetRadiusLimit( float fMin, float fMax );

private:
	void UpdateEyePt();

private:
	float		m_fRadius;		//摄像机到视点中心距离
	float		m_fRadiusMin;		//最小距离
	float		m_fRadiusMax;		//最大距离
};

///////////////////////////////////////////////////////////////////////////////
inline float CCameraThirdPerson::GetRadius() const
{
	return m_fRadius;
}

///////////////////////////////////////////////////////////////////////////////
inline void CCameraThirdPerson::ChangeRadius( float fChange )
{
	SetRadius( m_fRadius + fChange );
}

///////////////////////////////////////////////////////////////////////////////
inline void CCameraThirdPerson::SetRadiusLimit( float fMin, float fMax )
{
	fMax = max( fMin, fMax );
	m_fRadiusMin = max( 0.1f, fMin );
	m_fRadiusMax = fMax;
}

#endif