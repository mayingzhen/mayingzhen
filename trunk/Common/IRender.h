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

		virtual void RenderSkelMesh(IRendMesh* pSkelMesh) = 0;

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