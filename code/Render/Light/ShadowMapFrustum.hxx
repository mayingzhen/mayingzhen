#include "ShadowMapFrustum.h"
#include "../Terrain/TerrainSection.h"

namespace ma
{
	ShadowMapFrustum::ShadowMapFrustum(Light* pLight,int nIndex)
	{
		m_pLight = pLight;
		m_nIndex = nIndex;
	
		int nShadowMapSize = (int)GetRenderSetting()->m_fShadowMapSize;

		if ( GetRenderSetting()->m_bHWShadowMap )
		{
			m_pShdowDepth = GetRenderSystem()->CreateRenderTarget(nShadowMapSize,nShadowMapSize,FMT_R32F);
			m_pDepthStencil = GetRenderSystem()->CreateRenderTarget(nShadowMapSize,nShadowMapSize,FMT_D24S8);
		}
		else
		{
			m_pShdowDepth = GetRenderSystem()->CreateRenderTarget(nShadowMapSize,nShadowMapSize,FMT_R32F);
			m_pDepthStencil = GetRenderSystem()->CreateRenderTarget(nShadowMapSize,nShadowMapSize,FMT_D24S8,true);
		}

		// TexScaleBiasMat
		float fOffsetX = 0.5f + (0.5f / (float)nShadowMapSize);
		float fOffsetY = 0.5f + (0.5f / (float)nShadowMapSize);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		m_TexScaleBiasMat = Matrix4x4
			( 0.5f,     0.0f,     0.0f,         0.0f,
			 0.0f,     -0.5f,     0.0f,         0.0f,
			 0.0f,      0.0f,     (float)range, 0.0f,
			 fOffsetX,  fOffsetY, fBias,        1.0f );
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{

	}

	void ShadowMapFrustum::SetViewMatrix(const Matrix4x4& matView) 
	{
		int index = GetRenderSystem()->CurThreadFill(); 
		m_matViewProj[index].SetMatView(matView);
	}

	void ShadowMapFrustum::SetProjMatrix(const Matrix4x4& matPoj) 
	{
		int index = GetRenderSystem()->CurThreadFill(); 
		m_matViewProj[index].SetMatProj(matPoj);
	}

	const Matrix4x4& ShadowMapFrustum::GetViewMarix()  
	{
		int index = GetRenderSystem()->GetThreadList(); 
		return m_matViewProj[index].GetMatView();
	}

	const Matrix4x4& ShadowMapFrustum::GetProjMatrix() 
	{
		int index = GetRenderSystem()->GetThreadList(); 
		return m_matViewProj[index].GetMatProj();
	}

	const Matrix4x4& ShadowMapFrustum::GetViewProjMatrix()
	{
		int index = GetRenderSystem()->GetThreadList(); 
		return m_matViewProj[index].GetMatViewProj();
	}

	void ShadowMapFrustum::ClearCasterList() 
	{
		int index = GetRenderSystem()->GetThreadList(); 
		m_arrCasterList[index].clear();
	}

	void ShadowMapFrustum::AddCaster(RenderObject* pRenderObj) 
	{
		int index = GetRenderSystem()->GetThreadList(); 
		m_arrCasterList[index].push_back(pRenderObj);
	}

	Texture* ShadowMapFrustum::GetDepthTexture()
	{
		if ( GetRenderSetting()->m_bHWShadowMap )
		{
			return m_pDepthStencil;
		}
		else
		{
			return m_pShdowDepth;
		}
	}

	void ShadowMapFrustum::RenderShadowMap()
	{
		float fShadowMapSize = GetRenderSetting()->m_fShadowMapSize;

		Rectangle viewPort(0, 0, fShadowMapSize, fShadowMapSize);

		Rectangle preViewPort =	GetRenderSystem()->SetViewPort(viewPort);
		Texture* pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShdowDepth);
		Texture* pPreDeptStencil = GetRenderSystem()->SetDepthStencil(m_pDepthStencil);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(1,1,1,0), 1.0f, 0);

		int index = GetRenderSystem()->CurThreadProcess();
		GetRenderContext()->SetViewMatrix(m_matViewProj[index].GetMatView());
		GetRenderContext()->SetProjMatrix(m_matViewProj[index].GetMatProj());
		
		for (UINT i = 0; i <  m_arrCasterList[index].size(); ++i)
		{
			RenderObject* pRenderObj = m_arrCasterList[index][i];
			if (pRenderObj == NULL)
				continue;

			//if ( SafeCast<TerrainSection>(pRenderObj) )
			//	continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("ShadowDepth");

			pRenderObj->Render(pTech);
		}


		GetRenderSystem()->SetDepthStencil(pPreDeptStencil);
		GetRenderSystem()->SetRenderTarget(pPreTarget);
		GetRenderSystem()->SetViewPort(preViewPort);
	}

}

