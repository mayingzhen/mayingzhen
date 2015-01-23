#ifndef _D3D9ShaderProgram_H_
#define _D3D9ShaderProgram_H_



namespace ma
{

	class D3D9ShaderProgram : public ShaderProgram , public D3D9Resource
	{
	public:

		D3D9ShaderProgram();

		~D3D9ShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		virtual void RT_SetShader();

		void Destory();

	private:

		IDirect3DVertexShader9*		m_pVertexShader;
		IDirect3DPixelShader9*		m_pPiexelShader;

		LPD3DXCONSTANTTABLE			m_pVShConstantTable;
		LPD3DXCONSTANTTABLE			m_pPShConstantTable;
	};


}

#endif // _D3D9ShaderProgram_H_
