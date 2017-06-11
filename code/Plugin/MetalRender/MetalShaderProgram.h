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
        
		virtual void CommitChanges(/*id<MTLRenderCommandEncoder> renderEncoder*/);

		void Destory();

		const BYTE*	GetByteVSCode() const {return &m_pByteVSCode[0];}
		uint32	GetByteVSCodeSize() {return m_pByteVSCode.size();}

		const BYTE*	GetBytePSCode() const {return &m_pBytePSCode[0];}
		uint32	GetBytePSCodeSize() {return m_pBytePSCode.size();}

		/// Return vertex element mask.
        //void SetVetexDecal(MTLVertexDescriptor* pVDec) { m_pVertexDec = pVDec; }

	private:
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	private:

		id<MTLFunction>         m_pVertexShader;
		id<MTLFunction> 		m_pPiexelShader;

		vector<BYTE>			m_pByteVSCode;		
		vector<BYTE>			m_pBytePSCode;	

        std::vector< RefPtr<ConstantBuffer> > m_vecVSConstantBuffers;
        std::vector< RefPtr<ConstantBuffer> > m_vecPSConstantBuffers;
        
        id <MTLRenderPipelineState> _pipelineState;
        MTLRenderPipelineReflection *_reflection;
        
        //MTLVertexDescriptor*    m_pVertexDec;

		friend class MetalRenderDevice;
	};


}


