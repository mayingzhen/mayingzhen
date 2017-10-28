#pragma once

namespace ma
{
	class RenderCommand : public Referenced
	{
	public:

		virtual	void Begin() = 0;

		virtual void End() = 0;

		virtual void SetIndexBuffer(IndexBuffer* pIB) = 0;

		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB, UINT nOffSet) = 0;

		virtual void SetTechnique(Technique* pTech) = 0;

		virtual void DrawIndex(UINT nIndexStart, UINT nIndexCount, UINT nInstanceCount, PRIMITIVE_TYPE ePrType) = 0;
	};
}




