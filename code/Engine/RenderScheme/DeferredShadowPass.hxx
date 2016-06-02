#include "DeferredShadowPass.h"
#include "../Scene/Light/DirectonalLight.h"

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

		m_pScreen = CreateTechnique("screen","screen","screen","");
		m_pScreen->m_eBlendMode = BM_MULTIPLY;
		//m_pScreen->SetSceneBlending(SBF_ZERO,SBF_SOURCE_COLOUR,SBO_ADD);

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

		FrameBuffer fb;
		fb.AttachColor(0,m_pShadowTex.get());
		fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
		GetRenderSystem()->SetFrameBuffer(&fb);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1,0);
		
		m_pFrustumVolume->Bind();

		GetRenderSystem()->SetStencilCheckEnabled(true);
		for (int i = m_ShadowLight->GetCurSplitCount() - 1; i >= 0; --i) // ´ÓºóÍùÇ°
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, i + 1, 0xFFFFffff, 0xFFFFffff,
				SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);
			
			ShaderProgram* pShader = m_pFrustumVolume->GetShaderProgram();
			
			Matrix4 matFrum = m_ShadowLight->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
			GetRenderSystem()->SetValue( pShader->GetUniform("matFrustum"), matFrum );

			GetRenderSystem()->DrawRenderable(m_pRenderable.get(),m_pFrustumVolume.get());
		}

		for (int i = m_ShadowLight->GetCurSplitCount() - 1; i >= 0; --i)
		{
			ShadowMapFrustum& shadowMapFru = m_ShadowLight->GetShadowMapFrustum(i);
			if ( !shadowMapFru.GetDraw() )
				continue;

			ShaderProgram* pShader = m_pDefferedShadow->GetShaderProgram();

			GetRenderSystem()->SetStencilBufferParams(CMPF_EQUAL, i + 1, 0xFFFFffff, 0xFFFFffff,
				SOP_KEEP, SOP_KEEP, SOP_KEEP, false);

			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisX"),shadowMapFru.m_vWBasisX);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisY"),shadowMapFru.m_vWBasisY);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisZ"),shadowMapFru.m_vWBasisZ);
			GetRenderSystem()->SetValue(pShader->GetUniform("vStoCamPos"),shadowMapFru.m_vShadowCamPos);

			//pShader->SetVector4(m_paramViewPosVecLS, &shadowMapFru.m_viewPosVecLS);
			//pShader->SetVector2(m_paramgIrregkernelRadius,&shadowMapFru.m_vkernelRadius);
			GetRenderSystem()->SetValue(pShader->GetUniform("g_tShadowMap"),shadowMapFru.GetShadowMap());

			ScreenQuad::Render(m_pDefferedShadow.get());
		}

		//GetRenderSystem()->SetRenderTarget(pPreTarget);

// 		ShaderProgram* pShader = m_pScreen->GetShaderProgram();
// 		GetRenderSystem()->SetValue(pShader->GetUniform("tSrcColor"),m_pShadowTex.get());
// 
// 		ScreenQuad::Render(m_pScreen.get());		
	}

	void DeferredShadowPass::Shoutdown()
	{

	}

}