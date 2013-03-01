#ifndef  _DXRENDER__H__
#define  _DXRENDER__H__

#include "Common/IRender/IRender.h"

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

		virtual void RenderMesh(const D3DXMATRIX* pWordMat,const Mesh* pMesh);

		void RenderMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture);

		void RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,xmUint nSkinMaxtrixNum,
			const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture);

		void DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor);

		void SetViewMatrix(const D3DXMATRIX* viewMatrix);

		void SetProjMatrix(const D3DXMATRIX* projMatrix);

	private:
		void FlushLine();

		void FlushRenderQueue();

		void GBufferPass();	

		void ShadingPass();

	private:
		ID3DXEffect* m_pDefault;
		ID3DXEffect* m_pDefaultSkin;
		ID3DXEffect* m_pLineShader;
		
		// GBuffer
		ID3DXEffect* m_pGBufferTech;
		ID3DXEffect* m_pShadingTech;
		//ID3DXEffect* m_pSkinGBufferTech;
		LPDIRECT3DTEXTURE9 m_pDepthTex;
		LPDIRECT3DTEXTURE9 m_pNormalTex;

		float m_fNearClip;
		float m_fFarClip;

		IDirect3DVertexDeclaration9* m_pPrimitiveVBDesc;
	
	};
}


#endif
