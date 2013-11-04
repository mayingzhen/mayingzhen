#include "ShadingPass.h"
#include "../RenderSystem/RenderSetting.h"

namespace ma
{
	void ShadingPass::Init()
	{

	}

	void ShadingPass::Render()
	{
		RENDER_PROFILE(ShadingPass);

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

	void ShadingPass::ShoutDown()
	{

	}

}

