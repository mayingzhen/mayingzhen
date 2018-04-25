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
	
		//m_ShadowLight = pScene->GetDirLight();
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

		//CCW faces
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
			indBuff.push_back( (uint16_t)  nFaces[i][1] );
			indBuff.push_back( (uint16_t)  nFaces[i][2] );

			indBuff.push_back( (uint16_t)  nFaces[i][0] );
			indBuff.push_back( (uint16_t)  nFaces[i][2] );
			indBuff.push_back( (uint16_t)  nFaces[i][3] );
		}

		m_pRenderable->m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&vertBuff[0], sizeof(Vector3) * vertBuff.size(), sizeof(Vector3));

		m_pRenderable->m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((uint8_t*)&indBuff[0],sizeof(uint16_t) * indBuff.size(), sizeof(uint16_t));

		m_pRenderable->m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_pRenderable->m_pSubMeshData = CreateSubMeshData();
		m_pRenderable->m_pSubMeshData->m_nIndexCount = indBuff.size();
		m_pRenderable->m_pSubMeshData->m_nVertexCount = vertBuff.size();

		VertexElement element[1];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		//m_pRenderable->m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,1); 
	}

	void DeferredShadow::Init()
	{
		m_pFrustumVolume = CreateTechnique("frustumclipvolume","volume","volume","");

		m_pDefferedShadow = CreateTechnique("DeferredShadow","DefferedShadow","DefferedShadow","");
		RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
		pDSState->m_bDepthWrite = false;
		m_pDefferedShadow->SetDepthStencilState(pDSState.get());

		m_pBlendMaterial = CreateTechnique("DeferredShadowBlend","DefferedShadow","DefferedShadow","FRUSTUM_BLEND");

		m_pScreen = CreateTechnique("screen","screen","screen","");
		RefPtr<BlendState> pBlendState = CreateBlendState();
		pBlendState->m_eBlendMode = BM_MULTIPLY;
		m_pScreen->SetBlendState(pBlendState.get());

		CreateSimpleLightFrustumMesh();
	}

	void DeferredShadow::Reset()
	{
		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_R,false);

		m_pShadowSampler = CreateSamplerState(m_pShadowTex.get(),CLAMP,TFO_POINT,false);
	}

	void DeferredShadow::Render()
	{
		RENDER_PROFILE(DeferredShadow);

		RefPtr<Texture> pPreTarget = GetRenderSystem()->GetRenderTarget(0);

// 		FrameBuffer fb;
// 		fb.AttachColor(0,m_pShadowTex.get());
// 		GetRenderSystem()->SetFrameBuffer(&fb);

		//GetRenderSystem()->ClearBuffer(true,false,false,ColourValue::White,1,0);
		
		m_pFrustumVolume->Bind(NULL);

		float fBlendValue = 0.8f;
		
		//uint32_t stenCillUse = 1 << SBU_DEFERREDSHADOW | 1 << (SBU_DEFERREDSHADOW + 1) | 1 << (SBU_DEFERREDSHADOW + 2);

	//	GetRenderSystem()->SetStencilCheckEnabled(true);
		for (int i = m_ShadowLight->GetCurSplitCount() - 1; i >= 0; --i) // 从后往前
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			// This frustum
			{
// 				GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 2) << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
// 					SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				//ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				m_pFrustumVolume->SetValue(m_pFrustumVolume->GetUniform("matFrustum"), matFrum );

				//GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
			}

			// This frustum, not including blend region
			{
// 				GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 1)  << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
// 					SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				//ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				Matrix4 matBlend = Matrix4::IDENTITY;
				matBlend.setScale(Vector3(fBlendValue,fBlendValue,1.0f));
				matFrum = matFrum * matBlend;
				m_pFrustumVolume->SetValue(m_pFrustumVolume->GetUniform("matFrustum"), matFrum );

				//GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
			}

		}

		for (int i = 0; i < m_ShadowLight->GetCurSplitCount(); ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

// 			uint32_t refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
// 			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
// 				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			//ShaderProgram* pShader = m_pDefferedShadow->GetShaderProgram();

			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("vStoWBasisX"),shadowMapFru.m_vWBasisX);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("vStoWBasisY"),shadowMapFru.m_vWBasisY);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("vStoWBasisZ"),shadowMapFru.m_vWBasisZ);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("vStoCamPos"),shadowMapFru.m_vShadowCamPos);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("kernelRadius"),shadowMapFru.m_vkernelRadius);
			m_pDefferedShadow->SetValue(m_pDefferedShadow->GetUniform("g_tShadowMap"),shadowMapFru.GetShadowMap());

			//ScreenQuad::Render(m_pDefferedShadow.get());
		}

		//ShaderProgram* pBlendShader = m_pBlendMaterial->GetShaderProgram();

		// Blend
		for (int i = 0; i < m_ShadowLight->GetCurSplitCount() - 1; ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			ShadowMapFrustum& shadowMapNextFru = m_ShadowLight->GetShadowMapFrustum(i + 1);

// 			uint32_t refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
// 			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
// 				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vNextStoWBasisX"), shadowMapNextFru.m_vWBasisX);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vNextStoWBasisY"), shadowMapNextFru.m_vWBasisY);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vNextStoWBasisZ"), shadowMapNextFru.m_vWBasisZ);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vNextStoCamPos"), shadowMapNextFru.m_vShadowCamPos);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("g_vNextViewPosVecLS"), shadowMapNextFru.m_viewPosVecLS);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("NextkernelRadius"),shadowMapNextFru.m_vkernelRadius);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("g_tNextShadowMap"),shadowMapNextFru.GetShadowMap());

			float fNextSize = (float)shadowMapNextFru.GetShadowMap()->GetTexture()->GetWidth();
			Vector4 vNextshadowMapTexelSize(fNextSize, 1.0f / fNextSize, 0, 0);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("g_NextshadowMapTexelSize"),vNextshadowMapTexelSize);

			Vector4 vBlendInfo = Vector4(fBlendValue,1.0f / (1.0f - fBlendValue),fBlendValue,1.0f / (1.0f - fBlendValue));
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("BlendInfo"), vBlendInfo);

			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vStoWBasisX"), shadowMapFru.m_vWBasisX);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vStoWBasisY"), shadowMapFru.m_vWBasisY);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vStoWBasisZ"), shadowMapFru.m_vWBasisZ);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("vStoCamPos"), shadowMapFru.m_vShadowCamPos);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("kernelRadius"),shadowMapFru.m_vkernelRadius);
			m_pBlendMaterial->SetValue(m_pBlendMaterial->GetUniform("g_tShadowMap"),shadowMapFru.GetShadowMap());

			//ScreenQuad::Render(m_pDefferedShadow.get());
		}

// 		if (!m_pScene->GetRenderScheme()->GetDeferredShadingEnabled())
// 		{
// 			FrameBuffer fb;
// 			fb.AttachColor(0,pPreTarget.get());
// 			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
// 			GetRenderSystem()->SetFrameBuffer(&fb);

			//ShaderProgram* pShader = m_pScreen->GetShaderProgram();
			//m_pScreen->SetValue(m_pScreen->GetUniform("tSrcColor"),m_pShadowTex.get());

			//ScreenQuad::Render(m_pScreen.get());	
		//}
	}

	void DeferredShadow::Shoutdown()
	{

	}

}
