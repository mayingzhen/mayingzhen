#ifndef  _IRENDER__H__
#define  _IRENDER__H__

namespace ma
{
	class IRendMesh;
	class IRendWindow;
	class IRendTexture;

	class COMMON_API IRender
	{
	public:	
		virtual void BeginRender() = 0;

		virtual void EndRender() = 0;

		virtual void RenderMesh(D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture) = 0;

		virtual void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,xmUint nSkinMaxtrixNum,
			D3DXMATRIX* pWordMat,IRendMesh* pSkelMesh,IRendTexture* pTexture) = 0;

		// Debg Render
		virtual void DrawBox(const D3DXMATRIX* pWordMat,const D3DXVECTOR3* boxSize, D3DCOLOR color) = 0;

		virtual void DrawWireSphere(const D3DXMATRIX* pWordMat,float fRadius, D3DCOLOR color) = 0;

// 		virtual IRendMesh* CreateRendMesh() = 0;
// 
// 		virtual IRendTexture* CreateRendTexture() = 0;

		//virtual IRendWindow* CreateRendWindow(int w, int z) = 0;

		virtual void SetViewMatrix(const D3DXMATRIX* viewMatrix) = 0;

		virtual void SetProjMatrix(const D3DXMATRIX* projMatrix) = 0;
	};

	COMMON_API void SetRender(IRender* pRender);

	COMMON_API IRender* GetRender();
}



#endif