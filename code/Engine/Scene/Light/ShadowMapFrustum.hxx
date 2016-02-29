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

		m_viewport = Rectangle(0,0,0,0);

		m_matLightView = Matrix4::IDENTITY;
		m_matLightProj = Matrix4::IDENTITY;
		m_matLightViewProj[0] = Matrix4::IDENTITY;
		m_matLightViewProj[1] = Matrix4::IDENTITY;
		m_matCrop = Matrix4::IDENTITY;
		m_matShadow[0]  = Matrix4::IDENTITY;
		m_matShadow[1]  = Matrix4::IDENTITY;
		m_matTexAdjust = Matrix4::IDENTITY;
	
		m_fShadowFarDist = 0;

		m_eCaterType = CasterCull_No;
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{

	}

	void ShadowMapFrustum::CreateShadowMap(int nSize)
	{
		PixelFormat shadowMapColorFormat = GetDeviceCapabilities()->GetShadowMapColorFormat();
		PixelFormat shadowMapDepthFormat = GetDeviceCapabilities()->GetShadowMapDepthFormat();
		m_pShadowMapColor = GetRenderSystem()->CreateRenderTexture(nSize,nSize,shadowMapColorFormat,USAGE_RENDERTARGET);	

		if (shadowMapDepthFormat != PF_UNKNOWN)
		{
			m_pShdowMapDepth = GetRenderSystem()->CreateRenderTexture(nSize,nSize,shadowMapDepthFormat,USAGE_DEPTHSTENCIL);
		}
		else
		{
			m_pShdowMapDepth = GetRenderSystem()->CreateDepthStencil(nSize,nSize,PF_D24S8);
		}

		m_viewport = Rectangle(1.0f, 1.0f, (float)nSize - 2.0f, (float)nSize - 2.0f);

		m_matTexAdjust = CalculateTexAdjustMatrix(m_pShadowMapColor.get(),m_viewport);
	}

	Texture* ShadowMapFrustum::GetShadowMap() const 
	{
		if (GetDeviceCapabilities()->GetShadowMapDepthFormat() != PF_UNKNOWN)
			return m_pShdowMapDepth.get();
		else
			return m_pShadowMapColor.get();
	}

	Matrix4 ShadowMapFrustum::CalculateTexAdjustMatrix(Texture* pShadowMap,Rectangle veiewPort)
	{
		Matrix4 texAdjust(Matrix4::IDENTITY);

		float width = (float)pShadowMap->GetWidth();
		float height = (float)pShadowMap->GetHeight();

		Vector2 offset( (float)veiewPort.left / width, (float)veiewPort.top / height);

		Vector2 scale( 0.5f * (float)veiewPort.width() / width, 0.5f * (float)veiewPort.height() / height );

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

			if (GetDeviceCapabilities()->GetShadowMapDepthFormat() != PF_UNKNOWN)
			{
				offset.x -= 0.5f / width;
				offset.y -= 0.5f / height;
			}

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

		if (m_eCaterType != CasterCull_No)
		{
			Frustum ligtFrustum;
			ligtFrustum.Update(m_matLightProj * m_matLightView, GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2);

			Frustum lightViewFrustum = m_frustum;
			lightViewFrustum.Transformed(m_matLightView);

			ShadowCasterQuery shaowQuery(ligtFrustum,lightViewFrustum,m_matLightView,m_arrCaster,m_casterAABB);
			pCamera->GetScene()->GetCullTree()->FindObjectsIn(shaowQuery);
		}

		for (VEC_CASTER::iterator iter = m_arrCaster.begin();iter != m_arrCaster.end();++iter)
		{
			RenderComponent* pRenderComp = (*iter);

			if (m_fShadowFarDist > 0)
			{
				float fLodValue = (pRenderComp->GetSceneNode()->GetPosWS() - pCamera->GetPosWS()).length();
				if (fLodValue > m_fShadowFarDist)
					continue;
			}
				
			uint32 nLod = pRenderComp->GetLodIndex();
			for (UINT i = 0; i < pRenderComp->GetRenderableCount(nLod); ++i)
			{
				m_arrRenderable[GetRenderSystem()->CurThreadFill()].push_back(pRenderComp->GetRenderableByIndex(nLod,i));
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
		Scene* pCurScene = pCamera->GetScene();

		Vector3 vLightDir = m_pParent->GetSceneNode()->GetForward();
	
		Vector3 vLightUp;
		if ( Math::Abs( (-vLightDir).dotProduct( pCamera->GetUp() ) ) > 0.95f )
		{
			vLightUp = pCamera->GetRight();
		}
		else
		{
			vLightUp = pCamera->GetUp();
		}

		Vector3 pos = pCamera->GetPosWS() - pCamera->GetFarClip() * vLightDir;

		Matrix4 matLightView = Math::MakeLookAtMatrixRH(pos,pos + vLightDir,vLightUp);
		Matrix4 matInvLightView = matLightView.inverse();

		AABB aabbInLightView;

		if (m_eCaterType == CasterCull_No)
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
		if (m_eCaterType == LightFrustum_Cull)
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
		//AABB splitBB = m_frustum.GetAABB();
		//splitBB.transform(matVP);

		AABB castersBB = m_casterAABB;
		castersBB.transform(matVP);

		AABB sceeneBB = m_sceneAABB;
		sceeneBB.transform(matVP);

		//AABB cropBB;
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
// 		RenderShadowCSM* pParent = pCamera->GetScene()->GetSunShaow();
// 
// 		float fConstantBias = 0;
// 		float fSlopeScaleBias = 0;
// 		pParent->GetDepthBiasParams(fConstantBias,fSlopeScaleBias);
// 
// 		float vCurSplitx = pParent->GetCurSplitPos().x;
// 		float multiplier = max(fSpiltFar / vCurSplitx, 1.0f);	
// 
// 		m_fConstantBias[GetRenderSystem()->CurThreadFill()] = fConstantBias * multiplier;
// 		m_fSlopeScaleBias[GetRenderSystem()->CurThreadFill()] = fConstantBias * multiplier;
	}

	void ShadowMapFrustum::Update(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		Clear(pCamera);

		UpdateDepthBias(pCamera,fSpiltNear,fSpiltFar);

		UpdateFrustum(pCamera,fSpiltNear,fSpiltFar);

		UpdateLightMatrix(pCamera,fSpiltNear,fSpiltFar);

		UpdateBatch(pCamera);

		UpdateCropMats();

		m_matLightViewProj[GetRenderSystem()->CurThreadFill()] = m_matCrop * m_matLightProj * m_matLightView;
		m_matShadow[GetRenderSystem()->CurThreadFill()] = m_matTexAdjust * m_matLightViewProj[GetRenderSystem()->CurThreadFill()];

// 		if (pCamera->GetDeferredShadowEnabled())
// 		{
// 			ProjectScreenToWorldExpansionBasis(m_matShadow,*pCamera,(float)pCamera->GetWidth(),(float)pCamera->GetHeight(),
// 				m_vWBasisX,m_vWBasisY,m_vWBasisZ,m_vShadowCamPos);
// 		}
// 
// 		RenderShadowCSM* pShadowCSM = pCamera->GetSceneManager()->GetRenderShadow();
// 		if (pShadowCSM->GetShadowBlurLevel() == ShadowBlur_JITTERIN)
// 		{
// 			Vector3 vViewPosLS = this->GetLightViewMatrix() * pCamera->GetEyeNode()->GetWorldPos();
// 			Vector3 vViewVectLS = this->GetLightViewMatrix() * pCamera->GetAtNode()->GetWorldPos() - vViewPosLS;
// 			m_viewPosVecLS = Vector4(vViewPosLS.x,vViewPosLS.y,vViewVectLS.x,vViewVectLS.y);
// 		}
	}

	void ShadowMapFrustum::Render(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		Rectangle rPreViewport = GetRenderSystem()->SetViewPort(m_viewport);
		RefPtr<Texture> pPreRenderTarget = GetRenderSystem()->SetRenderTarget(m_pShadowMapColor);
		RefPtr<Texture> pPreDepthStencil = GetRenderSystem()->SetDepthStencil(m_pShdowMapDepth);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.f, 0);

		if (GetDeviceCapabilities()->GetShadowMapDepthFormat() != PF_UNKNOWN)
		{
			GetRenderSystem()->SetColorWrite(false);

			float fConstantBias = m_fConstantBias[GetRenderSystem()->CurThreadProcess()];
			float fSlopeScaleBias = m_fSlopeScaleBias[GetRenderSystem()->CurThreadProcess()];
			GetRenderSystem()->SetDepthBias(fConstantBias,fSlopeScaleBias);
		}

		for (UINT i = 0; i <  m_arrRenderable[GetRenderSystem()->CurThreadProcess()].size(); ++i)
		{
			Renderable* pRenderObj = m_arrRenderable[GetRenderSystem()->CurThreadProcess()][i];
			if (pRenderObj == NULL)
				continue;
			 
			 SubMaterial* pMaterial = pRenderObj->GetMaterial();
		
			 Technique* pTech = pMaterial->GetShadowDepthTechnqiue();
 
			 GetRenderContext()->SetCurRenderObj(pRenderObj);

			 pTech->Bind();

			 Uniform* pUniform = pTech->GetShaderProgram()->GetUniform("matLightViewProj");
			 GetRenderSystem()->SetValue(pUniform,m_matLightViewProj[GetRenderSystem()->CurThreadProcess()]);

			 pRenderObj->Render(pTech);
		}

		GetRenderSystem()->SetViewPort(rPreViewport);
		GetRenderSystem()->SetRenderTarget(pPreRenderTarget.get());
		GetRenderSystem()->SetDepthStencil(pPreDepthStencil.get());

		if (GetDeviceCapabilities()->GetShadowMapDepthFormat() != PF_UNKNOWN)
		{
			GetRenderSystem()->SetDepthBias(0,0);

			GetRenderSystem()->SetColorWrite(true);
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

