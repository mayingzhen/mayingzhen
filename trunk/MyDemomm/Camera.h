#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once

#include <d3dx9.h>
#include <assert.h>

#include "bounding.h"


class CCamera
{
public:
	CCamera();

	inline const D3DXMATRIX&  GetViewMatrix()	const;
	inline const D3DXMATRIX&  GetProjMatrix()	const;

	void SetViewParams( const D3DXVECTOR3 &vEyePt, const D3DXVECTOR3 &vLookAtPt );
	CFrustum CalculateFrustum(float fNear, float fFar);
	void Update();

	void DoKeyControl();
	void DoMouseControl();

private:
	void CalculateSplitPositions(float *pDistances,int nNumSplits);
	void CalculateMatrices();
	void UpdateViewMatrix();

public:
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

	enum {NUM_PSSM = 4};
	float m_fSplitPos[NUM_PSSM + 1];

	CFrustum m_frustum;

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


extern CCamera g_Camera;

#endif