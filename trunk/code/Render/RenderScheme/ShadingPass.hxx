#include "ShadingPass.h"
#include "../RenderSystem/RenderSetting.h"
#include "HDRPostProcess.h"

namespace ma
{
	void ShadingPass::Init()
	{
		m_pShadingTex = NULL;

		if ( GetRenderSetting()->m_bIsHDRRending )
		{
			m_pShadingTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A16B16G16R16F);	

			m_pHdrPostprocess = new HDRPostProcess(m_pShadingTex,NULL);
			m_pHdrPostprocess->Init();
		}
	}

	void ShadingPass::RenderObjecList()
	{
		RENDER_PROFILE(RenderObjecList);

		Color cClearClor = GetRenderSetting()->m_cClearClor;

		GetRenderSystem()->ClearBuffer(true,true,true,cClearClor, 1.0f, 0);

		RenderQueue* pRenderQueue = GetRenderSystem()->GetRenderQueue();

		UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			RenderObject* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			if (pRenderObj == NULL)
				continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("Shading");

			pRenderObj->Render(pTech);
		}

		UINT nTrans = pRenderQueue->GetRenderObjNumber(RL_Trans);
		for (UINT i = 0; i < nTrans; ++i)
		{
			RenderObject* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Trans,i);
			if (pRenderObj == NULL)
				continue;

			Material* pMaterial = pRenderObj->GetMaterial();

			Technique* pTech = pMaterial->GetTechnqiue("Shading");

			pRenderObj->Render(pTech);
		}
	}

	void ShadingPass::Render()
	{
		RENDER_PROFILE(ShadingPass);

		if ( GetRenderSetting()->m_bIsHDRRending )
		{
			GetRenderSystem()->PushRenderTarget(m_pShadingTex);
		}

		RenderObjecList();

		if ( GetRenderSetting()->m_bIsHDRRending )
		{
			GetRenderSystem()->PopRenderTargert();

			m_pHdrPostprocess->Render();
		}
	}

	void ShadingPass::ShoutDown()
	{

	}

}

