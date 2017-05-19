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

		virtual void CommitChanges();

		void Destory();

		const BYTE*	GetByteVSCode() const {return &m_pByteVSCode[0];}
		uint32	GetByteVSCodeSize() {return m_pByteVSCode.size();}

		const BYTE*	GetBytePSCode() const {return &m_pBytePSCode[0];}
		uint32	GetBytePSCodeSize() {return m_pBytePSCode.size();}

		/// Return vertex element mask.
		unsigned GetElementMask() const { return m_nElementMask; }

	private:
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	private:

		ID3D11VertexShader*		m_pVertexShader;
		ID3D11PixelShader*		m_pPiexelShader;

		vector<BYTE>			m_pByteVSCode;		
		vector<BYTE>			m_pBytePSCode;	

		 /// Vertex element mask for vertex shaders. Zero for pixel shaders.
		unsigned m_nElementMask;
		RefPtr<ConstantBuffer> m_vecVSConstantBuffers[MAX_SHADER_PARAMETER_GROUPS];
		RefPtr<ConstantBuffer> m_vecPSConstantBuffers[MAX_SHADER_PARAMETER_GROUPS];

		friend class D3D11RenderDevice;
	};


}


