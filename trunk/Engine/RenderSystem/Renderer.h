#ifndef  _Renderer__H__
#define  _Renderer__H__

#include "Material.h"
#include "PrimtiveStream.h"
//#include "MWLight.h"

namespace ma
{

	class ENGINE_API Renderer
	{
		//DECLARE_ALLOC();

	public:
		static const int ClassID = 'Rnd0';

	public:
								Renderer();
		virtual                 ~Renderer();

		virtual int				GetClassID() const { return ClassID; }
		virtual const AABB&		GetWorldAabb();
		virtual void            GetWorldPosition(Vector3* pos);
		virtual void            GetWorldTransform(Matrix4x4* form);
		virtual int				GetBlendMatrix(Matrix4x4* forms);
		virtual void            BeginRendering();
		virtual void            EndRendering();

				void            SetPrimitiveCount(int size);
				void            SetPrimitiveType(PRIMITIVE_TYPE type);

		virtual VertexStream *  GetVertexStream();
		virtual IndexStream *   GetIndexStream();
				int             GetPrimitiveCount() const;
				PRIMITIVE_TYPE  GetPrimitiveType() const;

				void            SetMaterial(const Material * material);
				Material *      GetMaterial();

		//virtual Technique *		GetTechnique(eRenderTechType::enum_t type) { return NULL; }

				void			SetUsingDefferedShading(bool b) { mUsingDefferedShading = b; }
				bool			IsUsingDefferedShading() { return mUsingDefferedShading; }

	protected:
		VertexStream            mVertexStream;
		IndexStream             mIndexStream;
		int                     mPrimCount;
		PRIMITIVE_TYPE          mPrimType;

		Material                mMaterial;
		bool					mUsingDefferedShading;
	};

	struct RenderOp
	{
		Matrix4x4 xform;
		RenderState rState;
		VertexStream vxStream;
		IndexStream ixStream;
		int iPrimCount;
		PRIMITIVE_TYPE ePrimType;

		RenderOp()
		{
			//xform =  Mat4::Identity;
			MatrixIdentity(&xform);
			iPrimCount = 0;
			ePrimType = PRIM_TRIANGLELIST;
		}
	};

}

#endif