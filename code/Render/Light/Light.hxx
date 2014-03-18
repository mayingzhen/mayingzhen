#include "Light.h"
#include "LightSystem.h"

namespace ma
{
	IMPL_OBJECT(Light,Component)

	Light::Light(GameObject* pGameObj)
		:Component(pGameObj)
	{
		m_bCreateShadow = false;
		m_cLightColor = Vector4(1,1,1,1);
		m_fLightIntensity = 1.0f;
	
		GetLightSystem()->AddLight(this);
	}

	Light::~Light()
	{
		GetLightSystem()->RemoveLight(this);
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

	Vector3 DirectonalLight::GetDirection()
	{
// 		Vector3 vSource(-10000,10000,-10000);
// 		Vector3 vTarget(0,1,0);
// 		Vector3 vDir = vSource - vTarget;
// 		Vec3Normalize(vDir);
// 		return  vDir;
		return m_pGameObject->GetSceneNode()->GetWorldMatrix().GetRow(2);
	}

	void DirectonalLight::SetCreateShadow(bool bCreateShaow)
	{
		if ( !GetRenderSetting()->m_bShadow )
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
		int index = GetRenderSystem()->GetThreadList();

		return m_fSplitPos[index];
	}


	class CastersBuilder : public SceneVisiter
	{

	public:
		CastersBuilder(ShadowMapFrustum* pSMF,const Frustum& splitFrustum,const Vector3& vLigtDir)
			:m_splitFrustum(splitFrustum),m_vLightDir(vLigtDir)
		{
			m_pSMF = pSMF;
			m_pSMF->ClearCasterList();
		}

		virtual bool VisiteComponent(Component* pComp)
		{
// 			RenderComponent* pRenderObj = SafeCast<RenderComponent>(pComp);
// 			if (pRenderObj)
// 			{
// 				AABB aabb = pRenderObj->GetGameObject()->GetAABBWS();
// 
// 				if ( SweepIntersectionTest(aabb,m_splitFrustum,m_vLightDir) )
// 				{
// 					m_pSMF->AddCaster(pRenderObj);
// 					m_aabb.Merge(aabb);
// 				}
// 			}

			return true;
		}

		const AABB&		GetCastersAABB() {return m_aabb;}

	private: 
		const Vector3		m_vLightDir;

		const Frustum&		m_splitFrustum;

		ShadowMapFrustum*	m_pSMF;	

		AABB				m_aabb;
		
	};


	void DirectonalLight::UpdateShadowFrustum(Camera* pCamera)
	{
		if (pCamera == NULL)
			return;

		int index = GetRenderSystem()->GetThreadList();

		Matrix4x4 matLightView;

		MatrixInverse(&matLightView,NULL,&m_pSceneNode->GetWorldMatrix());

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

			Matrix4x4 matView = pCamera->GetMatView();
			Frustum splitFrustum;
			splitFrustum.Update(matView * matSplitProj);

			CastersBuilder casterBuild(pSMF,splitFrustum,GetDirection());
			GetEntitySystem()->GetRootGameObject()->TravelScene(&casterBuild);

			AABB aabbCasters = casterBuild.GetCastersAABB();
			aabbCasters.Transform(matLightView);

			AABB aabbReceivers = GetTerrain()->GetWorldAABB();
			aabbReceivers.Transform(matLightView);

			AABB aabbSplit;
			aabbSplit.Merge(splitFrustum.m_pPoints,8);
			aabbSplit.Transform(matLightView);

			AABB cropAABB;

			cropAABB.m_vMin.x = Max(Max(aabbCasters.m_vMin.x, aabbReceivers.m_vMin.x), aabbSplit.m_vMin.x);
			cropAABB.m_vMax.x = Min(Min(aabbCasters.m_vMax.x, aabbReceivers.m_vMax.x), aabbSplit.m_vMax.x);
			cropAABB.m_vMin.y = Max(Max(aabbCasters.m_vMin.y, aabbReceivers.m_vMin.y), aabbSplit.m_vMin.y);
			cropAABB.m_vMax.y = Min(Min(aabbCasters.m_vMax.y, aabbReceivers.m_vMax.y), aabbSplit.m_vMax.y);
			cropAABB.m_vMin.z = aabbCasters.m_vMin.z;
			cropAABB.m_vMax.z = Min(aabbReceivers.m_vMax.z, aabbSplit.m_vMax.z);

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



