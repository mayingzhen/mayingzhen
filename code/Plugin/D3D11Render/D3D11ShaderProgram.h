#pragma once


namespace ma
{

	/// %Shader parameter definition.
	struct ShaderParameter
	{
		/// Construct with defaults.
		ShaderParameter() :
		type_(VS),
		buffer_(0),
		offset_(0),
		size_(0),
		bufferPtr_(0)
	{
	}

	/// Construct with parameters.
	ShaderParameter(ShaderType type, const string& name, unsigned buffer, unsigned offset, unsigned size, ConstantBuffer* ptr = 0) :
	type_(type),
		name_(name),
		buffer_(buffer),
		offset_(offset),
		size_(size),
		bufferPtr_(ptr)
	{
	}

	/// %Shader type.
	ShaderType type_;
	/// Name of the parameter.
	string name_;
	/// Constant buffer index.
	unsigned buffer_;
	/// Offset in constant buffer.
	unsigned offset_;
	/// Size of parameter in bytes.
	unsigned size_;
	/// Constant buffer pointer. Defined only in shader programs.
	ConstantBuffer* bufferPtr_;
	};


	class D3D11ShaderProgram : public ShaderProgram , public D3D11Resource
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
		void CalculateConstantBufferSizesVS();
		void CalculateConstantBufferSizesPS();

	private:

		ID3D11VertexShader*		m_pVertexShader;
		ID3D11PixelShader*		m_pPiexelShader;

		//LPD3DXCONSTANTTABLE			m_pVShConstantTable;
		//LPD3DXCONSTANTTABLE			m_pPShConstantTable;

		vector<BYTE>				m_pByteVSCode;		
		vector<BYTE>				m_pBytePSCode;	

		 /// Vertex element mask for vertex shaders. Zero for pixel shaders.
		unsigned elementMask_;

		map<string, ShaderParameter> parametersVS_;
		map<string, ShaderParameter> parametersPS_;

		bool useTextureUnit_[MAX_TEXTURE_UNITS];

		unsigned constantBufferSizesVS_[MAX_SHADER_PARAMETER_GROUPS];
		unsigned constantBufferSizesPS_[MAX_SHADER_PARAMETER_GROUPS];

		RefPtr<ConstantBuffer> vsConstantBuffers_[MAX_SHADER_PARAMETER_GROUPS];
		RefPtr<ConstantBuffer> psConstantBuffers_[MAX_SHADER_PARAMETER_GROUPS];

		friend class D3D11RenderDevice;
	};


}


