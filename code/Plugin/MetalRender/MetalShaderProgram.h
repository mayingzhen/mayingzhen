#pragma once

#include "MetalConstantBuffer.h"



namespace ma
{

	class MetalShaderProgram : public ShaderProgram
	{
	public:

		MetalShaderProgram();

		~MetalShaderProgram();

    protected:
        
        virtual void RT_StreamComplete();
        
        void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);
        
        void ParseUniform();
        
        void Destory();
        
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	public:

		id<MTLFunction>         m_pVertexShader;
		id<MTLFunction> 		m_pPiexelShader;
        
        id <MTLRenderPipelineState> m_pipelineState;
        MTLRenderPipelineReflection* m_pReflection;
    
		friend class MetalRenderDevice;
	};


}


