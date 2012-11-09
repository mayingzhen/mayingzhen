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

		virtual IRendMesh* CreateRendMesh(const char* pszMeshFile) = 0;

		virtual IRendTexture* CreateRendTexture(const char* pszTextueFile) = 0;

		virtual IRendWindow* CreateRendWindow(int w, int z) = 0;
	};

	void SetRender(IRender* pRender);

	IRender* GetRender();
}



#endif