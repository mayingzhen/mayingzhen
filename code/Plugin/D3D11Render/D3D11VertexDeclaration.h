#pragma once

namespace ma
{
	class D3D11VertexDeclaration : public VertexDeclaration
	{

	public: 
		D3D11VertexDeclaration();
		
		~D3D11VertexDeclaration();

	private:

		virtual void RT_StreamComplete();
	};

}

