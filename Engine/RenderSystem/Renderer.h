#ifndef  _Renderer__H__
#define  _Renderer__H__

#include "Material.h"
//#include "PrimtiveStream.h"
//#include "MWLight.h"

namespace ma
{

	class ENGINE_API Renderer
	{
	public:
								Renderer();
		virtual                 ~Renderer();

		virtual const AABB&		GetWorldAabb();
		virtual void            GetWorldPosition(Vector3* pos);
		virtual void            GetWorldTransform(Matrix4x4* form);
		virtual int				GetBlendMatrix(Matrix4x4* forms);
		virtual void            BeginRendering();
		virtual void            EndRendering();

				void            SetPrimitiveCount(int size);
				void            SetPrimitiveType(PRIMITIVE_TYPE type);

		virtual VertexStream *  GetVertexStream();
		//virtual IndexStream *   GetIndexStream();
				int             GetPrimitiveCount() const;
				PRIMITIVE_TYPE  GetPrimitiveType() const;

				void            SetMaterial(const Material * material);
				Material *      GetMaterial();

	protected:
		VertexStream            m_vertexStream;
		//IndexStream             mIndexStream;
		//int                     mPrimCount;
		//PRIMITIVE_TYPE          mPrimType;

		Material*               m_pMaterial;

		AABB					m_AABB;
		const Matrix4x4*		m_pMatWorld;
		const Matrix4x4*		m_arrSkinMatrix;
		UINT					m_nSkinMatrixNum;
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