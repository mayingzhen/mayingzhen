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
	float		m_fFOV;			//视角
	float		m_fAspect;		//视口宽高比例
	float		m_fNearClip;	//近剪裁面距离
	float		m_fFarClip;		//远剪裁面距离

	float		m_fNearMin;
	float		m_fFarMax;

	D3DXVECTOR3	m_vEyePt;		//摄像机位置
	D3DXVECTOR3	m_vLookAtPt;	//视点中心位置
	D3DXVECTOR3	m_vUpVector;

	float		m_fYaw;			//方向角
	float		m_fPitch;		//俯仰角
	float		m_fPitchMin;	//最小俯仰
	float		m_fPitchMax;	//最大俯仰
	float		m_fRoll;//旋转角度

	D3DXMATRIX	m_mView;		//视矩阵
	D3DXMATRIX	m_mProj;		//投影矩阵

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