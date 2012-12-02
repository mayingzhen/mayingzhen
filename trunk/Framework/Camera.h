#ifndef  _CAMERA__H__
#define  _CAMERA__H__

#include "Framework/SceneNode.h"

namespace ma
{
	class FRAMEWORK_API Camera : public SceneNode
	{
	public:
		Camera();
		~Camera();

		void LookAt(const xmVector3* pEye,const xmVector3* pAt,const xmVector3* pUp);

		void SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		const D3DXMATRIX* GetViewMatrix() const {return &m_matView;}
		const D3DXMATRIX* GetProjMatrix() const {return &m_matProj;}

	private:
		D3DXMATRIX		m_matViewProj;
		D3DXMATRIX		m_matView;
		D3DXMATRIX		m_matProj;

		float			m_fAspect;
		float			m_fFOV;	
		float			m_fNear;
		float			m_fFar;
		//bool			m_bProjDirty;

		//bool			m_bOrtho;
		float			m_fHeight;


	};
}

#endif