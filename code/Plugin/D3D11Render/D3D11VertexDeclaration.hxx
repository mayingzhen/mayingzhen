#include "D3D11VertexDeclaration.h"
#include "D3D11ShaderProgram.h"

namespace ma
{

	D3D11VertexDeclaration::D3D11VertexDeclaration()
	{
	}

	D3D11VertexDeclaration::~D3D11VertexDeclaration()
	{
	}

	void D3D11VertexDeclaration::RT_StreamComplete()
	{
// 		// Do not create input layout if no vertex buffers / elements
// 
// 		map<unsigned long long, ID3D11InputLayout* >::iterator
// 			it = m_mapVertexDeclaration.find(newVertexDeclarationHash);
// 		if (it == m_mapVertexDeclaration.end())
// 		{
// 			D3D11_INPUT_ELEMENT_DESC d3dve[MAX_ELEMENT];
// 			for (int i = 0; i < this->GetElementCount(); ++i)
// 			{
// 				const VertexElement& element = this->GetElement(i);
// 				d3dve[i].SemanticName = D3D11Mapping::GetD3DDeclUsage(element.Usage);
// 				d3dve[i].SemanticIndex = element.UsageIndex;
// 				d3dve[i].Format = D3D11Mapping::GetD3DDeclType(element.Type);
// 				d3dve[i].InputSlot = 0;
// 				d3dve[i].AlignedByteOffset = element.Offset;
// 				d3dve[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
// 				d3dve[i].InstanceDataStepRate = 0;
// 			}
// 
// 			ID3D11InputLayout* pD3D11VertexDecl = NULL;
// 			GetD3D11DxDevive()->CreateInputLayout(&d3dve[0], m_pVertexDecl->GetElementCount(), m_pShader->GetByteVSCode(),
// 				m_pShader->GetByteVSCodeSize(), &pD3D11VertexDecl);
// 			ASSERT(pD3D11VertexDecl);
// 			if (pD3D11VertexDecl == NULL)
// 			{
// 				LogError("Failed to create input layout");
// 			}
// 		}
	}
}
