#include "ShadowMapFrustum.h"

namespace ma
{
	ShadowMapFrustum::ShadowMapFrustum(Light* pLight,int nIndex)
	{
		m_pLight = pLight;
		m_nIndex = nIndex;

		MatrixIdentity(&m_matView[0]);
		MatrixIdentity(&m_matView[1]);
		MatrixIdentity(&m_matPoj[0]);
		MatrixIdentity(&m_matPoj[1]);

		
		float fShadowMapSize = GetShadowSystem()->GetShadowMapSize();

		if ( GetShadowSystem()->IsHWShadowMap() )
		{
			m_pShdowDepth = GetRenderSystem()->CreateRenderTarget(fShadowMapSize,fShadowMapSize,FMT_D24S8);
			m_pDepthStencil = NULL;
		}
		else
		{
			m_pShdowDepth = GetRenderSystem()->CreateRenderTarget(fShadowMapSize,fShadowMapSize,FMT_R32F);
			m_pDepthStencil = GetRenderSystem()->CreateRenderTarget(fShadowMapSize,fShadowMapSize,FMT_D24S8,true);
		}

		// TexScaleBiasMat
		float fOffsetX = 0.5f + (0.5f / (float)fShadowMapSize);
		float fOffsetY = 0.5f + (0.5f / (float)fShadowMapSize);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		m_TexScaleBiasMat = Matrix4x4
			( 0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f );
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{

	}

	void ShadowMapFrustum::SetViewMatrix(const Matrix4x4& matView) 
	{
		int index = GetRenderThread()->GetThreadList(); 
		m_matView[index] = matView;
	}

	void ShadowMapFrustum::SetProjMatrix(const Matrix4x4& matPoj) 
	{
		int index = GetRenderThread()->GetThreadList(); 
		m_matPoj[index] = matPoj;
	}

	const Matrix4x4& ShadowMapFrustum::GetViewMarix()  
	{
		int index = GetRenderThread()->GetThreadList(); 
		return m_matView[index];
	}

	const Matrix4x4& ShadowMapFrustum::GetProjMatrix() 
	{
		int index = GetRenderThread()->GetThreadList(); 
		return m_matPoj[index];
	}

	void ShadowMapFrustum::ClearCasterList() 
	{
		int index = GetRenderThread()->GetThreadList(); 
		m_arrCasterList[index].clear();
	}

	void ShadowMapFrustum::AddCaster(RenderObject* pRenderObj) 
	{
		int index = GetRenderThread()->GetThreadList(); 
		m_arrCasterList[index].push_back(pRenderObj);
	}


	void ShadowMapFrustum::ShadowDepthPass()
	{
		GetRenderSystem()->PushViewPort(Rectangle(0, 0, 1024, 1024));

		if ( GetShadowSystem()->IsHWShadowMap() )
		{
			GetRenderSystem()->PushDepthStencil(m_pShdowDepth);
		}
		else
		{
			GetRenderSystem()->PushRenderTarget(m_pShdowDepth);
			GetRenderSystem()->PushDepthStencil(m_pDepthStencil);
		}

		GetRenderSystem()->ClearBuffer(true,true,true,Color(1,1,1,0), 1.0f, 0);

		int index = GetRenderThread()->m_nCurThreadProcess;
		GetMaterialManager()->SetCurViewPojectMat(m_matView[index], m_matPoj[index]);
		
		for (UINT i = 0; i <  m_arrCasterList[index].size(); ++i)
		{
			RenderObject* pRenderObj = m_arrCasterList[index][i];
			if (pRenderObj == NULL)
				continue;

			if ( SafeCast<TerrainSection>(pRenderObj) )
				continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("ShadowDepth");

			pRenderObj->Render(pTech);
		}

		if ( GetShadowSystem()->IsHWShadowMap() )
		{
			GetRenderSystem()->PopDepthStencil();
		}
		else
		{	
			GetRenderSystem()->PopDepthStencil();
			GetRenderSystem()->PopRenderTargert();
		}

		GetRenderSystem()->PopViewPort();
	}

}

