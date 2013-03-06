#ifndef  _IRENDER__H__
#define  _IRENDER__H__

#include "IRenderItem.h"

namespace ma
{
	class IRendMesh;
	class IRendWindow;
	class IRendTexture;

	class ENGINE_API IRender
	{
	public:	
		virtual void BeginRender();

		virtual void EndRender();

		virtual void RenderMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture) = 0;

		virtual void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,UINT nSkinMaxtrixNum,
			const D3DXMATRIX* pWordMat,const IRendMesh* pSkelMesh,const IRendTexture* pTexture) = 0;

		virtual void DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor) = 0;

		virtual void DrawMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture);

		virtual void DrawBox(const D3DXMATRIX& wordMat,const D3DXVECTOR3& boxSize, xmUint32 color);

		virtual void DrawCircle(UINT nbSegments, const D3DXMATRIX& world, xmUint32 dwColor, float radius, bool semicircle = false);

		virtual void DrawWireSphere(const D3DXMATRIX& wordMat,float fRadius, xmUint32 color);

		virtual void SetViewMatrix(const D3DXMATRIX* viewMatrix) = 0;

		virtual void SetProjMatrix(const D3DXMATRIX* projMatrix) = 0;

	protected:
		//std::vector<IRenderItem> m_vGeneral;	
		//std::vector<IRenderItem> m_vTransparent;
	};

	ENGINE_API void SetRender(IRender* pRender);

	ENGINE_API IRender* GetRender();
}



#endif