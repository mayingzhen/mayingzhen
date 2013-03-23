#include "Light.h"

namespace ma
{
	Light::Light(Scene* pSene, const char* pNodeName):
	SceneNode(pSene,pNodeName)
	{
		m_eLightType = LIGHT_POINT;
		m_vUpVector = D3DXVECTOR3(0, 1, 0);
		m_fAspectRatio = 1.0f;
		m_vTarget = D3DXVECTOR3(0, 0, 0);
		m_fNearClip = 0.1f;
		m_fFarClip = 3000.0f;
		m_fRadius = 1.0f;
		m_fFOV =  D3DXToRadian(45.0f);
		m_vLightDiffuse = D3DXVECTOR3(0.9f,0.9f,0.9f);
		m_bCreateShadow = false;
	}

	void Light::Update()
	{
		SyncFromSceneNode();
	}

	void Light::SyncFromSceneNode()
	{
		D3DXMatrixInverse(&m_mView,NULL,&GetWorldMatrix());
	}

	void Light::SyncToSceneNode()
	{
		maNodeTransform tsfWS;
		D3DXMATRIX matViewInv;
		D3DXMatrixInverse(&matViewInv,NULL,&m_mView);
		maTransformFromMatrix(&tsfWS,matViewInv);
		SetTransformWS(tsfWS);
	}

	void Light::LookAt(const D3DXVECTOR3* pEye,const D3DXVECTOR3* pAt,const D3DXVECTOR3* pUp)
	{
		if (pAt)
		{
			m_vTarget = *pAt;
		}
		if (pUp)
		{
			m_vUpVector = *pUp;
		}

		// view matrix
		D3DXMatrixLookAtLH(&m_mView, pEye, &m_vTarget, &m_vUpVector);
		
		SyncToSceneNode();


		// projection matrix
		if(m_eLightType == LIGHT_SPOT)
		{
			D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
		}
		else if (m_eLightType == LIGHT_DIRECTIONAL)
		{
			D3DXMatrixOrthoLH(&m_mProj, 1, 1, 0, 1);
		}
	}
}


