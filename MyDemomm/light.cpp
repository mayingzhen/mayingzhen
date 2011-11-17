#include "public.h"
#include "light.h"
#include "CObject.h"

Light::Light()
{
	m_Type = TYPE_ORTHOGRAPHIC;
	m_vUpVector = D3DXVECTOR3(0, 1, 0);
	m_fAspectRatio = 1.0f;
	m_vSource = D3DXVECTOR3(-10000, 5000, -10000);
	m_vTarget = D3DXVECTOR3(0, 0, 0);
	m_fNear = 1.0f;
	m_fFar = 30000.0f;
	m_fFOV = DegreeToRadian(90.0f);
	m_vLightDiffuse = D3DXVECTOR3(0.7f,0.7f,0.7f);
	m_vLightAmbient = D3DXVECTOR3(0.25f,0.25f,0.25f);

	ZeroMemory(&m_ControlState, sizeof(ControlState));
	m_ControlState.m_vRotation = D3DXVECTOR3(-DegreeToRadian(130.0f), -DegreeToRadian(35.0f),0);
}

// processes light controls
void Light::DoControls(void)
{
	float fDeltaTime = DeltaTimeUpdate(m_ControlState.m_fLastUpdate);

	// Rotate light
	//
	if(GetKeyDown(VK_LEFT)) m_ControlState.m_vRotation.x += 0.02f * fDeltaTime;
	else if(GetKeyDown(VK_RIGHT)) m_ControlState.m_vRotation.x -= 0.02f * fDeltaTime;
	if(GetKeyDown(VK_UP)) m_ControlState.m_vRotation.y += 0.01f * fDeltaTime;
	else if(GetKeyDown(VK_DOWN)) m_ControlState.m_vRotation.y -= 0.01f * fDeltaTime;

	m_ControlState.m_vRotation.y = Clamp(m_ControlState.m_vRotation.y, DegreeToRadian(-89.9f), DegreeToRadian(0.0f));
	float ch = cosf(m_ControlState.m_vRotation.x);
	float sh = sinf(m_ControlState.m_vRotation.x);
	float cp = cosf(m_ControlState.m_vRotation.y);
	float sp = sinf(m_ControlState.m_vRotation.y);
	D3DXVECTOR3 vDist = m_vTarget - m_vSource;
	float leng = D3DXVec3Length(&vDist);
	m_vSource = m_vTarget + D3DXVECTOR3(sh*cp, -sp, cp*ch) * leng;

	// Switch light type
	//
	if(GetKeyDown('T'))
	{
		if(!m_ControlState.m_bSwitchingType)
		{
			m_Type = (m_Type == Light::TYPE_ORTHOGRAPHIC) ? Light::TYPE_PERSPECTIVE : Light::TYPE_ORTHOGRAPHIC;
			m_ControlState.m_bSwitchingType = true;
		}
	}
	else
	{
		m_ControlState.m_bSwitchingType = false;
	}

	CalculateMatrices();
}

// calculates default light matrices
void Light::CalculateMatrices(void)
{
	// view matrix
	D3DXMatrixLookAtLH(&m_mView, &m_vSource, &m_vTarget, &m_vUpVector);
	//m_mView = MatrixLookAtLH(m_vSource, m_vTarget, m_vUpVector);

	// projection matrix
	if(m_Type == TYPE_PERSPECTIVE)
	{
		//m_mProj = MatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, m_fNear, m_fFar);
		D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspectRatio, m_fNear, m_fFar);
	}
	else
	{
		// this is just a funny way to calculate a size for the light using FOV
		float fFarPlaneSize = 2 * tanf(m_fFOV * 0.5f) * m_fFar;
		//m_mProj = MatrixOrthoLH(fFarPlaneSize * m_fAspectRatio, fFarPlaneSize, m_fNear, m_fFar);
		D3DXMatrixOrthoLH(&m_mProj, fFarPlaneSize * m_fAspectRatio, fFarPlaneSize, m_fNear, m_fFar);
	}
}

// finds scene objects that overlap given frustum from light's view
std::vector<CObject *> Light::FindCasters(const CFrustum &frustum)
{
	std::vector<CObject *> casters;
	//casters.reserve(g_SceneObjects.size());
	for(unsigned int i = 0; i < CObject::OBJ_ARRAY_NUM/*g_SceneObjects.size()*/; i++)
	{
		CObject *pObject = CObject::GObjHash[i];//g_SceneObjects[i];
		if(pObject->m_ObjFlag & FOB_NOT_CAST_SHADOW) 
			continue;
	
		if ( pObject->m_WorldAABB.Intersect(frustum.m_AABB) )
			continue;
	
		casters.push_back(pObject);
	}
	return casters;
}


// build a matrix for cropping light's projection
// given vectors are in light's clip space
inline D3DXMATRIX Light::BuildCropMatrix(const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax)
{
	float fScaleX, fScaleY, fScaleZ;
	float fOffsetX, fOffsetY, fOffsetZ;

	fScaleX = 2.0f / (vMax.x - vMin.x);
	fScaleY = 2.0f / (vMax.y - vMin.y);

	fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
	fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

	fScaleZ = 1.0f / (vMax.z - vMin.z);
	fOffsetZ = -vMin.z * fScaleZ;

	// crop volume matrix
	return D3DXMATRIX(   fScaleX,     0.0f,     0.0f,   0.0f,
						    0.0f,  fScaleY,     0.0f,   0.0f,
							0.0f,     0.0f,  fScaleZ,   0.0f,
						fOffsetX, fOffsetY, fOffsetZ,   1.0f  );
}

// inline D3DXVECTOR4 Transform(const D3DXVECTOR4 &A, const D3DXMATRIX &M)
// {
// 	return D3DXVECTOR4((A.x * M._11) + (A.y * M._21) + (A.z * M._31) + (A.w * M._41),
// 		(A.x * M._12) + (A.y * M._22) + (A.z * M._32) + (A.w * M._42),
// 		(A.x * M._13) + (A.y * M._23) + (A.z * M._33) + (A.w * M._43),
// 		(A.x * M._14) + (A.y * M._24) + (A.z * M._34) + (A.w * M._44));
// }
// 
// // transforms vector with matrix
// inline D3DXVECTOR4 Transform(const D3DXVECTOR3 &A, const D3DXMATRIX &M)
// {
// 	return Transform(D3DXVECTOR4(A.x,A.y,A.z,1), M);
// }

//helper function for computing AABB in clip space
// inline CAABB CreateClipSpaceAABB(const CAABB &bb, const D3DXMATRIX &mViewProj)
// {
// 	D3DXVECTOR4 vTransformed[8];
// 	// for each point
// 	for(int i=0;i<8;i++)
// 	{
// 		// transform to projection space
// 		vTransformed[i] = Transform(bb.m_pPoints[i], mViewProj);
// 
// 		// compute clip-space coordinates
// 		vTransformed[i].x /= vTransformed[i].w;
// 		vTransformed[i].y /= vTransformed[i].w;
// 		vTransformed[i].z /= vTransformed[i].w;
// 	}
// 
// 	return CAABB(vTransformed, 8);
// }

// crops the light volume on given frustum (scene-independent projection)
D3DXMATRIX Light::CalculateCropMatrix(const CFrustum &frustum)
{
	D3DXMATRIX mViewProj = m_mView * m_mProj;

	CAABB cropBB;

	// find boundaries in light clip space
	//cropBB = CreateClipSpaceAABB(frustum.m_AABB, mViewProj);
	frustum.m_AABB.Transform(cropBB, mViewProj);

	// use default near plane
	cropBB.vMin.z = 0.0f;

	// finally, create matrix
	return BuildCropMatrix(cropBB.vMin, cropBB.vMax);
}

// crops the light volume on given objects, constrained by given frustum
D3DXMATRIX Light::CalculateCropMatrix(const std::vector<CObject *> &casters, 
									  const std::vector<CObject *> &receivers, const CFrustum &frustum)
{
// 	//if(!g_bUseSceneDependentProjection) return CalculateCropMatrix(frustum);
// 
// 	D3DXMATRIX mViewProj = m_mView * m_mProj;
// 
// 	// bounding box limits
// 	CAABB receiversBB, splitBB, castersBB;
// 
// 	// for each caster
// 	// find boundaries in light’s clip space
// 	for(unsigned int i = 0; i < casters.size(); i++)
// 		castersBB.Union(CreateClipSpaceAABB(casters[i]->m_AABB, mViewProj));
// 
// 	// for each receiver
// 	// find boundaries in light’s clip space
// 	for(unsigned int i = 0; i < receivers.size(); i++)
// 	{
// 		receiversBB.Union(CreateClipSpaceAABB(receivers[i]->m_AABB, mViewProj));
// 	}
// 
// 	// find frustum boundaries in light’s clip space
// 	splitBB = CreateClipSpaceAABB(frustum.m_AABB, mViewProj);
// 
// 	// next we will merge the bounding boxes
// 	//
// 	BoundingBox cropBB;
// 	cropBB.m_vMin.x = Max(Max(castersBB.m_vMin.x, receiversBB.m_vMin.x), splitBB.m_vMin.x);
// 	cropBB.m_vMax.x = Min(Min(castersBB.m_vMax.x, receiversBB.m_vMax.x), splitBB.m_vMax.x);
// 	cropBB.m_vMin.y = Max(Max(castersBB.m_vMin.y, receiversBB.m_vMin.y), splitBB.m_vMin.y);
// 	cropBB.m_vMax.y = Min(Min(castersBB.m_vMax.y, receiversBB.m_vMax.y), splitBB.m_vMax.y);
// 	cropBB.m_vMin.z = castersBB.m_vMin.z;
// 	cropBB.m_vMax.z = Min(receiversBB.m_vMax.z, splitBB.m_vMax.z);
// 
// 	// when there are no casters, the merged
// 	// bounding box will be infinitely small
// 	if(casters.size() == 0)
// 	{
// 		// it will cause artifacts when rendering receivers,
// 		// so just use the frustum bounding box instead
// 		cropBB.m_vMin = splitBB.m_vMin;
// 		cropBB.m_vMax = splitBB.m_vMax;
// 	}
// 
// 	// finally, create matrix
// 	return BuildCropMatrix(cropBB.m_vMin, cropBB.m_vMax);
	D3DXMATRIX mat;
	return mat;
}


// returns direction of light
D3DXVECTOR3 Light::GetDir(void)
{
	D3DXVECTOR3 temp;
	D3DXVec3Normalize( &temp, &(m_vTarget - m_vSource) );
	return temp;
}
