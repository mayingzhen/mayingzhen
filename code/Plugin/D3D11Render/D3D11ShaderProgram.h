#pragma once


namespace ma
{

	class D3D11ShaderProgram : public ShaderProgram
	{
	public:

		D3D11ShaderProgram();

		~D3D11ShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		virtual void RT_SetShader();

		void Destory();

		const BYTE*	GetByteVSCode() const {return &m_pByteVSCode[0];}
		uint32	GetByteVSCodeSize() {return m_pByteVSCode.size();}

		const BYTE*	GetBytePSCode() const {return &m_pBytePSCode[0];}
		uint32	GetBytePSCodeSize() {return m_pBytePSCode.size();}

		/// Return vertex element mask.
		unsigned GetElementMask() const { return elementMask_; }

	private:
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	private:

		ID3D11VertexShader*		m_pVertexShader;
		ID3D11PixelShader*		m_pPiexelShader;

		vector<BYTE>			m_pByteVSCode;		
		vector<BYTE>			m_pBytePSCode;	

		 /// Vertex element mask for vertex shaders. Zero for pixel shaders.
		unsigned elementMask_;
		RefPtr<ConstantBuffer> vsConstantBuffers_[MAX_SHADER_PARAMETER_GROUPS];
		RefPtr<ConstantBuffer> psConstantBuffers_[MAX_SHADER_PARAMETER_GROUPS];

		friend class D3D11RenderDevice;
	};


}


