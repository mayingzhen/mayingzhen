#ifndef  _IRENDER__H__
#define  _IRENDER__H__

namespace ma
{
	class IRendMesh;
}

namespace ma
{
	class IRender
	{
		virtual IRender() = 0;
		
		virtual ~IRender() = 0;
	
		virtual void RenderMesh(IRendMesh* pRendMesh) = 0;

		virtual void RenderSkelMesh(IRendMesh* pSkelMesh) = 0;

		IRendMesh* CreateRendMesh(const char* pszMeshFile);

	};

	void SetRender(IRender* pRender) = 0;

	IRender* GetRender() = 0;
}



#endif