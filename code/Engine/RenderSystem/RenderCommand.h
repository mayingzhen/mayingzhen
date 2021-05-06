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

		virtual void SetViewPort(const Rectangle& viewPort) = 0;

		virtual void SetScissor(const Rectangle& viewPort) = 0;

		virtual void DrawIndex(uint32_t nIndexStart, uint32_t nIndexCount, uint32_t nVertexStart, uint32_t nInstanceCount) = 0;

		virtual void Draw(uint32_t nVertexStart, uint32_t nVertexCount, uint32_t nInstanceCount) = 0;
	};

	class ComputeCommand : public Referenced
	{
	public:

		virtual	void Begin() = 0;

		virtual void End() = 0;

		virtual void SetTechnique(Technique* pTech) = 0;

		virtual void SetStorgeBuffer(VertexBuffer* pBuffer) = 0;

		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
	};
}




