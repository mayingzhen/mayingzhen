#pragma once


namespace ma
{
    class MetalRenderPass;

	class MetalRenderCommand : public RenderCommand
	{
	public:

		MetalRenderCommand();

		~MetalRenderCommand();

		virtual	void Begin();
		virtual void End();

		virtual void SetIndexBuffer(IndexBuffer* pIB);
		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void DrawRenderable(const Renderable* pRenderable, Technique* pTech);

	public:
        id<MTLRenderCommandEncoder> m_encoder;
        
        MetalRenderPass* m_pRenderPass = NULL;
	};


}


