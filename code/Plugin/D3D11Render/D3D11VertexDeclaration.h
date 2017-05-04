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

	public:
		D3D11VertexDeclaration*	m_pImpl;
	};

}

