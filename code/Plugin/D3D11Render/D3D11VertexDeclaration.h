#pragma once

#include "D3D11Mapping.h"

namespace ma
{
	class D3D11ShaderProgram;

	class D3D11VertexDeclaration : public VertexDeclaration
	{

	public: 
		D3D11VertexDeclaration();
		
		~D3D11VertexDeclaration();

		//ID3D11InputLayout* GetD3DVertexDeclaration();

	private:

		virtual void RT_StreamComplete();

	protected:
		//ID3D11InputLayout* mD3D11VertexDecl;

		//D3D11ShaderProgram* mD3D11Shader;
	};

}

