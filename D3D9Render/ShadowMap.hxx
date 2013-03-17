#include "ShadowMap.h"

namespace ma
{
	ShadowMap::ShadowMap()
	{
		m_pTexture = NULL;
		m_pDSTexture = NULL;
		m_pSurface = NULL;
		m_pDSSurface = NULL;
		m_pOldDSSurface = NULL;
		m_pOldRenderTarget = NULL;
	}

	ShadowMap::~ShadowMap()
	{
		Destroy();
	}

	bool IsFrustumCull(const AABB& aabb, const D3DXMATRIX& matViewProj)
	{
		AABB viewProjAABB;
		viewProjAABB.Merge(aabb);
		viewProjAABB.Transform(matViewProj);
		if (viewProjAABB.m_vMax.x < -1 || viewProjAABB.m_vMin.x > +1 ||
			viewProjAABB.m_vMax.y < -1 || viewProjAABB.m_vMin.y > +1 ||
			viewProjAABB.m_vMax.z < 0 || viewProjAABB.m_vMin.z > +1) // gl -1 ~ 1;
		{
			return true;
		}

		return false;
	}

	void ShadowMap::FindCaster(IRenderItem** arrRenderItem,int nNumber)
	{
		for (UINT i = 0; i < nNumber; ++i)
		{
			IRenderItem* pRenderItem = arrRenderItem[i];
			if (pRenderItem == NULL)
				continue;

			Frustum splitFrustum = m_pCamera->GetSplitFrustum(m_nIndex);	
// 			if ( IsFrustumCull(pRenderItem->GetAABB(), splitFrustum.m_matViewProj) )
// 			{
// 				continue;
// 			}

			m_arrCasterList.push_back(pRenderItem);
		}
	}


	void ShadowMap::Init(LPDIRECT3DDEVICE9 pDxDevice,int iSizeX,int iSizeY)
	{
		m_pDxDevice = pDxDevice;

		HRESULT hr;

		// Create a renderable texture
		hr = m_pDxDevice->CreateTexture( iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
			D3DPOOL_DEFAULT, &m_pTexture, NULL );
		assert( SUCCEEDED(hr) );

		hr = m_pTexture->GetSurfaceLevel(0, &m_pSurface);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("GetSurfaceLevel failed!"), TEXT("Error!"), MB_OK);
			return ;
		}

#ifdef PIX_DEBUG
		hr = m_pDxDevice->CreateDepthStencilSurface(iSizeX, iSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 
			0, TRUE, &m_pDSSurface, NULL);
#else
		hr = m_pDxDevice->CreateTexture( iSizeX, iSizeY, 1, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D24S8, 
			D3DPOOL_DEFAULT, &m_pDSTexture, NULL);
		assert( SUCCEEDED(hr) );

		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Creating render texture failed!"), TEXT("Error!"), MB_OK);
			return ;
		}

		hr = m_pDSTexture->GetSurfaceLevel(0, &m_pDSSurface);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("GetSurfaceLevel failed!"), TEXT("Error!"), MB_OK);
			return ;
		}
#endif

		// Create a new viewport
		m_Viewport.X = 0;
		m_Viewport.Y = 0;
		m_Viewport.Width  = iSizeX;
		m_Viewport.Height = iSizeY;
		m_Viewport.MinZ = 0.0f;
		m_Viewport.MaxZ = 1.0f;

		m_nSizeX = iSizeX;
		m_nSizeY = iSizeY;

		// TexScaleBiasMat
		float fOffsetX = 0.5f + (0.5f / (float)iSizeX);
		float fOffsetY = 0.5f + (0.5f / (float)iSizeY);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		m_TexScaleBiasMat = D3DXMATRIX( 0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f );

		return ;
	}

	D3DXMATRIX ShadowMap::CalculateCropMatrix()
	{
		Frustum splitFrustum = m_pCamera->GetSplitFrustum(m_nIndex);

		D3DXMATRIX mViewProj = m_pLight->GetViewMatrix() * m_pLight->GetProjmatrix();

		AABB cropAABB;
		cropAABB.Merge(splitFrustum.vPts,8);
		cropAABB.Transform(mViewProj); 
		cropAABB.m_vMin.z = 0.0f; // use default near plane

		// finally, create matrix
		//return BuildCropMatrix(cropBB.vMin, cropBB.vMax);
		D3DXVECTOR3 vMax = cropAABB.m_vMax;
		D3DXVECTOR3 vMin = cropAABB.m_vMin;

		float fScaleX, fScaleY, fScaleZ;
		float fOffsetX, fOffsetY, fOffsetZ;

		fScaleX = 2.0f / (vMax.x - vMin.x);
		fScaleY = 2.0f / (vMax.y - vMin.y);

		fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
		fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

		fScaleZ = 1.0f / (vMax.z - vMin.z);
		fOffsetZ = -vMin.z * fScaleZ;

		// crop volume matrix
		return D3DXMATRIX(   fScaleX,     0.0f,     0.0f,   0.0f,
			0.0f,  fScaleY,     0.0f,   0.0f,
			0.0f,     0.0f,  fScaleZ,   0.0f,
			fOffsetX, fOffsetY, fOffsetZ,   1.0f  );
	}

	void ShadowMap::CalculateMatrix(Camera* pCamera,int nSpitIndex,Light* pLight)
	{
		if (pCamera == NULL || pLight == NULL)
			return;

		m_pLight = pLight;
		m_pCamera = pCamera;
		m_nIndex = nSpitIndex;
		Frustum splitFrustum = pCamera->GetSplitFrustum(nSpitIndex);

		m_viewMat = pLight->GetViewMatrix();
		m_projMat = pLight->GetProjmatrix() * CalculateCropMatrix();
		m_TexMat = m_viewMat * m_projMat * m_TexScaleBiasMat;
	}

	void ShadowMap::Destroy(void)
	{
		SAFE_RELEASE(m_pDSSurface);
		SAFE_RELEASE(m_pSurface);
		SAFE_RELEASE(m_pTexture);
		SAFE_RELEASE(m_pDSTexture);

		SAFE_RELEASE(m_pOldDSSurface);
		SAFE_RELEASE(m_pOldRenderTarget);
	}

	void ShadowMap::BeginRender() 
	{
		// Store original values
		m_pDxDevice->GetViewport(&m_OldViewport);
		m_pDxDevice->GetRenderTarget(0, &m_pOldRenderTarget);
		m_pDxDevice->GetDepthStencilSurface(&m_pOldDSSurface);

		// Set new values
		m_pDxDevice->SetViewport(&m_Viewport);
		m_pDxDevice->SetRenderTarget(0, m_pSurface);
		m_pDxDevice->SetDepthStencilSurface(m_pDSSurface);
	}

	void ShadowMap::EndRender() 
	{
		m_pDxDevice->SetDepthStencilSurface(m_pOldDSSurface);
		SAFE_RELEASE(m_pOldDSSurface)

			m_pDxDevice->SetRenderTarget(0, m_pOldRenderTarget);
		SAFE_RELEASE(m_pOldRenderTarget);

		m_pDxDevice->SetViewport(&m_OldViewport);
	}


}

