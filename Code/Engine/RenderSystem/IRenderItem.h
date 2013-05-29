#ifndef  _IRENDITEM__H__
#define  _IRENDITEM__H__

namespace ma
{
		
	#define FOB_IS_SKIN				(1<<1)			
	#define FOB_IS_TERRAIN			(1<<2)	
	#define FOB_NOT_CAST_SHADOW		(1<<3)
	#define FOB_NOT_RECEIVE_SHADOW  (1<<4) 

	class ENGINE_API IRenderItem 
	{
// 		IRenderItem()
// 		{
// 			m_pMesh = NULL;
// 			m_pTex = NULL;
// 			m_arrSkinMatrix = NULL;
// 			m_nSkinMatrixNum = 0;
// 			m_ObjFlag = 0;
// 		}

		//virtual void Draw() = 0;

		//UINT m_ObjFlag;
	};
}

#endif
