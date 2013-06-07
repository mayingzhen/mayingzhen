#include "ShadowDepthPass.h"

namespace ma
{
	ShadowDepthPass::Render()
	{
		if (m_pMainLigt == NULL || m_pMainCamera == NULL)
			return;

		HRESULT hr = D3D_OK;

		//D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadowMapPass");

		//float fDepthBias = 0.002f;
		//m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);

		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		for (UINT i = 0; i < 4; ++i)
		{
			//D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadowMapPass");

			ShadowMap* pShadowMap = m_arrShadowMap[i];
			if(pShadowMap == NULL)
			{
				m_arrShadowMap[i] = new ShadowMap();
				pShadowMap = m_arrShadowMap[i];
				pShadowMap->Init();
			}

			pShadowMap->CalculateMatrix(m_pMainCamera,i,m_pMainLigt);

			pShadowMap->ClearCaster();
			if (!m_SolidEntry.empty())
				pShadowMap->FindCaster((IRenderItem**)&m_SolidEntry[0],m_SolidEntry.size());
			if (!m_TransEntry.empty())
				pShadowMap->FindCaster((IRenderItem**)&m_TransEntry[0],m_TransEntry.size());

			pShadowMap->BeginRender();

			m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00FFFFFF, 1.0f, 0L);

			UINT nCaster = pShadowMap->GetCasterNumber();
			for(UINT j = 0; j < nCaster; ++j)
			{
				IRenderItem* pRenderItem = pShadowMap->GetCasterByIndex(j);
				if (pRenderItem == NULL)
					continue;

				Technique* pTechnqiue = NULL;

				if (pRenderItem->m_nSkinMatrixNum != 0)
				{
					pTechnqiue = m_pSkinShadowMapTech;
					pTechnqiue->SetMatrixArray("mSkinMatrixArray",(const D3DXMATRIX*)pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
				}
				else
				{
					pTechnqiue = m_pShdowMapTech;
				}

				Matrix4x4 matWVP = *(pRenderItem->m_pMatWorld) * pShadowMap->GetViewMatrix() * pShadowMap->GetProjMatrix();
				Matrix4x4 matWV = *(pRenderItem->m_pMatWorld) * pShadowMap->GetViewMatrix();
				pTechnqiue->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
				pTechnqiue->SetMatrix("worldview",(const D3DXMATRIX*)&matWV);

				pTechnqiue->Begin();

				//pRenderItem->Render();
				GetRenderDevice()->Render(pTechnqiue,pRenderItem);

				pTechnqiue->End();


// 				UINT cPasses = 0; 
// 				hr = pCurEffect->Begin(&cPasses, 0 );
// 				for (UINT i = 0; i < cPasses; ++i)
// 				{
// 					hr = pCurEffect->BeginPass(i);
// 					hr = pCurEffect->CommitChanges();
// 					//pD3D9RenderMesh->GetD3DXMesh()->DrawSubset(0);
// 					pCurEffect->EndPass();
// 				}	
// 				pCurEffect->End();
			}

			pShadowMap->EndRender();	
		}

		//m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);

		//D3DPERF_EndEvent();
	}

}