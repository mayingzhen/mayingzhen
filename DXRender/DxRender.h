#ifndef  _DXRENDER__H__
#define  _DXRENDER__H__

#include "Common/IRender.h"

namespace ma
{
	class DXRENDER_API DxRender : public IRender
	{	
	public:

		DxRender();

		~DxRender();
	
		void BeginRender();

		void EndRender();

		void RenderMesh(D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture);

		void RenderSkelMesh(IRendMesh* pSkelMesh);

		IRendMesh* CreateRendMesh();

		IRendTexture* CreateRendTexture();

		//IRendWindow* CreateRendWindow(int w, int z);

		void SetViewMatrix(const D3DXMATRIX* viewMatrix);

		void SetProjMatrix(const D3DXMATRIX* projMatrix);

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