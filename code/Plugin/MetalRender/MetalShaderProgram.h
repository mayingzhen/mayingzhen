#pragma once

#include "MetalConstantBuffer.h"



namespace ma
{
    class RenderPass;
    
	class MetalShaderProgram : public ShaderProgram
	{
	public:

		MetalShaderProgram();

		~MetalShaderProgram();
        
        void ParseUniform(MTLRenderPipelineReflection* pReflection);

    protected:
        
        virtual void RT_StreamComplete();
        
        void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);
        
        void Destory();
        
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	public:

		id<MTLFunction>         m_pVertexShader;
		id<MTLFunction> 		m_pPiexelShader;
    
		friend class MetalRenderDevice;
	};


}


