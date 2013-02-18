#ifndef  _IRENDER__H__
#define  _IRENDER__H__

#include "Common/IRender/IRendITem.h"

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

		virtual void RenderMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture) = 0;

		virtual void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,xmUint nSkinMaxtrixNum,
			const D3DXMATRIX* pWordMat,const IRendMesh* pSkelMesh,const IRendTexture* pTexture) = 0;

		virtual void DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor) = 0;

		virtual void DrawMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture);

		virtual void DrawBox(const D3DXMATRIX& wordMat,const D3DXVECTOR3& boxSize, xmUint32 color);

		virtual void DrawCircle(xmUint nbSegments, const D3DXMATRIX& world, xmUint32 dwColor, float radius, bool semicircle = false);

		virtual void DrawWireSphere(const D3DXMATRIX& wordMat,float fRadius, xmUint32 color);

		virtual void SetViewMatrix(const D3DXMATRIX* viewMatrix) = 0;

		virtual void SetProjMatrix(const D3DXMATRIX* projMatrix) = 0;

	protected:
		std::vector<IRendITem> m_vGeneral;	
		std::vector<IRendITem> m_vTransparent;
	};

	COMMON_API void SetRender(IRender* pRender);

	COMMON_API IRender* GetRender();
}



#endif