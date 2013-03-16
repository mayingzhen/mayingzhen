#include "ShadowMapRender.h"

namespace ma
{
	ShdowMapRender::ShdowMapRender()
	{

	}

	void ShdowMapRender::CalculateShadowMap(Light* pLigt,Camera* pCamera)
	{
		for (int i = 0; i < MAX_SHDOWMAP; ++i)
		{
			Frustum splitFrustum;
			splitFrustum = pCamera.CalculateFrustum(pCamera.m_fSplitPos[i], pCamera.m_fSplitPos[i + 1]);
			//std::list<CObject*> ObjList = FindCasters(splitFrustum); 

			if ( ObjList.empty() ) 
			{
				m_arrShadowMap[i] = NULL;
				continue;
			}
			else
			{
				ShadowMap* pShadowMap = CShadowMapPool::GetOneShdowMap();
				lightIt->m_pShadowMap[i] = pShadowMap;	
				m_allShdowMap.push_back(pShadowMap);

				D3DXMATRIX mCropMatrix = lightIt->CalculateCropMatrix(splitFrustum);
				pShadowMap->m_nIndex = i;
				pShadowMap->m_viewMat = lightIt->m_mView;
				pShadowMap->m_projMat = lightIt->m_mProj * mCropMatrix;
				pShadowMap->m_TexMat = pShadowMap->m_viewMat * pShadowMap->m_projMat * *( pShadowMap->GetTexScaleBiasMat() );
				pShadowMap->m_casterList.clear();
				pShadowMap->m_casterList = ObjList;
			}			
		}
	}
}
