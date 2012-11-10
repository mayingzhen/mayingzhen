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

		const D3DXMATRIX* GetViewMatrix() const {return &m_matView;}
		const D3DXMATRIX* GetProjMatrix() const {return &m_matProj;}

	private:
		D3DXMATRIX m_matViewProj;
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProj;

	};
}

#endif