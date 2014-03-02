#ifndef _D3D9ShaderProgram_H_
#define _D3D9ShaderProgram_H_



namespace ma
{

	class D3D9ShaderProgram : public ShaderProgram
	{
	public:

		D3D9ShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* defines);

		~D3D9ShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		void SetValue(Uniform* uniform, float value);

		void SetValue(Uniform* uniform, const float* values, UINT count = 1);

		void SetValue(Uniform* uniform, int value);

		void SetValue(Uniform* uniform, const int* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Matrix4x4& value);

		void SetValue(Uniform* uniform, const Matrix4x4* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector2& value);

		void SetValue(Uniform* uniform, const Vector2* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector3& value);

		void SetValue(Uniform* uniform, const Vector3* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector4& value);

		void SetValue(Uniform* uniform, const Vector4* values, UINT count = 1);

		void SetValue(Uniform* uniform, const SamplerState* sampler);

		void SetValue(Uniform* uniform, const SamplerState** values, UINT count);

		void SetValue(Uniform* uniform, const Texture* sampler);

		void SetValue(Uniform* uniform, const Texture** values, UINT count) ;

		void Bind();

	private:

		IDirect3DVertexShader9*		m_pVertexShader;
		IDirect3DPixelShader9*		m_pPiexelShader;

		LPD3DXCONSTANTTABLE			m_pVShConstantTable;
		LPD3DXCONSTANTTABLE			m_pPShConstantTable;
	};


}

#endif // _D3D9ShaderProgram_H_
