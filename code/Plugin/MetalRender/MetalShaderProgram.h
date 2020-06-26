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
        
        void CreateFromSource(const std::string& vsSource, const std::string& vsFun, const std::string& fsSource, const std::string fsFun);
        
        void Destory();
        
		void ParseShaderUniform(ShaderType eType,const vector<uint8_t>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	public:

		id<MTLFunction>         m_pVertexShader;
		id<MTLFunction> 		m_pPiexelShader;
    
		friend class MetalRenderDevice;
	};


}


