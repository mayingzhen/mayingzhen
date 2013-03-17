#ifndef  _CAMERA__H__
#define  _CAMERA__H__

#include "Engine/Scene/SceneNode.h"

namespace ma
{
	class ENGINE_API Camera : public SceneNode
	{
	public:
		enum {NUM_PSSM = 4};

		Camera();
		
		~Camera();

		virtual void		Update();			

		void				LookAt(const D3DXVECTOR3* pEye,const D3DXVECTOR3* pAt = NULL,const D3DXVECTOR3* pUp = NULL);

		void				SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		const D3DXMATRIX&	GetViewMatrix() const {return m_matView;}

		const D3DXMATRIX&	GetProjMatrix() const {return m_matProj;}
		
		D3DXMATRIX			GetViewProjMatrix() const {return m_matView * m_matProj;}
		
		float				GetNearClip() {return m_fNear;}

		float				GetFarClip() {return m_fFar;}
		
		Frustum				GetSplitFrustum(int index) {return m_arrSplitFrustum[index];}

		float*				GetSplitPos() {return m_fSplitPos;}

	protected:
		void				CalculateSplitPositions();

		void				CalculateFrustum();

		Frustum				CalculateFrustum(float fNear, float fFar);

	private:
		D3DXMATRIX		m_matView;
		D3DXMATRIX		m_matProj;

		float			m_fAspect;
		float			m_fFOV;	
		float			m_fNear;
		float			m_fFar;

		D3DXVECTOR3		m_vLookAtPt;
		D3DXVECTOR3		m_vEyePt;
		D3DXVECTOR3		m_vUpVector;

		//bool			m_bProjDirty;

		//bool			m_bOrtho;
		float			m_fHeight;

		float m_fSplitPos[NUM_PSSM + 1];

		Frustum m_arrSplitFrustum[NUM_PSSM];

		Frustum	m_frustum;
	};
}

#endif