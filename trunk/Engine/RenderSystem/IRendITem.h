#ifndef  _IRENDITEM__H__
#define  _IRENDITEM__H__

namespace ma
{
// 	class IRendMesh;
// 	class IRendTexture;
		
	#define FOB_IS_SKIN				(1<<1)			
	#define FOB_IS_TERRAIN			(1<<2)	
	#define FOB_NOT_CAST_SHADOW		(1<<3)
	#define FOB_NOT_RECEIVE_SHADOW  (1<<4) 

	struct ENGINE_API IRendITem 
	{
		IRendITem()
		{
			//m_pMesh = NULL;
			//m_pTex = NULL;
			//m_arrSkinMatrix = NULL;
			//m_nSkinMatrixNum = 0;
			m_ObjFlag = 0;
		}

		const D3DXMATRIX*		m_pMatWorld;
// 		const IRendMesh*		m_pMesh;
// 		const IRendTexture*		m_pTex;
// 
// 		const D3DXMATRIX*		m_arrSkinMatrix;
// 		UINT					m_nSkinMatrixNum;

		UINT m_ObjFlag;
	};
}

#endif
