#ifndef  _IRENDER__H__
#define  _IRENDER__H__

namespace ma
{
	class IRendMesh;
	class IRendWindow;
	class IRendTexture;
}

namespace ma
{
	class IRender
	{
		virtual void BeginRender() = 0;

		virtual void EndRender() = 0;
	
		virtual void RenderMesh(IRendMesh* pRendMesh) = 0;

		virtual void RenderSkelMesh(IRendMesh* pSkelMesh) = 0;

		IRendMesh* CreateRendMesh(const char* pszMeshFile) = 0;

		IRendTexture* CreateRendTexture(const char* pszTextueFile) = 0;

		IRendWindow* CreateRendWindow(int w, int z) = 0;
	};

	void SetRender(IRender* pRender) = 0;

	IRender* GetRender() = 0;
}



#endif