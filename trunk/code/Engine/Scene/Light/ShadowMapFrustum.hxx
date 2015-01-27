#include "ShadowMapFrustum.h"
#include "ShadowCasterQuery.h"

namespace ma
{
	ShadowMapFrustum::ShadowMapFrustum()
	{
		m_fConstantBias[0] = 0;
		m_fSlopeScaleBias[0] = 0;
		m_fConstantBias[1] = 0;
		m_fSlopeScaleBias[1] = 0;

		m_bDraw[0] = true;
		m_bDraw[1] = true;

		m_viewport = Rectangle::ZERO;

		m_matLightView = Matrix4::IDENTITY;
		m_matLightProj = Matrix4::IDENTITY;
		m_matCrop = Matrix4::IDENTITY;
		m_matVPShadow[0]  = Matrix4::IDENTITY;
		m_matVPShadow[1]  = Matrix4::IDENTITY;
		m_matTexAdjust = Matrix4::IDENTITY;
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{

	}

	void ShadowMapFrustum::CreateViewPort(Texture* pShadowMap,Rectangle veiewPort)
	{
		veiewPort.x += 1.0f;
		veiewPort.y += 1.0f;
		veiewPort.width -= 2.0f;
		veiewPort.height -= 2.0f;

		m_viewport = veiewPort;

		m_matTexAdjust = CalculateTexAdjustMatrix(pShadowMap,m_viewport);
	}

	Matrix4 ShadowMapFrustum::CalculateTexAdjustMatrix(Texture* pShadowMap,Rectangle veiewPort)
	{
		Matrix4 texAdjust(Matrix4::IDENTITY);

		float width = (float)pShadowMap->GetWidth();
		float height = (float)pShadowMap->GetWidth();

		Vector2 offset( (float)veiewPort.left() / width, (float)veiewPort.top() / height);

		Vector2 scale( 0.5f * (float)veiewPort.width / width, 0.5f * (float)veiewPort.height / height );

		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2)
		{
			offset.x += scale.x;
			offset.y += scale.y;
			offset.y = 1.0f - offset.y;

			texAdjust.setTrans(Vector3(offset.x, offset.y, 0.5f));
			texAdjust.setScale(Vector3(scale.x, scale.y, 0.5f));
		}
		else
		{
			offset.x += scale.x + 0.5f / width;
			offset.y += scale.y + 0.5f / height;

			scale.y = -scale.y;
			texAdjust.setTrans(Vector3(offset.x, offset.y, 0.0f));
			texAdjust.setScale(Vector3(scale.x, scale.y, 1.0f));
		}

		return texAdjust;
	}

	void ShadowMapFrustum::UpdateBatch(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		Caster_Cull eCaterCullType = GetRenderShadowCSM()->GetCasterCullType();
		if (eCaterCullType != CasterCull_No)
		{
			Frustum ligtFrustum;
			ligtFrustum.Update(m_matLightProj * m_matLightView, GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2);

			Frustum lightViewFrustum = m_frustum;
			lightViewFrustum.Transformed(m_matLightView);

			ShadowCasterQuery shaowQuery(ligtFrustum,lightViewFrustum,m_matLightView,m_arrCaster,m_casterAABB);
			pCamera->GetSceneNode()->GetScene()->GetCullTree()->FindObjectsIn(shaowQuery);
		}

		for (VEC_CASTER::iterator iter = m_arrCaster.begin();iter != m_arrCaster.end();++iter)
		{
			RenderComponent* pRenderComp = (*iter);

			float fShadowFarDis = GetRenderShadowCSM()->GetShadowFarDistance();
			if (fShadowFarDis > 0)
			{
				float fLodValue = (pRenderComp->GetSceneNode()->GetPosWS() - pCamera->GetSceneNode()->GetPosWS()).length();
				if (fLodValue > fShadowFarDis)
					continue;
			}
		
			for (UINT i = 0; i < pRenderComp->GetRenderableCount(); ++i)
			{
				m_arrRenderable[GetRenderSystem()->CurThreadFill()].push_back(pRenderComp->GetRenderableByIndex(i));
			}
		}
	}

	void ShadowMapFrustum::UpdateFrustum(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		Matrix4 matView = pCamera->GetMatView();
		Matrix4 matProj;
		GetRenderDevice()->MakePerspectiveMatrix(matProj,pCamera->GetFov(),pCamera->GetAspect(),fSpiltNear,fSpiltFar);

		m_frustum.Update(matProj * matView,GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2);
	}


	void ShadowMapFrustum::UpdateLightMatrix(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{		
		Scene* pCurScene = pCamera->GetSceneNode()->GetScene();
		
		ColourValue color;
		Vector3 vLightDir;
		pCurScene->GetDirectionalLight(color, vLightDir);

		Vector3 vLightUp;
		if ( Math::Abs( (-vLightDir).dotProduct( pCamera->GetSceneNode()->GetUp() ) ) > 0.95f )
		{
			vLightUp = pCamera->GetSceneNode()->GetRight();
		}
		else
		{
			vLightUp = pCamera->GetSceneNode()->GetUp();
		}

		Vector3 pos = pCamera->GetSceneNode()->GetPosWS() - pCamera->GetFarClip() * vLightDir;

		Matrix4 matLightView = Math::MakeLookAtMatrixRH(pos,pos + vLightDir,vLightUp);
		Matrix4 matInvLightView = matLightView.inverse();

		AABB aabbInLightView;

		Caster_Cull eCaterCullType = GetRenderShadowCSM()->GetCasterCullType();
		if (eCaterCullType == CasterCull_No)
		{
			uint32 iNodeNum = pCurScene->GetVisibleNodeNum();
			for (uint32 i = 0; i < iNodeNum; ++i)
			{
				RenderComponent* pNode = pCurScene->GetVisibleNodeByIndex(i);
				if (pNode->GetViewMinZ() > fSpiltFar || pNode->GetViewMaxZ() < fSpiltNear)
					continue;

				m_sceneAABB.merge( pNode->GetAABBWS() );

				if (!pNode->GetShadowCaster())
					continue;

				m_casterAABB.merge( pNode->GetAABBWS() );

				m_arrCaster.push_back(pNode);
			}	

			if (m_arrCaster.empty())
			{
				m_bDraw[GetRenderSystem()->CurThreadFill()] = false;
			}
			else
			{
				m_bDraw[GetRenderSystem()->CurThreadFill()] = true;	

				aabbInLightView = m_casterAABB;
				aabbInLightView.transform(matLightView);
			}
		}
		else
		{
			uint32 iNodeNum = pCurScene->GetVisibleNodeNum();
			for (uint32 i = 0; i < iNodeNum; ++i)
			{
				RenderComponent* pNode = pCurScene->GetVisibleNodeByIndex(i);

				if (pNode->GetViewMinZ() > fSpiltFar || pNode->GetViewMaxZ() < fSpiltNear)
					continue;

				m_sceneAABB.merge( pNode->GetAABBWS() );
			}

			ConvexBody frustumVolume;
			frustumVolume.define(m_frustum);
			frustumVolume.clip(m_sceneAABB);
			if (frustumVolume.getPolygonCount() <= 0)
			{
				m_bDraw[GetRenderSystem()->CurThreadFill()] = false;
			}
			else
			{		
				m_bDraw[GetRenderSystem()->CurThreadFill()] = true;

				frustumVolume.transformed(matLightView);
				aabbInLightView = frustumVolume.getAABB();
			}
		}

		if (aabbInLightView.isNull())
		{
			Frustum frustumInLightView = m_frustum;
			frustumInLightView.Transformed(matLightView);
			aabbInLightView = frustumInLightView.GetAABB();
		}

		Vector3 vLightPos;
		Vector3 vCenter = aabbInLightView.getCenter();
		Vector3 vLigtViewSize = aabbInLightView.getSize();
		if (eCaterCullType == LightFrustum_Cull)
		{
			vLightPos = matInvLightView * Vector3(vCenter.x,vCenter.y,0);
			vLigtViewSize.z = -aabbInLightView.getMinimum().z;
		}
		else 
		{
			vLightPos = matInvLightView * Vector3(vCenter.x,vCenter.y,aabbInLightView.getMaximum().z);
		}

		m_matLightView = Math::MakeLookAtMatrixRH(vLightPos, vLightPos + vLightDir, vLightUp);

		QuantizeViewSize(vLigtViewSize.x,vLigtViewSize.y);

		vLigtViewSize.x = max(vLigtViewSize.x,1.0f);
		vLigtViewSize.y = max(vLigtViewSize.y,1.0f);
		vLigtViewSize.z = max(vLigtViewSize.z,1.0f);

		GetRenderDevice()->MakeOrthoMatrix(m_matLightProj,vLigtViewSize.x,vLigtViewSize.y,0.0f,vLigtViewSize.z); 
	}

	void ShadowMapFrustum::QuantizeViewSize(float& fWidth,float& fHight)
	{
		float fQuantize = 0.5f;
		fWidth = ceilf(sqrtf(fWidth / fQuantize));
		fHight = ceilf(sqrtf(fHight / fQuantize));
		fWidth = fWidth * fWidth * fQuantize;
		fHight = fHight * fHight * fQuantize;

	}

	void ShadowMapFrustum::UpdateCropMats()
	{
		if (!m_bDraw)
			return;

		Matrix4 matVP = m_matLightProj* m_matLightView;

		Frustum vpFurstum = m_frustum;
		vpFurstum.Transformed(matVP);
		AABB splitBB = vpFurstum.GetAABB();
		//AxisAlignedBox splitBB = m_frustum.GetAABB();
		//splitBB.transform(matVP);

		AABB castersBB = m_casterAABB;
		castersBB.transform(matVP);

		AABB sceeneBB = m_sceneAABB;
		sceeneBB.transform(matVP);

		//AxisAlignedBox cropBB;
		Vector3 vMax,vMin;
		vMin.x = max(castersBB.getMinimum().x, splitBB.getMinimum().x);
		vMax.x = min(castersBB.getMaximum().x, splitBB.getMaximum().x);
		vMin.y = max(castersBB.getMinimum().y, splitBB.getMinimum().y);
		vMax.y = min(castersBB.getMaximum().y, splitBB.getMaximum().y);
		vMin.z = min(sceeneBB.getMinimum().z, castersBB.getMinimum().z);
		vMax.z = max(sceeneBB.getMaximum().z, splitBB.getMaximum().z);

		float fScaleX = 2.0f / (vMax.x - vMin.x);
		float fScaleY = 2.0f / (vMax.y - vMin.y);

		float fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
		float fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

		float fScaleZ = 1.0f / (vMax.z - vMin.z);
		float fOffsetZ = -vMin.z * fScaleZ;

		m_matCrop = Matrix4::IDENTITY;
		m_matCrop.setScale(Vector3(fScaleX,fScaleY,fScaleZ));
		m_matCrop.setTrans(Vector3(fOffsetX,fOffsetY,fOffsetZ));
	}

	void ShadowMapFrustum::UpdateDepthBias(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		float fConstantBias = 0;
		float fSlopeScaleBias = 0;
		GetRenderShadowCSM()->GetDepthBiasParams(fConstantBias,fSlopeScaleBias);

		float vCurSplitx = GetRenderShadowCSM()->GetCurSplitPos().x;
		float multiplier = max(fSpiltFar / vCurSplitx, 1.0f);	

		m_fConstantBias[GetRenderSystem()->CurThreadFill()] = fConstantBias * multiplier;
		m_fSlopeScaleBias[GetRenderSystem()->CurThreadFill()] = fConstantBias * multiplier;
	}

	void ShadowMapFrustum::Update(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		Clear(pCamera);

		UpdateDepthBias(pCamera,fSpiltNear,fSpiltFar);

		UpdateFrustum(pCamera,fSpiltNear,fSpiltFar);

		UpdateLightMatrix(pCamera,fSpiltNear,fSpiltFar);

		UpdateBatch(pCamera);

		UpdateCropMats();

		m_matVPShadow[GetRenderSystem()->CurThreadFill()] = m_matCrop * m_matLightProj * m_matLightView;
	}

	void ShadowMapFrustum::Render(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		GetRenderSystem()->SetViewPort(m_viewport);

		if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
		{
			GetRenderSystem()->SetDepthBias(m_fConstantBias[GetRenderSystem()->CurThreadProcess()]);
		}

		for (UINT i = 0; i <  m_arrRenderable[GetRenderSystem()->CurThreadProcess()].size(); ++i)
		{
			Renderable* pRenderObj = m_arrRenderable[GetRenderSystem()->CurThreadProcess()][i];
			if (pRenderObj == NULL)
				continue;
			 
			 SubMaterial* pMaterial = pRenderObj->GetMaterial();
		
			 Technique* pTech = pMaterial->GetShadowDepthTechnqiue();

	 
			 //pRenderObj->Render(pTech);
			 GetRenderContext()->SetCurRenderObj(pRenderObj);

			 pTech->Bind();

			 Uniform* pUniform = pTech->GetShaderProgram()->GetUniform("matLightViewProj");
			 GetRenderSystem()->SetValue(pUniform,m_matVPShadow[GetRenderSystem()->CurThreadProcess()]);

			 GetRenderSystem()->DrawRenderable(pRenderObj,pTech);
		}
	}

	void ShadowMapFrustum::Clear(Camera* pCamera)
	{
		m_arrRenderable[GetRenderSystem()->CurThreadFill()].clear();
		m_casterAABB.setNull();
		m_arrCaster.clear();	
		m_sceneAABB.setNull();
	}
}

