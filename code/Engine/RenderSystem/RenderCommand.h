#pragma once

namespace ma
{
	class RenderCommand : public Referenced
	{
	public:

		virtual	void Begin() = 0;
		virtual void End() = 0;

		virtual void SetIndexBuffer(IndexBuffer* pIB) = 0;
		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB) = 0;

		virtual void DrawRenderable(const Renderable* pRenderable, Technique* pTech) = 0;
	};
}




