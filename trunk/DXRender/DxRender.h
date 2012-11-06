#ifndef  _DXRENDER__H__
#define  _DXRENDER__H__

#include "Framework/IRender.h"

namespace ma
{
	class DxRender : public IRender
	{	
		DxRender();

		~DxRender();
	
		void RenderMesh(IRendMesh* pRendMesh);

		void RenderSkelMesh(IRendMesh* pSkelMesh);

		LPDIRECT3DDEVICE9 GetDXDevive() {return m_pd3dDevice;}

	private:
		bool Init(HWND hWnd);

	private:
		LPDIRECT3D9  m_pD3D;				   // The main D3D object
		D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device
	
	};
}


#endif