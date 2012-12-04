#ifndef  _DXRENDER__H__
#define  _DXRENDER__H__

#include "Common/IRender.h"

namespace ma
{
	struct RenderObj
	{
		D3DXMATRIX m_matWorld;
		//IRe

	};
}



namespace ma
{
	class DXRENDER_API DxRender : public IRender
	{	
	public:

		DxRender();

		~DxRender();

		void InitDefaultShader();
	
		void BeginRender();

		void EndRender();

		void RenderMesh(D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture);

		void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,xmUint nSkinMaxtrixNum,
			D3DXMATRIX* pWordMat,IRendMesh* pSkelMesh,IRendTexture* pTexture);

		//IRendMesh* CreateRendMesh();

		//IRendTexture* CreateRendTexture();

		//IRendWindow* CreateRendWindow(int w, int z);

		void SetViewMatrix(const D3DXMATRIX* viewMatrix);

		void SetProjMatrix(const D3DXMATRIX* projMatrix);


	private:
		ID3DXEffect* m_pDefault;
		ID3DXEffect* m_pDefaultSkin;
		
	
	};
}


#endif