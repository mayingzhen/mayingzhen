#include "Renderer.h"
//#include "MWMaterialManager.h"

namespace ma
{
	Renderer::Renderer()
	{
		mUsingDefferedShading = true;
	}

	Renderer::~Renderer()
	{
	}

	const AABB & Renderer::GetWorldAabb()
	{
		return AABB();
	}

	void Renderer::GetWorldPosition(Vector3 * pos)
	{
		*pos = Vector3(0,0,0);
	}

	void Renderer::GetWorldTransform(Matrix4x4 * form)
	{
		MatrixIdentity(form);
	}

	int Renderer::GetBlendMatrix(Matrix4x4 * forms)
	{
		return 0;
	}

	void Renderer::BeginRendering()
	{
	}

	void Renderer::EndRendering()
	{
	}

	void Renderer::SetPrimitiveCount(int size)
	{
		mPrimCount = size;
	}

	void Renderer::SetPrimitiveType(PRIMITIVE_TYPE type)
	{
		mPrimType = type;
	}

	VertexStream * Renderer::GetVertexStream()
	{
		return &mVertexStream;
	}

	IndexStream * Renderer::GetIndexStream()
	{
		return &mIndexStream;
	}

	int Renderer::GetPrimitiveCount() const
	{
		return mPrimCount;
	}

	PRIMITIVE_TYPE Renderer::GetPrimitiveType() const
	{
		return mPrimType;
	}

	void Renderer::SetMaterial(const Material * material)
	{
		mMaterial = *material;
	}

	Material * Renderer::GetMaterial()
	{
		return &mMaterial;
	}

}

