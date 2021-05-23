#include "ShadowMapFrustum.h"
#include "../../RenderSystem/RenderCommand.h"
#include "../../RenderScheme/DeferredShadowPass.h"


namespace ma
{
	ShadowRenderStep::ShadowRenderStep()
	{
		m_fConstantBias = 0;
		m_fSlopeScaleBias = 0;

		m_bDraw = true;

		m_matLightView = Matrix4::IDENTITY;
		m_matLightProj = Matrix4::IDENTITY;
		m_matLightViewProj = Matrix4::IDENTITY;
		m_matCrop = Matrix4::IDENTITY;
		m_matShadow  = Matrix4::IDENTITY;
		m_matTexAdjust = Matrix4::IDENTITY;
	}

	ShadowRenderStep::~ShadowRenderStep()
	{
	}

	void ShadowRenderStep::InitShadowMap(DirLightProxy* pParent, Rectangle viewPort, RenderPass* pSMPass)
	{
		m_shadowMapPass = pSMPass;

		m_pParent = pParent;

		m_veiwPort = viewPort;

		Texture* pShadowTexture = pSMPass->m_depthStencil.m_pTexture.get();

		float width = (float)pShadowTexture->GetWidth();
		float height = (float)pShadowTexture->GetHeight();
		Vector2 mini((float)viewPort.left / width, (float)viewPort.top / height);
		Vector2 offset((float)viewPort.width() / width, (float)viewPort.height() / height);
		Vector2 max = mini + offset;
		m_uvClamp = Vector4(mini.x, mini.y, max.x, max.y);

		m_matTexAdjust = CalculateTexAdjustMatrix(pShadowTexture, viewPort);
	}

	Matrix4 ShadowRenderStep::CalculateTexAdjustMatrix(Texture* pShadowMap,Rectangle veiewPort)
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

			texAdjust.setTrans(Vector3(offset.x, offset.y, 0.5f));
			texAdjust.setScale(Vector3(scale.x, scale.y, 0.5f));
		}
		else
		{
			offset.x += scale.x;
			offset.y += scale.y;

			if (GetRenderDevice()->GetRenderDeviceType() != RenderDevice_VULKAN)
			{
				scale.y = -scale.y;
			}

			texAdjust.setTrans(Vector3(offset.x, offset.y, 0.0f));
			texAdjust.setScale(Vector3(scale.x, scale.y, 1.0f));
		}

		return texAdjust;
	}

	void ShadowRenderStep::UpdateBatch(Camera* pCamera)
	{
		if (!m_bDraw)
			return;

		m_pRenderQueue->Clear();

		bool bGLSystem = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2;
		bool bInvY = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_VULKAN;
		m_lightFrustum.Update(m_matLightProj * m_matLightView, bGLSystem, bInvY);

		std::vector<RenderProxy*> arrCaster;
		pCamera->GetScene()->GetCullTree()->FindObjectsIn(&m_lightFrustum,-1, arrCaster);
		
		for (auto& pRenderComp : arrCaster)
		{
			if (!pRenderComp->GetShadowCaster())
			{
				continue;
			}

			m_casterAABB.merge(pRenderComp->GetAABBWS());

			for (uint32_t i = 0; i < pRenderComp->GetRenderableCount(); ++i)
			{
				Renderable* renderable = pRenderComp->GetRenderableByIndex(i);
				if (renderable == nullptr)
				{
					continue;
				}

				Technique* tech = renderable->GetMaterial()->GetShadowDepthTechnqiue(m_shadowMapPass);
				if (tech == nullptr || tech->GetTransluce())
				{
					continue;
				}

				m_batchRender.AddRenderObj(renderable, tech);
			}
		}

		m_batchRender.PrepareRender(m_pRenderQueue.get());
	}

	void ShadowRenderStep::UpdateFrustum(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		Matrix4 matView = pCamera->GetMatView();
		Matrix4 matProj;
		GetRenderDevice()->MakePerspectiveMatrix(matProj,pCamera->GetFov(),pCamera->GetAspect(),fSpiltNear,fSpiltFar);

		bool bGLSystem = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2;
		bool bInvY = GetRenderDevice()->GetRenderDeviceType() == RenderDevice_VULKAN;
		m_frustum.Update(matProj * matView, bGLSystem, bInvY);
	}


	void ShadowRenderStep::UpdateLightMatrix(Camera* pCamera, float fSpiltNear, float fSpiltFar)
	{
		Scene* pCurScene = pCamera->GetScene();

		uint32_t index = GetRenderSystem()->CurThreadProcess();

		Vector3 vLightDir = m_pParent->m_vDir[index];

		Vector3 vLightUp;
		if (Math::Abs((-vLightDir).dotProduct(pCamera->GetUp())) > 0.95f)
		{
			vLightUp = pCamera->GetRight();
		}
		else
		{
			vLightUp = pCamera->GetUp();
		}

		Vector3 pos = m_frustum.GetAABB().getCenter() - pCamera->GetFarClip() * vLightDir;

		Matrix4 matLightView = Math::MakeLookAtMatrixLH(pos, pos + vLightDir, vLightUp);
		Matrix4 matInvLightView = matLightView.inverse();

		AABB aabbInLightView;

		uint32_t iNodeNum = 0;//pCurScene->GetVisibleNodeNum();
		for (uint32_t i = 0; i < iNodeNum; ++i)
		{
			RenderComponent* pNode = nullptr;// pCurScene->GetVisibleNodeByIndex(i);

			if (pNode->GetViewMinZ() > fSpiltFar || pNode->GetViewMaxZ() < fSpiltNear)
				continue;

			m_sceneAABB.merge(pNode->GetAABBWS());
		}

		if (1)
		{
			ConvexBody frustumVolume;
			frustumVolume.define(m_frustum);
			frustumVolume.clip(m_sceneAABB);
			if (frustumVolume.getPolygonCount() <= 0)
			{
				m_bDraw = false;
			}
			else
			{
				m_bDraw = true;

				frustumVolume.transformed(matLightView);
				aabbInLightView = frustumVolume.getAABB();
			}

			if (aabbInLightView.isNull())
			{
				Frustum frustumInLightView = m_frustum;
				frustumInLightView.Transformed(matLightView);
				aabbInLightView = frustumInLightView.GetAABB();
			}
		}
		else
		{
			Frustum frustumInLightView = m_frustum;
			frustumInLightView.Transformed(matLightView);
			aabbInLightView = frustumInLightView.GetAABB();
		}
		

		Vector3 vCenter = aabbInLightView.getCenter();
		Vector3 vLigtViewSize = aabbInLightView.getSize();
		Vector3 vLightPos = matInvLightView * Vector3(vCenter.x,vCenter.y,0);
		vLigtViewSize.z = aabbInLightView.getMaximum().z;

		m_matLightView = Math::MakeLookAtMatrixLH(vLightPos, vLightPos + vLightDir, vLightUp);

		QuantizeViewSize(vLigtViewSize.x,vLigtViewSize.y);

		vLigtViewSize.x = max(vLigtViewSize.x,1.0f);
		vLigtViewSize.y = max(vLigtViewSize.y,1.0f);
		vLigtViewSize.z = max(vLigtViewSize.z,1.0f);

		GetRenderDevice()->MakeOrthoMatrix(m_matLightProj,vLigtViewSize.x,vLigtViewSize.y,0.0f,vLigtViewSize.z); 
	}

	void ShadowRenderStep::QuantizeViewSize(float& fWidth,float& fHight)
	{
		float fQuantize = 0.5f;
		fWidth = ceilf(sqrtf(fWidth / fQuantize));
		fHight = ceilf(sqrtf(fHight / fQuantize));
		fWidth = fWidth * fWidth * fQuantize;
		fHight = fHight * fHight * fQuantize;

	}

	void ShadowRenderStep::UpdateCropMats()
	{
		m_matCrop = Matrix4::IDENTITY;
		return;

		if (!m_bDraw || m_casterAABB.isNull())
		{
			m_matCrop = Matrix4::IDENTITY;
			return;
		}

		Matrix4 matVP = m_matLightProj* m_matLightView;;

		AABB castersBB = m_casterAABB;
		castersBB.transform(matVP);

		//AABB cropBB;
		Vector3 vMax = castersBB.getMaximum();
		Vector3 vMin = castersBB.getMinimum();

		float fScaleX = 2.0f / (vMax.x - vMin.x);
		float fScaleY = 2.0f / (vMax.y - vMin.y);

		float fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
		float fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

		float fScaleZ = 1.0f / (vMax.z - vMin.z);
		float fOffsetZ = -vMin.z * fScaleZ;
         
		fScaleX = std::max<float>(fScaleX, 1.0f);
		fScaleY = std::max<float>(fScaleY, 1.0f);
		m_matCrop.setScale(Vector3(fScaleX,fScaleY,1.0f));
		m_matCrop.setTrans(Vector3(fOffsetX,fOffsetY,0.0f));
	}

	void ShadowRenderStep::UpdateDepthBias(Camera* pCamera,float fSpiltNear,float fSpiltFar)
	{
		float fConstantBias = 0;
		float fSlopeScaleBias = 0;
		//m_pParent->GetDepthBiasParams(fConstantBias,fSlopeScaleBias);

		//float vCurSplitx = m_pParent->GetCurSplitPos().x;
		//float multiplier = max(fSpiltFar / vCurSplitx, 1.0f);	

		//m_fConstantBias = fConstantBias * multiplier;
		//m_fSlopeScaleBias = fConstantBias * multiplier;
	}

	void ShadowRenderStep::PrepareRender()
	{

	}

	void ShadowRenderStep::Update(Camera* pCamera, float fSpiltNear,float fSpiltFar)
	{
		m_fNear = fSpiltNear;
		m_fFar = fSpiltFar;

		Clear();

		UpdateDepthBias(pCamera,fSpiltNear,fSpiltFar);

		UpdateFrustum(pCamera,fSpiltNear,fSpiltFar);

		UpdateLightMatrix(pCamera,fSpiltNear,fSpiltFar);

		UpdateBatch(pCamera);

		UpdateCropMats();

		UpdateDefferedShadow(pCamera);

		m_matLightViewProj = m_matCrop * m_matLightProj * m_matLightView;
		m_matShadow = m_matTexAdjust * m_matLightViewProj;

		m_bDraw = true;
	}

	void ShadowRenderStep::UpdateDefferedShadow(Camera* pCamera)
	{
		SMFrustumInfo info;
		info.m_fNear = m_fNear;
		info.m_fFar = m_fFar;
		info.m_matLightViewProj = m_matLightProj * m_matLightView/*m_matLightViewProj*/;
		info.m_matShadow = m_matShadow;
		//info.m_pShadowDepth = m_pParent->GetShadowMapSampler();
		info.m_uvClamp = m_uvClamp;

		Matrix4 matView = pCamera->GetMatView();
		Matrix4 matProj;
		GetRenderDevice()->MakePerspectiveMatrix(matProj, pCamera->GetFov(), pCamera->GetAspect(), m_fNear, m_fFar);
		Matrix4 viewPoj = matProj * matView;
		info.m_matFrustum = viewPoj.inverse();

		Vector4 vNear = pCamera->GetMatProj() * Vector4(0.0f, 0.0f, m_fNear, 1.0);
		Vector4 vFar = pCamera->GetMatProj() * Vector4(0.0f, 0.0f, m_fFar, 1.0);
		info.m_fNear = vNear.z / vNear.w;
		info.m_fFar = vFar.z / vFar.w;

		GetRenderSystem()->RC_AddRenderCommad([info]() {
			GetDeferredShadow()->AddSMFrustumInfo(info);
			});
	}


	void ShadowRenderStep::Clear()
	{
		m_casterAABB.setNull();
		//m_arrCaster.clear();	
		m_sceneAABB.setNull();
	}
}

