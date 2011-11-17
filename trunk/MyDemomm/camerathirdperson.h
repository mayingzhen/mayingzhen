#ifndef __CAMERA_THIRD_PERSON_H__
#define __CAMERA_THIRD_PERSON_H__

#include "Camera.h"


class CCameraThirdPerson : public CCamera
{
public:
	CCameraThirdPerson();

public:
	virtual void SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt );

	//����LookAtPt����
	virtual void SetEyePt( const D3DXVECTOR3 &vEyePt );
	//����EyePt����
	virtual void SetLookAtPt( const D3DXVECTOR3 &vLookAtPt );

	//�ƶ������ַ��򲻱䣩
	void MoveLookAtPtTo( const D3DXVECTOR3 &vLookAtPt );

	//����
	virtual void SetYaw( float fRadian );

	//����
	virtual void SetPitch( float fRadian );

	inline float GetRadius() const;
	inline void	ChangeRadius( float fChange );
	void		SetRadius( float fRadius );

	inline void SetRadiusLimit( float fMin, float fMax );

private:
	void UpdateEyePt();

private:
	float		m_fRadius;		//��������ӵ����ľ���
	float		m_fRadiusMin;		//��С����
	float		m_fRadiusMax;		//������
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