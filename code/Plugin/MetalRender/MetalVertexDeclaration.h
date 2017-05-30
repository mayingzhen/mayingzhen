#pragma once

namespace ma
{
	class D3D11VertexDeclaration : public VertexDeclaration
	{

	public: 
		D3D11VertexDeclaration();
		
		~D3D11VertexDeclaration();

	public:

		virtual void RT_StreamComplete();

		static void Clear();

	public:
		ID3D11InputLayout*	m_pImpl;
	};

}

