#pragma once

namespace ma
{
	class RenderCommand : public Referenced
	{
	public:

		virtual	void Begin() = 0;

		virtual void End() = 0;

		virtual void SetIndexBuffer(IndexBuffer* pIB) = 0;

		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB, uint32_t nOffSet) = 0;

		virtual void SetTechnique(Technique* pTech) = 0;

		virtual void DrawIndex(uint32_t nIndexStart, uint32_t nIndexCount, uint32_t nInstanceCount, PRIMITIVE_TYPE ePrType) = 0;
	};
}




