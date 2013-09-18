#include "ShadowMapFrustum.h"

namespace ma
{
	ShadowMapFrustum::ShadowMapFrustum()
	{
	}

	ShadowMapFrustum::~ShadowMapFrustum()
	{
	}


	void ShadowMapFrustum::Init()
	{
		int iSizeX = 1024;
		int iSizeY = 1024;

		m_pShdowDepth = GetRenderDevice()->CreateRenderTarget(iSizeX,iSizeY,FMT_D24S8);

		// TexScaleBiasMat
		float fOffsetX = 0.5f + (0.5f / (float)iSizeX);
		float fOffsetY = 0.5f + (0.5f / (float)iSizeY);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		m_TexScaleBiasMat = Matrix4x4
			( 0.5f,     0.0f,     0.0f,         0.0f,
			  0.0f,    -0.5f,     0.0f,         0.0f,
			  0.0f,     0.0f,     (float)range, 0.0f,
			  fOffsetX, fOffsetY, fBias,        1.0f );

		return ;
	} 

	void ShadowMapFrustum::Update()
	{
		Frustum splitFrustum = m_pCamera->GetSplitFrustum(m_nIndex);

		Matrix4x4 mViewProj ;//= m_pLight->GetViewMatrix() * m_pLight->GetProjmatrix();

		AABB cropAABB;
		cropAABB.Merge(splitFrustum.vPts,8);
		cropAABB.Transform(mViewProj); 
		cropAABB.m_vMin.z = 0.0f; // use default near plane

		// finally, create matrix
		//return BuildCropMatrix(cropBB.vMin, cropBB.vMax);
		Vector3 vMax = cropAABB.m_vMax;
		Vector3 vMin = cropAABB.m_vMin;

		float fScaleX, fScaleY, fScaleZ;
		float fOffsetX, fOffsetY, fOffsetZ;

		fScaleX = 2.0f / (vMax.x - vMin.x);
		fScaleY = 2.0f / (vMax.y - vMin.y);

		fOffsetX = -0.5f * (vMax.x + vMin.x) * fScaleX;
		fOffsetY = -0.5f * (vMax.y + vMin.y) * fScaleY;

		fScaleZ = 1.0f / (vMax.z - vMin.z);
		fOffsetZ = -vMin.z * fScaleZ;

		// crop volume matrix
		Matrix4x4 matCrop = Matrix4x4(   fScaleX,     0.0f,     0.0f,   0.0f,
			0.0f,  fScaleY,     0.0f,   0.0f,
			0.0f,     0.0f,  fScaleZ,   0.0f,
			fOffsetX, fOffsetY, fOffsetZ,   1.0f  );

		m_viewProjCropMat = mViewProj * matCrop;
	}

	void ShadowMapFrustum::ShadowDepthPass()
	{
		GetRenderDevice()->PushRenderTarget(m_pShdowDepth);
		GetRenderDevice()->PushViewport(Rectangle(0, 0, 1024, 1024));
		
		for (UINT i = 0; i <  m_arrCasterList.size(); ++i)
		{
			Renderable* pRenderable =m_arrCasterList[i];
			if (pRenderable == NULL)
				continue;

			Material* pMaterial = pRenderable->m_pMaterial;
			pMaterial->SetCurTechnqiue("shadowDepth",NULL);
			//pMaterial->GetParameter("matWoldViewProjCrop")->setMatrix();
			GetRenderSystem()->DrawRenderable(pRenderable);
		}

		GetRenderDevice()->PopRenderTarget();
		GetRenderDevice()->PopViewport();
	}

}

