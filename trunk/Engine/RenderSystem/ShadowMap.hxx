#include "ShadowMap.h"

namespace ma
{
	ShadowMap::ShadowMap()
	{
		m_pLight = NULL; 
		m_pCamera = NULL;
	}

	ShadowMap::~ShadowMap()
	{

	}

	bool IsFrustumCull(const AABB& aabb, const Matrix4x4& matViewProj)
	{
		AABB viewProjAABB = aabb.Transform(matViewProj);
		if (viewProjAABB.m_vMax.x < -1 || viewProjAABB.m_vMin.x > +1 ||
			viewProjAABB.m_vMax.y < -1 || viewProjAABB.m_vMin.y > +1 ||
			viewProjAABB.m_vMax.z < 0 || viewProjAABB.m_vMin.z > +1) // gl -1 ~ 1;
		{
			return true;
		}

		return false;
	}

	void ShadowMap::FindCaster(IRenderItem* arrRenderItem,int nNumber)
	{
		m_arrCasterList.clear();

		for (UINT i = 0; i < nNumber; ++i)
		{
			IRenderItem* pRenderItem = arrRenderItem[i];
			if (pRenderItem == NULL)
				continue;

			
			if ( IsFrustumCull(pRenderItem->GetAABB(), m_frustum.m_matViewProj) )
			{
				continue;
			}

			m_arrCasterList.push_back(pRenderItem);
		}
	}

	void ShadowMap::CalculateMatrix(Camera* pCamera,int nSpitIndex,Light* pLight)
	{
		if (pCamera == NULL || pLight == NULL)
			return;


	}

	bool ShadowMap::Create(int iSizeX,int iSizeY)
	{
		HRESULT hr;

		// TexScaleBiasMat
		float fOffsetX = 0.5f + (0.5f / (float)iSizeX);
		float fOffsetY = 0.5f + (0.5f / (float)iSizeY);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		m_TexScaleBiasMat = Matrix4x4( 0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f );

		return true;
	}
}

