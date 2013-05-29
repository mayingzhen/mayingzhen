#ifndef  _Renderer__H__
#define  _Renderer__H__

#include "Material.h"
//#include "PrimtiveStream.h"
//#include "MWLight.h"

namespace ma
{

	struct ENGINE_API Renderable
	{
	public:
		//Renderable();
		
		//virtual                 ~Renderable();

		//virtual const AABB&		GetWorldAabb();
		
		//virtual void            GetWorldPosition(Vector3* pos);
		
		//virtual void            GetWorldTransform(Matrix4x4* form);
		
		//virtual int				GetBlendMatrix(Matrix4x4* forms);
		
		//virtual void            BeginRendering();
		
		//virtual void            EndRendering();
		
		//PRIMITIVE_TYPE			GetPrimitiveType() const;

		//void					SetMaterial(const Material * material);
		
		//Material*				GetMaterial();

	//protected:
		PRIMITIVE_TYPE			m_ePrimitiveType;
		VertexDeclaration*		m_pDeclaration;
		VertexBuffer*			m_pVertexBuffers;
		IndexBuffer*			m_pIndexBuffer;
		UINT					m_nVertexStart;
		UINT					m_nVertexCount;
		UINT					m_nIndexStart;
		UINT					m_nIndexCount;

		Material*               m_pMaterial;

		//const Matrix4x4*		m_arrSkinMatrix;
		//UINT					m_nSkinMatrixNum;
	};

// 	struct RenderOp
// 	{
// 		Matrix4x4 xform;
// 		RenderState rState;
// 		VertexStream vxStream;
// 		IndexStream ixStream;
// 		int iPrimCount;
// 		PRIMITIVE_TYPE ePrimType;
// 
// 		RenderOp()
// 		{
// 			//xform =  Mat4::Identity;
// 			MatrixIdentity(&xform);
// 			iPrimCount = 0;
// 			ePrimType = PRIM_TRIANGLELIST;
// 		}
// 	};

}

#endif