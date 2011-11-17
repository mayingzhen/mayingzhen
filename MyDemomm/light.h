#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <vector>

#include "bounding.h"

class CObject;

class Light
{
public:
	Light();

	// finds scene objects that overlap given frustum from light's view
	std::vector<CObject *> FindCasters(const CFrustum &frustum);

	// processes light controls
	void DoControls(void);

	// calculates light matrix to contain given visible objects
	void CalculateMatrices(void);

	// crops the light volume on given frustum (scene-independent projection)
	D3DXMATRIX CalculateCropMatrix(const CFrustum &frustum);

	// crops the light volume on given objects, constrained by given frustum
	D3DXMATRIX CalculateCropMatrix(const std::vector<CObject *> &casters, 
		const std::vector<CObject *> &receivers, const CFrustum &frustum);

	// retuns direction of light
	D3DXVECTOR3 GetDir(void);

private:

	// build a matrix for cropping light's projection
	inline D3DXMATRIX BuildCropMatrix(const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax);

public:

	enum LightType
	{
		TYPE_ORTHOGRAPHIC, // eg. directional light
		TYPE_PERSPECTIVE   // eg. spot light
	};

	// type of light
	LightType m_Type;

	// matrices, updated with CalculateMatrices()
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

	// settings
	D3DXVECTOR3 m_vSource;
	D3DXVECTOR3 m_vTarget;
	float m_fNear;
	float m_fFar;
	float m_fFOV;
	D3DXVECTOR3 m_vUpVector;
	float m_fAspectRatio;
	D3DXVECTOR3 m_vLightDiffuse;
	D3DXVECTOR3 m_vLightAmbient;

public:

	struct ControlState
	{
		D3DXVECTOR3 m_vRotation;
		bool m_bSwitchingType;
		double m_fLastUpdate;
	};

	ControlState m_ControlState;
};







#endif
