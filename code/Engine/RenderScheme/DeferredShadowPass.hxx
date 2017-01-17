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
	DeferredShadowPass::DeferredShadowPass(Scene* pScene)
		:RenderPass(pScene)
	{
		m_pRenderable = new Renderable;
	
		m_ShadowLight = pScene->GetDirLight();
	}

	void DeferredShadowPass::CreateSimpleLightFrustumMesh()
	{
		std::vector<Vector3> vertBuff;
		std::vector<uint16> indBuff;

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

		for(uint32 i = 0; i < 8; ++i)
		{
			vertBuff.push_back(vPoint[i]);
		}

		//CCW faces
		static uint16 nFaces[6][4] = {{0,1,2,3},
		{4,7,6,5},
		{0,3,7,4},
		{1,5,6,2},
		{0,4,5,1},
		{3,2,6,7}
		};

		//init indices for triangles drawing
		for(int i=0; i < 6; i++)
		{
			indBuff.push_back( (uint16)  nFaces[i][0] );
			indBuff.push_back( (uint16)  nFaces[i][1] );
			indBuff.push_back( (uint16)  nFaces[i][2] );

			indBuff.push_back( (uint16)  nFaces[i][0] );
			indBuff.push_back( (uint16)  nFaces[i][2] );
			indBuff.push_back( (uint16)  nFaces[i][3] );
		}

		m_pRenderable->m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((UINT8*)&vertBuff[0], sizeof(Vector3) * vertBuff.size(), sizeof(Vector3));

		m_pRenderable->m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((UINT8*)&indBuff[0],sizeof(UINT16) * indBuff.size(), sizeof(UINT16));

		m_pRenderable->m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_pRenderable->m_pSubMeshData = CreateSubMeshData();
		m_pRenderable->m_pSubMeshData->m_nIndexCount = indBuff.size();
		m_pRenderable->m_pSubMeshData->m_nVertexCount = vertBuff.size();

		VertexElement element[1];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		m_pRenderable->m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,1); 
	}

	void DeferredShadowPass::Init()
	{
		m_pFrustumVolume = CreateTechnique("frustumclipvolume","volume","volume","");

		m_pDefferedShadow = CreateTechnique("DeferredShadow","DefferedShadow","DefferedShadow","");
		m_pDefferedShadow->m_bDepthWrite = false;

		m_pBlendMaterial = CreateTechnique("DeferredShadowBlend","DefferedShadow","DefferedShadow","FRUSTUM_BLEND");

		m_pScreen = CreateTechnique("screen","screen","screen","");
		m_pScreen->m_eBlendMode = BM_MULTIPLY;

		CreateSimpleLightFrustumMesh();
	}

	void DeferredShadowPass::Reset()
	{
		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_R,false);

		m_pShadowSampler = CreateSamplerState(m_pShadowTex.get(),CLAMP,TFO_POINT,false);
	}

	void DeferredShadowPass::Render()
	{
		RENDER_PROFILE(DeferredShadowPass);

		RefPtr<Texture> pPreTarget = GetRenderSystem()->GetRenderTarget(0);

		FrameBuffer fb;
		fb.AttachColor(0,m_pShadowTex.get());
		fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
		GetRenderSystem()->SetFrameBuffer(&fb);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1,0);
		
		m_pFrustumVolume->Bind();

		float fBlendValue = 0.8f;
		
		UINT32 stenCillUse = 1 << SBU_DEFERREDSHADOW | 1 << (SBU_DEFERREDSHADOW + 1) | 1 << (SBU_DEFERREDSHADOW + 2);

		GetRenderSystem()->SetStencilCheckEnabled(true);
		for (int i = m_ShadowLight->GetCurSplitCount() - 1; i >= 0; --i) // 从后往前
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			// This frustum
			{
				GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 2) << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
					SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				GetRenderSystem()->SetValue( pShader->GetUniform("matFrustum"), matFrum );

				GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
			}

			// This frustum, not including blend region
			{
				GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, (i * 2 + 1)  << SBU_DEFERREDSHADOW, stenCillUse, stenCillUse,
					SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);

				ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();

				Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
				Matrix4 matBlend = Matrix4::IDENTITY;
				matBlend.setScale(Vector3(fBlendValue,fBlendValue,1.0f));
				matFrum = matFrum * matBlend;
				GetRenderSystem()->SetValue( pShader->GetUniform("matFrustum"), matFrum );

				GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
			}

		}

		for (int i = 0; i < m_ShadowLight->GetCurSplitCount(); ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			UINT32 refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			ShaderProgram* pShader = m_pDefferedShadow->GetShaderProgram();

			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisX"),shadowMapFru.m_vWBasisX);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisY"),shadowMapFru.m_vWBasisY);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisZ"),shadowMapFru.m_vWBasisZ);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoCamPos"),shadowMapFru.m_vShadowCamPos);
			GetRenderSystem()->SetValue(pShader->GetUniform("g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			GetRenderSystem()->SetValue(pShader->GetUniform("kernelRadius"),shadowMapFru.m_vkernelRadius);
			GetRenderSystem()->SetValue(pShader->GetUniform("g_tShadowMap"),shadowMapFru.GetShadowMap());

			ScreenQuad::Render(m_pDefferedShadow.get());
		}

		ShaderProgram* pBlendShader = m_pBlendMaterial->GetShaderProgram();

		// Blend
		for (int i = 0; i < m_ShadowLight->GetCurSplitCount() - 1; ++i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			ShadowMapFrustum& shadowMapNextFru = m_ShadowLight->GetShadowMapFrustum(i + 1);

			UINT32 refUse = ( i * 2  + 1 ) << SBU_DEFERREDSHADOW;
			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, refUse, stenCillUse, stenCillUse,
				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vNextStoWBasisX"), shadowMapNextFru.m_vWBasisX);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vNextStoWBasisY"), shadowMapNextFru.m_vWBasisY);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vNextStoWBasisZ"), shadowMapNextFru.m_vWBasisZ);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vNextStoCamPos"), shadowMapNextFru.m_vShadowCamPos);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("g_vNextViewPosVecLS"), shadowMapNextFru.m_viewPosVecLS);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("NextkernelRadius"),shadowMapNextFru.m_vkernelRadius);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("g_tNextShadowMap"),shadowMapNextFru.GetShadowMap());

			float fNextSize = (float)shadowMapNextFru.GetShadowMap()->GetTexture()->GetWidth();
			Vector4 vNextshadowMapTexelSize(fNextSize, 1.0f / fNextSize, 0, 0);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("g_NextshadowMapTexelSize"),vNextshadowMapTexelSize);

			Vector4 vBlendInfo = Vector4(fBlendValue,1.0f / (1.0f - fBlendValue),fBlendValue,1.0f / (1.0f - fBlendValue));
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("BlendInfo"), vBlendInfo);

			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vStoWBasisX"), shadowMapFru.m_vWBasisX);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vStoWBasisY"), shadowMapFru.m_vWBasisY);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vStoWBasisZ"), shadowMapFru.m_vWBasisZ);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("vStoCamPos"), shadowMapFru.m_vShadowCamPos);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("g_vViewPosVecLS"), shadowMapFru.m_viewPosVecLS);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("kernelRadius"),shadowMapFru.m_vkernelRadius);
			GetRenderSystem()->SetValue(pBlendShader->GetUniform("g_tShadowMap"),shadowMapFru.GetShadowMap());

			ScreenQuad::Render(m_pDefferedShadow.get());
		}

		if (!m_pScene->GetRenderScheme()->GetDeferredShadingEnabled())
		{
			FrameBuffer fb;
			fb.AttachColor(0,pPreTarget.get());
			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
			GetRenderSystem()->SetFrameBuffer(&fb);

			ShaderProgram* pShader = m_pScreen->GetShaderProgram();
 			GetRenderSystem()->SetValue(pShader->GetUniform("tSrcColor"),m_pShadowTex.get());

			ScreenQuad::Render(m_pScreen.get());	
		}
	}

	void DeferredShadowPass::Shoutdown()
	{

	}

}