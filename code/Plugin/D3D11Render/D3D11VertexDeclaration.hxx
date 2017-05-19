#include "D3D11VertexDeclaration.h"
#include "D3D11ShaderProgram.h"

namespace ma
{
	std::map<uint64, ID3D11InputLayout* > g_vertexDeclarationPool;

	D3D11VertexDeclaration::D3D11VertexDeclaration()
	{
		m_pImpl = NULL;
	}

	D3D11VertexDeclaration::~D3D11VertexDeclaration()
	{
	}

	void D3D11VertexDeclaration::RT_StreamComplete()
	{
		ASSERT(m_pShader);
		if (m_pShader == NULL)
			return;

		ASSERT(m_pImpl == NULL);

		uint64 nHash = this->GetHash();

		auto it = g_vertexDeclarationPool.find(nHash);
		if (it != g_vertexDeclarationPool.end())
		{
			m_pImpl = it->second;
		}
		else
		{
			D3D11_INPUT_ELEMENT_DESC d3dve[MAX_ELEMENT];
			for (int i = 0; i < this->GetElementCount(); ++i)
			{
				const VertexElement& element = this->GetElement(i);
				d3dve[i].SemanticName = D3D11Mapping::GetD3DDeclUsage(element.Usage);
				d3dve[i].SemanticIndex = element.UsageIndex;
				d3dve[i].Format = D3D11Mapping::GetD3DDeclType(element.Type);
				d3dve[i].InputSlot = 0;
				d3dve[i].AlignedByteOffset = element.Offset;
				d3dve[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				d3dve[i].InstanceDataStepRate = 0;
			}

			D3D11ShaderProgram* pD3D11Shader = (D3D11ShaderProgram*)(m_pShader.get());

			ID3D11InputLayout* pD3D11VertexDecl = NULL;
			GetD3D11DxDevive()->CreateInputLayout(&d3dve[0], GetElementCount(), pD3D11Shader->GetByteVSCode(),
				pD3D11Shader->GetByteVSCodeSize(), &pD3D11VertexDecl);
			ASSERT(pD3D11VertexDecl);
			if (pD3D11VertexDecl == NULL)
			{
				LogError("Failed to create input layout");
			}

			g_vertexDeclarationPool.insert(std::make_pair(nHash, pD3D11VertexDecl));

			m_pImpl = pD3D11VertexDecl;
		}
	}

	void D3D11VertexDeclaration::Clear()
	{
		for (auto it = g_vertexDeclarationPool.begin(); it != g_vertexDeclarationPool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_vertexDeclarationPool.clear();
	}
}
