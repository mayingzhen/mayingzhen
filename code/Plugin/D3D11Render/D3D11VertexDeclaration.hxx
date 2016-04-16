#include "D3D11VertexDeclaration.h"
#include "D3D11ShaderProgram.h"

namespace ma
{

	D3D11VertexDeclaration::D3D11VertexDeclaration()
	{
	}

	D3D11VertexDeclaration::~D3D11VertexDeclaration()
	{
		//SAFE_RELEASE(mD3D11VertexDecl);
	}

	void D3D11VertexDeclaration::RT_StreamComplete()
	{
// 		D3D11_INPUT_ELEMENT_DESC d3dve[MAX_ELEMENT];
// 
// 		for (int i = 0; i < m_ElementCount; ++i)
// 		{
// 			d3dve[i].SemanticName = D3D11Mapping::GetD3DDeclUsage(m_Elements[i].Usage);
// 			d3dve[i].SemanticIndex = m_Elements[i].UsageIndex;
// 			d3dve[i].Format   = D3D11Mapping::GetD3DDeclType(m_Elements[i].Type);
// 			d3dve[i].InputSlot = 0;
// 			d3dve[i].AlignedByteOffset = m_Elements[i].Offset;
// 			d3dve[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
// 			d3dve[i].InstanceDataStepRate =0;
// 		}
// 	    
// 		GetD3D11DxDevive()->CreateInputLayout(&d3dve[0], m_ElementCount, mD3D11Shader->GetByteVSCode(),
// 			mD3D11Shader->GetByteVSCodeSize(), &mD3D11VertexDecl);
// 		ASSERT(mD3D11VertexDecl);
// 		if (mD3D11VertexDecl == NULL)
// 		{
// 			LogError("Failed to create input layout");
// 		}
	}

// 	ID3D11InputLayout * D3D11VertexDeclaration::GetD3DVertexDeclaration()  
// 	{ 
// // 		if (!mD3D11VertexDecl)
// // 		{
// // 			RT_StreamComplete();
// // 		}
// // 		return mD3D11VertexDecl; 
// 	}

}
