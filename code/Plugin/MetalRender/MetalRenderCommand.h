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
        
		virtual	void SetVertexBuffer(int index, VertexBuffer* pVB, uint32_t nOffSet);
        
        virtual void SetTechnique(Technique* pTech);
        
        virtual void SetScissor(uint32_t firstScissor, uint32_t scissorCount, const Vector4* pScissors);

		virtual void DrawIndex(uint32_t nIndexStart, uint32_t nIndexCount, uint32_t nVertexStart, uint32_t nInstanceCount);
        
        virtual void Draw(uint32_t nVertexStart, uint32_t nVertexCount, uint32_t nInstanceCount) ;

	public:
        id<MTLRenderCommandEncoder> m_encoder;
        
        IndexBuffer* m_pCurIB = NULL;
        
        MetalRenderPass* m_pRenderPass = NULL;
        
        // Cach
        enum
        {
            MAX_VB = 6,
        };
        
        IndexBuffer* m_pPreIB = NULL;
        VertexBuffer* m_pPreVB[MAX_VB] = { NULL };
        uint32_t m_nPreVBOffset[MAX_VB] = {0};
        id<MTLDepthStencilState> m_preDS = 0;
        id<MTLRenderPipelineState> m_prePipeline = 0;
        id<MTLTexture> m_preTexture[ShaderType_Number][16] = {0};
        id<MTLSamplerState> m_preSampler[ShaderType_Number][16] = {0};
	};

	class MetalComputeCommad : public ComputeCommad
	{
 public:

		virtual	void Begin();

		virtual void End();

		virtual void SetTechnique(Technique* pTech);

		virtual void SetStorgeBuffer(VertexBuffer* pBuffer);

		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
	};

}


