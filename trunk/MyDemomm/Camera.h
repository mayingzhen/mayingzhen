#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once

#include <d3dx9.h>
#include <assert.h>

#include "bounding.h"

struct SCameraShakeInfo
{
	double		fBeginTime;	//��ʼʱ��
	float		fRange;		//����
	float		fFrequency;	//Ƶ��
	int			iNumCycle;	//������
};

class CCamera
{
public:
	CCamera();

public:

	CFrustum CalculateFrustum(float fNear, float fFar);
	void CalculateSplitPositions(float *pDistances,int nNumSplits);
	void CalculateMatrices();

	inline const D3DXMATRIX&  GetViewMatrix()	const;
    inline const D3DXMATRIX&  GetProjMatrix()	const;

    inline const D3DXVECTOR3& GetEyePt()	const;
    inline const D3DXVECTOR3& GetLookAtPt()	const;

	inline float GetYaw()	const;
	virtual void SetYaw(float fRadian){};
	inline void  ChangeYaw(float fRadian);

	inline float GetPitch() const;
	virtual void SetPitch(float fRadian){};
	inline void  ChangePitch(float fRadian);
	
	//balder ����Roll��
	inline float GetRoll() const;
	virtual void SetRoll(float fRadian);
	inline void  ChangeRoll(float fRadian);

	virtual void SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt );

	//����LookAtPt����
	virtual void SetEyePt( const D3DXVECTOR3 &vEyePt );
	//����EyePt����
	virtual void SetLookAtPt( const D3DXVECTOR3 &vLookAtPt );

	inline void SetPitchLimit( float fMin, float fMax );

	// ͸��ͶӰ 
	void SetProjParams( float fFOV, float fAspect, float fNearClip, float fFarClip );
	//balder���һ��ָ������ӿڣ������Ϳ��Խ��޸�һ���ֲ���
	void SetProjParams( float *pFOV,float *pAspect,float *pNearClip,float *pFarClip);

	inline float GetFOV()		const;
	inline float GetAspect()	const;
	inline float GetNearClip()	const;
	inline float GetFarClip()	const;

	inline void GetVectorDir( D3DXVECTOR3* pVector );
	inline void GetVectorUp( D3DXVECTOR3* pVector );

	//��
	//fRange		�𶯷���
	//fFrequency	��Ƶ��
	//iNumCycle		��������
	void Shake( float fRange, float fFrequency, int iNumCycle );
	void SetShakeInfo(float *Range,float *Frequency);

	void Update();

	void SetViewMatrix( D3DXMATRIX& matIn )
	{
		m_mView = matIn;
	}
	void SetProjMatrix( D3DXMATRIX& matIn )
	{
		m_mProj = matIn;
	}

	void UpdateViewMatrix();

protected:
	void UpdateYawPitch();

public:
	//ͶӰ����
	float		m_fFOV;			//�ӽ�
	float		m_fAspect;		//�ӿڿ�߱���
	float		m_fNearClip;	//�����������
	float		m_fFarClip;		//Զ���������

	float		m_fNearMin;
	float		m_fFarMax;

	D3DXVECTOR3	m_vEyePt;		//�����λ��
	D3DXVECTOR3	m_vLookAtPt;	//�ӵ�����λ��
	D3DXVECTOR3	m_vUpVector;

	float		m_fYaw;			//�����
	float		m_fPitch;		//������
	float		m_fPitchMin;	//��С����
	float		m_fPitchMax;	//�����
	float		m_fRoll;//��ת�Ƕ�

	D3DXMATRIX	m_mView;		//�Ӿ���
	D3DXMATRIX	m_mProj;		//ͶӰ����

	SCameraShakeInfo	m_ShakeInfo;	//����Ϣ

private:
	// camera control states
	struct ControlState
	{
		bool bZooming;
		bool bStrafing;
		bool bRotating;
		POINT pntMouse;
		double fLastUpdate;
		float fRotX;
		float fRotY;
	};

	ControlState m_ControlState;

};

///////////////////////////////////////////////////////////////////////////////
inline const D3DXMATRIX& CCamera::GetViewMatrix() const
{
	return m_mView;
}

///////////////////////////////////////////////////////////////////////////////
inline const D3DXMATRIX& CCamera::GetProjMatrix() const 
{
	return m_mProj;
}

///////////////////////////////////////////////////////////////////////////////
inline const D3DXVECTOR3& CCamera::GetEyePt() const
{
	return m_vEyePt;
}

///////////////////////////////////////////////////////////////////////////////
inline const D3DXVECTOR3& CCamera::GetLookAtPt() const
{
	return m_vLookAtPt;
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetFOV() const
{
	return m_fFOV;
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetAspect() const
{
	return m_fAspect;
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetNearClip() const
{
	return m_fNearClip;
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetFarClip() const
{
	return m_fFarClip;
}

///////////////////////////////////////////////////////////////////////////////
inline void CCamera::GetVectorDir( D3DXVECTOR3* pVector )
{
	assert( pVector != NULL );
	::D3DXVec3Normalize( pVector, &( m_vLookAtPt - m_vEyePt ) );
}

///////////////////////////////////////////////////////////////////////////////
inline void CCamera::GetVectorUp( D3DXVECTOR3* pVector )
{
	assert( pVector != NULL );
	float fLen2d = -sinf( m_fPitch );
	pVector->x = fLen2d * cosf( m_fYaw );
	pVector->y = cosf( m_fPitch );
	pVector->z = fLen2d * sinf( m_fYaw );
}

///////////////////////////////////////////////////////////////////////////////
inline void CCamera::SetPitchLimit( float fMin, float fMax )
{
	fMax = max( fMin, fMax );
	m_fPitchMin = max( fMin, -D3DX_PI / 2 );
	m_fPitchMax = min( fMax, D3DX_PI / 2 );
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetYaw() const
{
	return m_fYaw;
}

///////////////////////////////////////////////////////////////////////////////
inline float CCamera::GetPitch() const
{
	return m_fPitch;
}

///////////////////////////////////////////////////////////////////////////////
inline void CCamera::ChangeYaw(float fRadian)
{
	SetYaw(m_fYaw + fRadian);
}

///////////////////////////////////////////////////////////////////////////////
inline void CCamera::ChangePitch(float fRadian)
{
	SetPitch(m_fPitch + fRadian);
}

inline float CCamera::GetRoll() const
{
	return m_fRoll;
}
inline void  CCamera::ChangeRoll(float fRadian)
{
	SetRoll(m_fRoll + fRadian);

}

extern CCamera g_Camera;

#endif