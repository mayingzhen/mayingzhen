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
	DeferredShadow* gpDeferredShadow = nullptr;

	DeferredShadow::DeferredShadow()
	{
		gpDeferredShadow = this;

		m_pRenderable = new Renderable;

		GetRenderSystem()->AddShaderGlobaMacro("USING_SHADOW", "1");
	}

	void DeferredShadow::CreateSimpleLightFrustumMesh()
	{
		std::vector<Vector3> vertBuff;
		std::vector<uint16_t> indBuff;

		indBuff.clear();
		indBuff.reserve(36);

		vertBuff.clear();
		vertBuff.reserve(8);

		float nearZ = 0.0f;
		float farZ = 1.0f; // gl -1 ~ 1
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2) 
		{
			nearZ = -1.0f;
			farZ = 1.0f;
		}

		Vector3 vPoint[8];
		vPoint[0] = Vector3(-1,1,nearZ);
		vPoint[1] =  Vector3(-1,-1,nearZ) ;
		vPoint[2] = Vector3(1,-1,nearZ);
		vPoint[3] = Vector3(1,1,nearZ);

		vPoint[4] = Vector3(-1,1,farZ);
		vPoint[5] = Vector3(-1,-1,farZ);
		vPoint[6] = Vector3(1,-1,farZ);
		vPoint[7] = Vector3(1,1,farZ);

		for(uint32_t i = 0; i < 8; ++i)
		{
			vertBuff.push_back(vPoint[i]);
		}

		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_VULKAN)
		{
			for (uint32_t i = 0; i < 8; ++i)
			{
				vPoint[i].y *= -1.0f;
			}
		}

		//CC faces
		static uint16_t nFaces[6][4] = {{0,1,2,3},
		{4,7,6,5},
		{0,3,7,4},
		{1,5,6,2},
		{0,4,5,1},
		{3,2,6,7}
		};

		//init indices for triangles drawing
		for(int i=0; i < 6; i++)
		{
			indBuff.push_back( (uint16_t)  nFaces[i][0] );
			indBuff.push_back( (uint16_t)  nFaces[i][2] );
			indBuff.push_back( (uint16_t)  nFaces[i][1] );

			indBuff.push_back( (uint16_t)  nFaces[i][0] );
			indBuff.push_back( (uint16_t)  nFaces[i][3] );
			indBuff.push_back( (uint16_t)  nFaces[i][2] );
		}

		m_pRenderable->m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&vertBuff[0], sizeof(Vector3) * vertBuff.size(), sizeof(Vector3));

		m_pRenderable->m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((uint8_t*)&indBuff[0],sizeof(uint16_t) * indBuff.size(), sizeof(uint16_t));

		m_pRenderable->m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_pRenderable->m_pSubMeshData = CreateSubMeshData();
		m_pRenderable->m_pSubMeshData->m_nIndexCount = indBuff.size();
		m_pRenderable->m_pSubMeshData->m_nVertexCount = vertBuff.size();
	}

	void DeferredShadow::Init()
	{
		CreateSimpleLightFrustumMesh();
	}

	void DeferredShadow::Reset(Texture* pDepthStencil)
	{
		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_R,false);

		m_pShadowSampler = CreateSamplerState(m_pShadowTex.get(),CLAMP,TFO_POINT,false);

		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_TextureSceneShadow", [this](Renderable*)->SamplerState* {
			return m_pShadowSampler.get();
		});

		RefPtr<Texture> m_pTempDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);

		m_pShadowPass = GetRenderDevice()->CreateRenderPass();
		m_pShadowPass->AttachColor(0, RenderSurface(m_pShadowTex) );
		RenderSurface ds;
		ds.m_pTexture = pDepthStencil;
		ds.m_eLoadOp = LOAD_OP_LOAD;
		ds.m_eStoreOp = STORE_OP_STORE;
		m_pShadowPass->AttachDepthStencil(ds);
		GetRenderSystem()->RenderPassStreamComplete(m_pShadowPass.get());

		VertexElement element[1];
		element[0] = VertexElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		RefPtr<VertexDeclaration> pVolumeDeclaration = GetRenderSystem()->CreateVertexDeclaration(element, 1);

		ShaderCreateInfo volume_info;
		volume_info.m_pRenderPass = m_pShadowPass;
		volume_info.m_strVSFile = "volume.hlsl:vs_main";
		volume_info.m_strPSFile = "volume.hlsl:ps_main";
		volume_info.m_pVertexDecl = pVolumeDeclaration;

		uint32_t stenCillUse = 1 << SBU_DEFERREDSHADOW | 1 << (SBU_DEFERREDSHADOW + 1) | 1 << (SBU_DEFERREDSHADOW + 2);

		

		for (int i = 0; i < 4; ++i)
		{
			{
				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->m_bDepthWrite = false;
				pDSState->m_bStencil = true;
				pDSState->m_eStencilfunc = CMPF_ALWAYS_PASS;
				//pDSState->m_nStencilWriteMask = stenCillUse;
				//pDSState->m_nStencilMask = stenCillUse;
				pDSState->m_eStencilFail = SOP_KEEP;
				pDSState->m_eDepthFailOp = SOP_REPLACE;
				pDSState->m_eStencilPass = SOP_KEEP;

				pDSState->m_nStencilRef = (i * 2 + 1) << SBU_DEFERREDSHADOW;

				volume_info.m_pDSState = pDSState;

				m_pFrustumVolumeScale[i] = CreateTechnique(volume_info);
			}

			{
				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->m_bDepthWrite = false;
				pDSState->m_bStencil = true;
				pDSState->m_eStencilfunc = CMPF_ALWAYS_PASS;
				//pDSState->m_nStencilWriteMask = stenCillUse;
				//pDSState->m_nStencilMask = stenCillUse;
				pDSState->m_eStencilFail = SOP_KEEP;
				pDSState->m_eDepthFailOp = SOP_REPLACE;
				pDSState->m_eStencilPass = SOP_KEEP;

				pDSState->m_nStencilRef = (i * 2 + 2) << SBU_DEFERREDSHADOW;

				volume_info.m_pDSState = pDSState;

				m_pFrustumVolume[i] = CreateTechnique(volume_info);
			}
		}

		RefPtr<VertexDeclaration> pDec = CreateVertexDeclaration();
		pDec->AddElement(VertexElement(0, 0, DT_FLOAT2, DU_POSITION, 0));
		pDec->AddElement(VertexElement(0, 8, DT_FLOAT2, DU_TEXCOORD, 0));

		ShaderCreateInfo shadow_info;
		shadow_info.m_pRenderPass = m_pShadowPass;
		shadow_info.m_strVSFile = "DefferedShadow.vert:main";
		shadow_info.m_strPSFile = "DefferedShadow.frag:main";
		shadow_info.m_pVertexDecl = pDec;

		for (int i = 0; i < 4; ++i)
		{
			{
				shadow_info.m_pDSState = CreateDepthStencilState();
				shadow_info.m_pDSState->m_bDepthWrite = false;
				shadow_info.m_pDSState->m_bStencil = true;
				shadow_info.m_pDSState->m_eStencilfunc = CMPF_EQUAL;
				//shadow_info.m_pDSState->m_nStencilWriteMask = stenCillUse;
				//shadow_info.m_pDSState->m_nStencilMask = stenCillUse;
				shadow_info.m_pDSState->m_eStencilFail = SOP_KEEP;
				shadow_info.m_pDSState->m_eDepthFailOp = SOP_KEEP;
				shadow_info.m_pDSState->m_eStencilPass = SOP_KEEP;

				shadow_info.m_pDSState->m_nStencilRef = (i * 2 + 1) << SBU_DEFERREDSHADOW;

				m_pDefferedShadow[i] = CreateTechnique(shadow_info);
			}

			{
				ShaderCreateInfo blend_info = shadow_info;

				blend_info.m_shaderMacro = "FRUSTUM_BLEND";

				blend_info.m_pDSState = CreateDepthStencilState();
				blend_info.m_pDSState->m_bDepthWrite = false;
				blend_info.m_pDSState->m_bStencil = true;
				blend_info.m_pDSState->m_eStencilfunc = CMPF_EQUAL;
				//blend_info.m_pDSState->m_nStencilWriteMask = stenCillUse;
				//blend_info.m_pDSState->m_nStencilMask = stenCillUse;
				blend_info.m_pDSState->m_eStencilFail = SOP_KEEP;
				blend_info.m_pDSState->m_eDepthFailOp = SOP_KEEP;
				blend_info.m_pDSState->m_eStencilPass = SOP_KEEP;

				blend_info.m_pDSState->m_nStencilRef = (i * 2 + 2) << SBU_DEFERREDSHADOW;

				m_pBlendMaterial[i] = CreateTechnique(blend_info);
			}
		}
	}

	void DeferredShadow::Render()
	{
		if (m_vecFrustum.empty())
		{
			return;
		}

		RENDER_PROFILE(DeferredShadow);

		m_pShadowPass->Begine();

		RenderCommand* pRenderCommand = m_pShadowPass->GetThreadCommand(0, 0);
		pRenderCommand->Begin();
	
		float fBlendValue = 0.8f;
		
		for (int i = m_vecFrustum.size() - 1; i >= 0; --i) // 从后往前
		{
			SMFrustumInfo& shadowMapFru = m_vecFrustum[i];

			Matrix4 matFrum = shadowMapFru.m_matLightViewProj.inverse();
	
			// This frustum
			{
				m_pFrustumVolume[i]->SetValue(m_pFrustumVolume[i]->GetUniform(VS,"matFrustum"), matFrum );

				m_pRenderable->PreRender(m_pFrustumVolume[i].get());
				m_pRenderable->Render(m_pFrustumVolume[i].get(), pRenderCommand);
			}

			// This frustum, not including blend region
			{
				Matrix4 matBlend = Matrix4::IDENTITY;
				matBlend.setScale(Vector3(fBlendValue,fBlendValue,1.0f));
				matFrum = matFrum * matBlend;
				m_pFrustumVolumeScale[i]->SetValue(m_pFrustumVolumeScale[i]->GetUniform(VS,"matFrustum"), matFrum );

				m_pRenderable->PreRender(m_pFrustumVolumeScale[i].get());
				m_pRenderable->Render(m_pFrustumVolumeScale[i].get(), pRenderCommand);
			}
		}

		for (int i = 0; i < m_vecFrustum.size(); ++i)
		{
			SMFrustumInfo& shadowMapFru = m_vecFrustum[i];

			Matrix4 viewToShadow = shadowMapFru.m_matShadow * GetSceneContext()->GetViewMatrixInv();
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_matViewToShadow"), viewToShadow);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_tShadowMap"), shadowMapFru.m_pShadowDepth);

			Vector4 uv_calmp = shadowMapFru.m_uvClamp;
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "uv_calmp"), uv_calmp);

			ScreenQuad::Render(m_pDefferedShadow[i].get(), pRenderCommand);
		}

		// Blend
		for (int i = 0; i < m_vecFrustum.size() - 1; ++i)
		{
			SMFrustumInfo& shadowMapFru = m_vecFrustum[i];

			Matrix4 viewToShadow = shadowMapFru.m_matShadow * GetSceneContext()->GetViewMatrixInv();
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_matViewToShadow"), viewToShadow);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_tShadowMap"), shadowMapFru.m_pShadowDepth);

			SMFrustumInfo& shadowMapNextFru = m_vecFrustum[i + 1];
			Matrix4 nextViewToShadow = shadowMapNextFru.m_matShadow * GetSceneContext()->GetViewMatrixInv();
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_matNextViewToShadow"), nextViewToShadow);
		
			Vector4 vBlendInfo = Vector4(fBlendValue,1.0f / (1.0f - fBlendValue),fBlendValue,1.0f / (1.0f - fBlendValue));
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(VS,"BlendInfo"), vBlendInfo);

			Vector4 uv_calmp = shadowMapFru.m_uvClamp;
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "uv_calmp"), uv_calmp);

			ScreenQuad::Render(m_pBlendMaterial[i].get(), pRenderCommand);
		}

		pRenderCommand->End();

		m_pShadowPass->End();

		m_vecFrustum.clear();
	}

	void DeferredShadow::AddSMFrustumInfo(const SMFrustumInfo& info)
	{
		m_vecFrustum.emplace_back(info);
	}

	void DeferredShadow::Shoutdown()
	{

	}

	DeferredShadow* GetDeferredShadow()
	{
		return gpDeferredShadow;
	}

}
