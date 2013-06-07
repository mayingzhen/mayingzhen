#include "RenderScheme.h"

namespace ma
{
	void GBufferPass::DoRender()
	{
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		//D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"GBufferPass");


		GetRenderDevice()->SetRenderTarget(0,m_pDepthTex);
		GetRenderDevice()->SetRenderTarget(1,m_pNormalTex);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0, 0, 0, 0), 1.0f, 0);


		float fNearClip = m_pMainCamera->GetNearClip();
		float fFarClip = m_pMainCamera->GetFarClip();
		Vector4 depth_near_far_invfar = Vector4(fNearClip, fFarClip, 1 / fFarClip, 0);

		for(UINT i = 0; i < uNumber; ++i)
		{
			IRenderItem* pRenderItem = m_SolidEntry[i];
			if (pRenderItem == NULL)
				continue;

			//D3D9RendMesh* pD3D9RenderMesh = (D3D9RendMesh*)pRenderItem->m_pMesh; 

			Technique* pCurTech = m_pGBufferTech;
			if (pRenderItem->m_nSkinMatrixNum != 0)
			{
				pCurTech = m_pGBufferSkinTech;
				pCurTech->SetMatrixArray("mSkinMatrixArray",pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
			}


			Matrix4x4 matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
			Matrix4x4 matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
			pCurTech->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
			pCurTech->SetMatrix("worldview",(const D3DXMATRIX*)&matWV);
			pCurTech->SetVector("depth_near_far_invfar",(const D3DXVECTOR4*)&depth_near_far_invfar);

			pCurTech->Begin();

			//pRenderItem->Draw();
			GetRenderDevice()->DrawMesh(RenderT)

			pCurTech->End();
		}

// 		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT0);
// 		SAFE_RELEASE(pOldRT0);
// 		hr = m_pD3DDevice->SetRenderTarget(1, pOldRT1);
// 		SAFE_RELEASE(pOldRT1);

		//D3DPERF_EndEvent();
	}
}