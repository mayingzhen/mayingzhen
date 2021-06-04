#include "DeferredShadowPass.h"
#include "../Scene/Light/DirectonalLight.h"

// 模板0位贴花遮挡 1位描边 2~4延迟阴影 5人物
enum StencilBitUse
{
	SBU_Decal = 0,
	SBU_Edge = 1,
	SBU_DEFERREDSHADOW = 2, // 2~3~4
	SBU_SKIN = 5,
};

namespace ma
{
	DeferredShadow::DeferredShadow(Texture* pDepthStencil)
	{
		m_strName = "DeferredShadowStep";

		GetRenderSystem()->AddShaderGlobaMacro("USING_SHADOW", "1");

		Reset(pDepthStencil);
	}

	void DeferredShadow::Reset(Texture* pDepthStencil)
	{
		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_R,false);

		m_pShadowSampler = CreateSamplerState(m_pShadowTex.get(),CLAMP,TFO_POINT,false);

		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_TextureSceneShadow", [this](Renderable*,SceneContext*)->SamplerState* {
			return m_pShadowSampler.get();
		});

		m_pRenderPass = GetRenderDevice()->CreateRenderPass();
		m_pRenderPass->AttachColor(0, RenderSurface(m_pShadowTex) );
		RenderSurface ds;
		ds.m_pTexture = pDepthStencil;
		ds.m_eLoadOp = LOAD_OP_LOAD;
		ds.m_eStoreOp = STORE_OP_STORE;
		m_pRenderPass->AttachDepthStencil(ds);
		GetRenderSystem()->RenderPassStreamComplete(m_pRenderPass.get());

		VertexElement element[1];
		element[0] = VertexElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		RefPtr<VertexDeclaration> pVolumeDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 1);

		RefPtr<VertexDeclaration> pDec = CreateVertexDeclaration();
		pDec->AddElement(VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0));
		pDec->AddElement(VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0));

		ShaderCreateInfo shadow_info;
		shadow_info.m_pRenderPass = m_pRenderPass;
		shadow_info.m_strVSFile = "defferedShadow.hlsl:vs_main";
		shadow_info.m_strPSFile = "defferedShadow.hlsl:ps_main";
		shadow_info.m_pVertexDecl = pDec;
		shadow_info.m_bDepthBouds = true;

		for (int i = 0; i < 4; ++i)
		{
			shadow_info.m_pDSState = CreateDepthStencilState();
			shadow_info.m_pDSState->m_bDepthWrite = false;
// 			shadow_info.m_pDSState->m_bStencil = true;
// 			shadow_info.m_pDSState->m_eStencilfunc = CMPF_NOT_EQUAL;
// 			//shadow_info.m_pDSState->m_nStencilWriteMask = stenCillUse;
// 			//shadow_info.m_pDSState->m_nStencilMask = stenCillUse;
// 			shadow_info.m_pDSState->m_eStencilFail = SOP_ZERO;
// 			shadow_info.m_pDSState->m_eDepthFailOp = SOP_KEEP;
// 			shadow_info.m_pDSState->m_eStencilPass = SOP_ZERO;
// 
// 			shadow_info.m_pDSState->m_nStencilRef = 0;//i << SBU_DEFERREDSHADOW;

			m_pDefferedShadow[i] = CreateTechnique(shadow_info);
		}
	}

	void DeferredShadow::Render(SceneContext* sc)
	{
		GetRenderSystem()->BeginProfile(m_strName.c_str());

		m_pRenderPass->Begine();

		RenderCommand* pRenderCommand = m_pRenderPass->GetThreadCommand(0);

		pRenderCommand->Begin();

		for (int i = 0; i < m_vecFrustum.size(); ++i)
		{
			SMFrustumInfo& shadowMapFru = m_vecFrustum[i];

			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_matViewToShadow"), shadowMapFru.m_matViewToShadow);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_tShadowMap"), shadowMapFru.m_pShadowDepth);

			pRenderCommand->SetDepthBounds(shadowMapFru.m_fDepthNear, shadowMapFru.m_fDepthFar);

			ScreenQuad::Render(m_pDefferedShadow[i].get(), pRenderCommand, sc);
		}
		pRenderCommand->End();

		m_vecFrustum.clear();

		m_pRenderPass->End();

		GetRenderSystem()->EndProfile();
	}

	void DeferredShadow::AddSMFrustumInfo(const SMFrustumInfo& info)
	{
		m_vecFrustum.emplace_back(info);
	}

}
