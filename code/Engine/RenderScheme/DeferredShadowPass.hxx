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
	DeferredShadow::DeferredShadow()
	{
		m_pRenderable = new Renderable;
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

	void DeferredShadow::Reset()
	{
		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_R,false);

		m_pShadowSampler = CreateSamplerState(m_pShadowTex.get(),CLAMP,TFO_POINT,false);

		m_pShadowPass = GetRenderDevice()->CreateRenderPass();
		m_pShadowPass->AttachColor(0, m_pShadowTex.get(), 0, 0);
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

		RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
		pDSState->m_bStencil = true;
		pDSState->m_eStencilfunc = CMPF_ALWAYS_PASS;
		pDSState->m_nStencilWriteMask = stenCillUse;
		pDSState->m_nStencilMask = stenCillUse;
		pDSState->m_eStencilFail = SOP_KEEP;
		pDSState->m_eDepthFailOp = SOP_REPLACE;
		pDSState->m_eStencilPass = SOP_KEEP;

		volume_info.m_pDSState = pDSState;

		for (int i = 4/*m_ShadowLight->GetCurSplitCount()*/ - 1; i >= 0; --i) // 从后往前
		{
			{
				pDSState->m_nStencilRef = (i * 2 + 2) << SBU_DEFERREDSHADOW;
				m_pFrustumVolume[i] = CreateTechnique(volume_info);
			}

			{
				pDSState->m_nStencilRef = (i * 2 + 1) << SBU_DEFERREDSHADOW;
				m_pFrustumVolumeScale[i] = CreateTechnique(volume_info);
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

		shadow_info.m_pDSState = CreateDepthStencilState();
		shadow_info.m_pDSState->m_bDepthWrite = false;
		shadow_info.m_pDSState->m_bStencil = true;
		shadow_info.m_pDSState->m_eStencilfunc = CMPF_EQUAL;
		shadow_info.m_pDSState->m_nStencilWriteMask = stenCillUse;
		shadow_info.m_pDSState->m_nStencilMask = stenCillUse;
		shadow_info.m_pDSState->m_eStencilFail = SOP_KEEP;
		shadow_info.m_pDSState->m_eDepthFailOp = SOP_KEEP;
		shadow_info.m_pDSState->m_eStencilPass = SOP_KEEP;

		for (int i = 0; i < 4; ++i)
		{
			{
				shadow_info.m_pDSState->m_nStencilRef = (i * 2 + 1) << SBU_DEFERREDSHADOW;

				m_pDefferedShadow[i] = CreateTechnique(shadow_info);
			}

			{
				ShaderCreateInfo blend_info = shadow_info;

				blend_info.m_shaderMacro = "FRUSTUM_BLEND";

				blend_info.m_pDSState->m_nStencilRef = (i * 2 + 2) << SBU_DEFERREDSHADOW;

				m_pBlendMaterial[i] = CreateTechnique(blend_info);
			}
		}
	}

	void DeferredShadow::Render()
	{
		RENDER_PROFILE(DeferredShadow);

		m_ShadowLight = GetRenderSystem()->GetScene()->GetMainDirLight();

		m_pShadowPass->Begine();

		RenderCommand* pRenderCommand = m_pShadowPass->GetThreadCommand(0, 0);
		pRenderCommand->Begin();
	
		float fBlendValue = 0.8f;
		
		for (int i = m_ShadowLight->GetCurSplitCount() - 1; i >= 0; --i) // 从后往前
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			//m_pFrustumVolume[i]->Bind(NULL);

			// This frustum
			{
				//GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 2) << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
				//	SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				//ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				m_pFrustumVolume[i]->SetValue(m_pFrustumVolume[i]->GetUniform(VS,"matFrustum"), matFrum );

				//GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
				m_pRenderable->PreRender(m_pFrustumVolume[i].get());
				m_pRenderable->Render(m_pFrustumVolume[i].get(), pRenderCommand);
			}

			// This frustum, not including blend region
			{
				//GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 1)  << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
				//	SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				//ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				Matrix4 matBlend = Matrix4::IDENTITY;
				matBlend.setScale(Vector3(fBlendValue,fBlendValue,1.0f));
				matFrum = matFrum * matBlend;
				m_pFrustumVolume[i]->SetValue(m_pFrustumVolume[i]->GetUniform(VS,"matFrustum"), matFrum );

				//GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolumeScale[i].get());
				m_pRenderable->PreRender(m_pFrustumVolumeScale[i].get());
				m_pRenderable->Render(m_pFrustumVolumeScale[i].get(), pRenderCommand);
			}
		}

		for (int i = 0; i < m_ShadowLight->GetCurSplitCount(); ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			//uint32_t refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
			//GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
			//	SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			//ShaderProgram* pShader = m_pDefferedShadow->GetShaderProgram();

			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "vStoWBasisX"),shadowMapFru.m_vWBasisX);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "vStoWBasisY"),shadowMapFru.m_vWBasisY);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "vStoWBasisZ"),shadowMapFru.m_vWBasisZ);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "vStoCamPos"),shadowMapFru.m_vShadowCamPos);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "kernelRadius"),shadowMapFru.m_vkernelRadius);
			m_pDefferedShadow[i]->SetValue(m_pDefferedShadow[i]->GetUniform(PS, "g_tShadowMap"), m_ShadowLight->GetShadowMapSampler());

			ScreenQuad::Render(m_pDefferedShadow[i].get(), pRenderCommand);
		}

		//ShaderProgram* pBlendShader = m_pBlendMaterial->GetShaderProgram();

		// Blend
		for (int i = 0; i < m_ShadowLight->GetCurSplitCount() - 1; ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			RenderPass* shadowMapPass = m_ShadowLight->GetShadowMapPass();

			ShadowMapFrustum& shadowMapNextFru = m_ShadowLight->GetShadowMapFrustum(i + 1);

			//uint32_t refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
			//GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
			//	SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vNextStoWBasisX"), shadowMapNextFru.m_vWBasisX);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vNextStoWBasisY"), shadowMapNextFru.m_vWBasisY);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vNextStoWBasisZ"), shadowMapNextFru.m_vWBasisZ);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vNextStoCamPos"), shadowMapNextFru.m_vShadowCamPos);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_vNextViewPosVecLS"), shadowMapNextFru.m_viewPosVecLS);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "NextkernelRadius"),shadowMapNextFru.m_vkernelRadius);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_tNextShadowMap"), m_ShadowLight->GetShadowMapSampler());

			float fNextSize = shadowMapPass->m_pDepthStencil->GetWidth();
			Vector4 vNextshadowMapTexelSize(fNextSize, 1.0f / fNextSize, 0, 0);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(VS,"g_NextshadowMapTexelSize"),vNextshadowMapTexelSize);

			Vector4 vBlendInfo = Vector4(fBlendValue,1.0f / (1.0f - fBlendValue),fBlendValue,1.0f / (1.0f - fBlendValue));
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(VS,"BlendInfo"), vBlendInfo);

			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vStoWBasisX"), shadowMapFru.m_vWBasisX);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vStoWBasisY"), shadowMapFru.m_vWBasisY);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vStoWBasisZ"), shadowMapFru.m_vWBasisZ);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "vStoCamPos"), shadowMapFru.m_vShadowCamPos);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "kernelRadius"),shadowMapFru.m_vkernelRadius);
			m_pBlendMaterial[i]->SetValue(m_pBlendMaterial[i]->GetUniform(PS, "g_tShadowMap"), m_ShadowLight->GetShadowMapSampler());

			ScreenQuad::Render(m_pDefferedShadow[i].get(), pRenderCommand);
		}

		pRenderCommand->End();

		m_pShadowPass->End();
	}

	void DeferredShadow::Shoutdown()
	{

	}

}
