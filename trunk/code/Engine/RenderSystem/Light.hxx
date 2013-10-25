#include "Light.h"

namespace ma
{
	IMPL_OBJECT(Light,Component)

	Light::Light(GameObject* pGameObj)
		:Component(pGameObj)
	{
		m_bCreateShadow = false;
		m_cLightColor = Vector4(1,1,1,1);
	
		GetRenderSystem()->AddLight(this);
	}

	IMPL_OBJECT(PointLight,Light)
	
	Vector3 PointLight::GetPos()
	{
		return m_pGameObject->GetSceneNode()->GetTransform().m_vPos;
	}

	IMPL_OBJECT(SpotLight,Light)

	void SpotLight::LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp)
	{
		m_vTarget = vAt;
		m_vUpVector = vUp;

		// view matrix
		MatrixLookAtLH(&m_mView, &vEye, &vAt, &vUp);

		// MatWorld
		Matrix4x4 matInv;
		MatrixInverse(&matInv,NULL,&m_mView);
		TransformFromMatrix(&m_tsfWS,&matInv);
		
		// projection matrix
		GetRenderDevice()->MakePerspectiveMatrix(&m_mProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	}

	
	IMPL_OBJECT(DirectonalLight,Light)

	DirectonalLight::DirectonalLight(GameObject* pGameObj)
		:Light(pGameObj) 
	{
		m_eLightType = LIGHT_DIRECTIONAL;
		m_fShadowOffset = 5000.0f;
	}

	void DirectonalLight::UpdateTransform()
	{
	}

	Vector3 DirectonalLight::GetDirection()
	{
		return m_pGameObject->GetSceneNode()->GetWorldMatrix().GetRow(2);
	}

	void DirectonalLight::SetCreateShadow(bool bCreateShaow)
	{
		if ( !GetRenderSystem()->IsShadow() )
			return;

		m_bCreateShadow = bCreateShaow;

		if (m_bCreateShadow && m_arrShadowFrustum.empty())
		{
			for (UINT i = 0; i < NUM_PSSM; ++i)
			{
				ShadowMapFrustum* pSMF = new ShadowMapFrustum(this,i);
				m_arrShadowFrustum.push_back(pSMF);
			}
		}
	}

	float* DirectonalLight::GetSplitPos() 
	{
		int index = GetRenderThread()->GetThreadList();

		return m_fSplitPos[index];
	}

	void DirectonalLight::UpdateShadowFrustum(Camera* pCamera)
	{
		if (pCamera == NULL)
			return;

		int index = GetRenderThread()->GetThreadList();

		Matrix4x4 matLightView;

// 		Vector3 vDir = GetDirection();
// 		Vector3 vEye = -vDir * m_fShadowOffset;
// 		Vector3 vAt = Vector3(0,0,0);
// 		Vector3 vUp = Vector3(0,1,0);
// 		MatrixLookAtLH(&matLightView,&vEye,&vAt,&vUp);
		MatrixInverse(&matLightView,NULL,&GetGameObject()->GetSceneNode()->GetWorldMatrix());

		//CalculateSplitPositions
		float fNear = pCamera->GetNearClip();
		float fFar = pCamera->GetFarClip();
		m_fSplitPos[index][NUM_PSSM + 1];
		float fSplitSchemeWeight = 0.5;
		for(int i = 0; i < NUM_PSSM; i++)
		{
			float fIDM = i / (float)NUM_PSSM;
			float fLog = fNear  * powf(fFar/fNear, fIDM);
			float fUniform = fNear + (fFar - fNear) * fIDM;
			m_fSplitPos[index][i] = fLog * fSplitSchemeWeight + fUniform * (1 - fSplitSchemeWeight);
		}
		m_fSplitPos[index][0] = fNear;
		m_fSplitPos[index][NUM_PSSM] = fFar;
		
		//  CaclulateFrustum
		for (UINT i = 0; i < NUM_PSSM; ++i)
		{
			ShadowMapFrustum* pSMF = m_arrShadowFrustum[i];
		
			Matrix4x4 matSplitProj;
			GetRenderDevice()->MakePerspectiveMatrix(&matSplitProj,pCamera->GetFov(),
				pCamera->GetAspect(),m_fSplitPos[index][i],m_fSplitPos[index][i + 1]);

			Frustum splitFrustum(pCamera->GetViewMatrix() * matSplitProj);
			AABB cropAABB;
			cropAABB.Merge(splitFrustum.vPts,8);
			cropAABB.Transform(matLightView); 
			cropAABB.m_vMin.z = 0.0f; // use default near plane

			Matrix4x4 matCrop = CalculateCropMatrix(cropAABB);

			pSMF->SetViewMatrix(matLightView);

			pSMF->SetProjMatrix(matCrop);
		}
	}

	Matrix4x4 DirectonalLight::CalculateCropMatrix(const AABB& cropAABB)
	{
		// finally, create matrix
		//return BuildCropMatrix(cropBB.vMin, cropBB.vMax);
		Vector3 vMax = cropAABB.m_vMax;
		Vector3 vMin = cropAABB.m_vMin;

		float fScaleX, fScaleY, fScaleZ;
		float fOffsetX, fOffsetY, fOffsetZ;

		fScaleX = 2.0f / (vMax.x - vMin.x);
		fScaleY = 2.0f / (vMax.y - vMin.y);

		fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
		fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

		fScaleZ = 1.0f / (vMax.z - vMin.z);
		fOffsetZ = -vMin.z * fScaleZ;

		// crop volume matrix
		Matrix4x4 matCrop = Matrix4x4(   fScaleX,     0.0f,     0.0f,   0.0f,
			0.0f,  fScaleY,     0.0f,   0.0f,
			0.0f,     0.0f,  fScaleZ,   0.0f,
			fOffsetX, fOffsetY, fOffsetZ,   1.0f  );

		return matCrop;
	}

}



