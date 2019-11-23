#include "ShadowMapFrustum.h"
#include "../../RenderSystem/RenderCommand.h"

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
	
		m_vWBasisX = Vector4::ZERO;
		m_vWBasisY = Vector4::ZERO;
		m_vWBasisZ = Vector4::ZERO;
		m_vShadowCamPos = Vector4::ZERO;

		// shadow jitterin
		m_viewPosVecLS = Vector4::ZERO;
		m_vkernelRadius = Vector2(2.0f,2.0f);

		m_pRenderQueue[0] = new RenderQueue();
		m_pRenderQueue[1] = new RenderQueue();

		m_pShadowMapFB = GetRenderDevice()->CreateRenderPass();
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{
	}

	void ShadowMapFrustum::CreateShadowMap(int nSize)
	{
		m_viewport = Rectangle(1.0f, 1.0f, (float)nSize - 2.0f, (float)nSize - 2.0f);

		PixelFormat shadowMapDepthFormat = GetDeviceCapabilities()->GetShadowMapDepthFormat();	
		m_pShdowMapDepth = GetRenderSystem()->CreateDepthStencil(nSize,nSize,shadowMapDepthFormat);
		m_pShadowMapFB->AttachDepthStencil(m_pShdowMapDepth.get());
		m_pShadowMapFB->SetViewPort(m_viewport);
		GetRenderSystem()->RenderPassStreamComplete(m_pShadowMapFB.get());

		m_matTexAdjust = CalculateTexAdjustMatrix(m_pShdowMapDepth.get(),m_viewport);

		m_pShadowMapSampler = GetRenderDevice()->CreateSamplerState();
		m_pShadowMapSampler->SetWrapMode(CLAMP);
		m_pShadowMapSampler->SetTexture( m_pShdowMapDepth.get() );
		m_pShadowMapSampler->SetFilterMode(TFO_SHADOWCOMPARE);
		m_pShadowMapSampler->SetSRGB(false);
		
	}

	SamplerState* ShadowMapFrustum::GetShadowMap() const 
	{
		return m_pShadowMapSampler.get();
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

			if (GetRenderDevice()->GetRenderDeviceType() != RenderDevice_VULKAN)
			{
				scale.y = -scale.y;
			}

			texAdjust.setTrans(Vector3(offset.x, offset.y, 0.0f));
			texAdjust.setScale(Vector3(scale.x, scale.y, 1.0f));
		}

		return texAdjust;
	}

	void ShadowMapFrustum::UpdateBatch(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadFill()].get();
		pRenderQueue->Clear();

		bool bGLSystem = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2;
		bool bInvY = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_VULKAN;
		m_lightFrustum.Update(m_matLightProj * m_matLightView, bGLSystem, bInvY);

		pCamera->GetScene()->GetCullTree()->FindObjectsIn(&m_lightFrustum,-1,m_arrCaster);
		
		for (auto pRenderComp : m_arrCaster)
		{
			if (!pRenderComp->GetShadowCaster())
				continue;

			pRenderComp->RenderShadow(pRenderQueue);
		}
	}

	void ShadowMapFrustum::UpdateFrustum(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		Matrix4 matView = pCamera->GetMatView();
		Matrix4 matProj;
		GetRenderDevice()->MakePerspectiveMatrix(matProj,pCamera->GetFov(),pCamera->GetAspect(),fSpiltNear,fSpiltFar);

		bool bGLSystem = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2;
		bool bInvY = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_VULKAN;
		m_frustum.Update(matProj * matView, bGLSystem, bInvY);
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

		Vector3 pos = m_frustum.GetAABB().getCenter() - pCamera->GetFarClip() * vLightDir;

		Matrix4 matLightView = Math::MakeLookAtMatrixRH(pos,pos + vLightDir,vLightUp);
		Matrix4 matInvLightView = matLightView.inverse();

		AABB aabbInLightView;

		uint32_t iNodeNum = pCurScene->GetVisibleNodeNum();
		for (uint32_t i = 0; i < iNodeNum; ++i)
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
	
		if (aabbInLightView.isNull())
		{
			Frustum frustumInLightView = m_frustum;
			frustumInLightView.Transformed(matLightView);
			aabbInLightView = frustumInLightView.GetAABB();
		}

		Vector3 vCenter = aabbInLightView.getCenter();
		Vector3 vLigtViewSize = aabbInLightView.getSize();
		Vector3 vLightPos = matInvLightView * Vector3(vCenter.x,vCenter.y,0);
		vLigtViewSize.z = -aabbInLightView.getMinimum().z;

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

        /*
		Matrix4 matVP = m_matLightProj* m_matLightView;

		Frustum vpFurstum = m_frustum;
		vpFurstum.Transformed(matVP);
		AABB splitBB = vpFurstum.GetAABB();

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
         */

		m_matCrop = Matrix4::IDENTITY;
		//m_matCrop.setScale(Vector3(fScaleX,fScaleY,fScaleZ));
		//m_matCrop.setTrans(Vector3(fOffsetX,fOffsetY,fOffsetZ));
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

	void ProjectScreenToWorldExpansionBasis(const Matrix4& mShadowTexGen, const Camera& cam, float fViewWidth, float fViewHeight, 
		Vector4& vWBasisX, Vector4& vWBasisY, Vector4& vWBasisZ, Vector4& vCamPos)
	{
		ASSERT(false);

		Matrix4 camMatrix = cam.GetMatrixWS();
		//camMatrix = cam.GetViewMatrixInv();
		Vector3 vPos = camMatrix.getTrans();
		//vPos = cam.GetPosWS();

		// projection ratio
		float fProjectionRatio = fViewWidth / fViewHeight ;

		//all values are in camera space
		float fFar = cam.GetFarClip();
		float fNear	= cam.GetNearClip();
		float fWorldHeightDiv2 = fNear * Math::Tan( cam.GetFov() * 0.5f );
		float fWorldWidthDiv2 = fWorldHeightDiv2 * fProjectionRatio; 
		float k = fFar / fNear;

		Vector3 vZ = -cam.GetForward().normalisedCopy() * fNear * k; 
		Vector3 vX = cam.GetRight().normalisedCopy() * fWorldWidthDiv2 * k;
		Vector3 vY = cam.GetUp().normalisedCopy() * fWorldHeightDiv2 * k;

		vZ = vZ - vX;
		vX *= (2.0f / fViewWidth);   

		vZ = vZ + vY;
		vY *= -(2.0f / fViewHeight); 

		// Transform basis to any local space ( shadow space here )
		vWBasisX = mShadowTexGen * Vector4(vX, 0.0f);
		vWBasisY = mShadowTexGen * Vector4(vY, 0.0f);
		vWBasisZ = mShadowTexGen * Vector4(vZ, 0.0f);
		vCamPos =  mShadowTexGen * Vector4(vPos, 1.0f);
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

		//if (pCamera->GetScene()->GetRenderScheme()->GetDeferredShadingEnabled())
		{
			Rectangle rect = GetRenderSystem()->GetViewPort();
			ProjectScreenToWorldExpansionBasis(m_matShadow[GetRenderSystem()->CurThreadFill()],*pCamera,rect.width(),rect.height(),
				m_vWBasisX,m_vWBasisY,m_vWBasisZ,m_vShadowCamPos);
		}

		Vector3 vAtPos = pCamera->GetPosWS() + pCamera->GetForward() * 1000.0f;

		Vector3 vViewPosLS = this->GetLightViewMatrix() * pCamera->GetPosWS();
		Vector3 vViewVectLS = this->GetLightViewMatrix() * vAtPos - vViewPosLS;
		m_viewPosVecLS = Vector4(vViewPosLS.x,vViewPosLS.y,vViewVectLS.x,vViewVectLS.y);
	}

	void ShadowMapFrustum::Render(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()].get();
		GetRenderSystem()->AddRenderStep(pRenderQueue, m_pShadowMapFB.get());

// 		m_pShadowMapFB->Begine();
// 
// 		RenderQueue* pRenderQueue = m_pRenderQueue[GetRenderSystem()->CurThreadProcess()];
// 		pRenderQueue->Render(m_pShadowMapFB.get(),RL_Mesh, RL_MeshTrans);
// 
// 		m_pShadowMapFB->End();

	}

	void ShadowMapFrustum::Clear(Camera* pCamera)
	{
		m_pRenderQueue[GetRenderSystem()->CurThreadFill()]->Clear();
		m_casterAABB.setNull();
		m_arrCaster.clear();	
		m_sceneAABB.setNull();
	}
}

