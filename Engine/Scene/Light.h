#ifndef  _Light__H__
#define  _Light__H__

#include "Engine/Scene/SceneNode.h"

namespace ma
{
	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
	};


	class ENGINE_API Light : public SceneNode
	{
	public:
		Light(Scene* pSene = NULL, const char* pNodeName = NULL);

		virtual	void	Update();

		LightType		GetLightType() {return m_eLightType;}

		void			SetLigtType(LightType eType) {m_eLightType = eType;}

		D3DXMATRIX		GetViewMatrix() {return m_mView;}

		D3DXMATRIX		GetProjmatrix()	{return m_mProj;}

		D3DXVECTOR3		GetDiffuse() {return m_vLightDiffuse;}

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius) {m_fRadius = fRadius;}

		D3DXVECTOR3		GetDirection() {return m_vDirection;}

		bool			IsCreateShadow() {return m_bCreateShadow;}

		void			SetCreateShadow(bool bCreateShaow) {m_bCreateShadow = bCreateShaow;}

		void			LookAt(const D3DXVECTOR3* pEye,const D3DXVECTOR3* pAt = NULL,const D3DXVECTOR3* pUp = NULL);

	protected:
		void			SyncFromSceneNode();

		void			SyncToSceneNode();

	protected:
		LightType		m_eLightType;

		D3DXVECTOR3		m_vLightDiffuse;

		D3DXVECTOR3		m_vTarget;
		D3DXVECTOR3		m_vUpVector;
		D3DXMATRIX		m_mView;
		D3DXMATRIX		m_mProj;
		

		// Point Light
		float			m_fRadius;

		// directional Ligt
		D3DXVECTOR3		m_vDirection;
		bool			m_bCreateShadow;

		// spot Light
		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;
	};

}

#endif


