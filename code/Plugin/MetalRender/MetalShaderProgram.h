#pragma once

#include "MetalConstantBuffer.h"



namespace ma
{

	class MetalShaderProgram : public ShaderProgram
	{
	public:

		MetalShaderProgram();

		~MetalShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		virtual void RT_SetShader();
        
		virtual void CommitChanges();

		void Destory();

	private:
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	private:

		id<MTLFunction>         m_pVertexShader;
		id<MTLFunction> 		m_pPiexelShader;

        std::vector< RefPtr<ConstantBuffer> > m_vecVSConstantBuffers;
        std::vector< RefPtr<ConstantBuffer> > m_vecPSConstantBuffers;
        
        id <MTLRenderPipelineState> m_pipelineState;
        MTLRenderPipelineReflection* m_pReflection;
    

		friend class MetalRenderDevice;
	};


}


