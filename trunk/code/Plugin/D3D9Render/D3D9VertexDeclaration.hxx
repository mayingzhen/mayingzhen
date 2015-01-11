#include "D3D9VertexDeclaration.h"

namespace ma
{

	D3D9VertexDeclaration::D3D9VertexDeclaration(IDirect3DDevice9 * Device)
	: mD3D9Device(Device),
	  mD3D9VertexDecl(NULL)
	{
	}

	D3D9VertexDeclaration::~D3D9VertexDeclaration()
	{
		SAFE_RELEASE(mD3D9VertexDecl);
	}

	void D3D9VertexDeclaration::RT_StreamComplete()
	{
		D3DVERTEXELEMENT9 d3dve[MAX_ELEMENT];

		for (int i = 0; i < m_ElementCount; ++i)
		{
			d3dve[i].Stream = m_Elements[i].Stream;
			d3dve[i].Offset = m_Elements[i].Offset;
			d3dve[i].Type   = D3D9Mapping::GetD3DDeclType(m_Elements[i].Type);
			d3dve[i].Method = D3DDECLMETHOD_DEFAULT;
			d3dve[i].Usage = D3D9Mapping::GetD3DDeclUsage(m_Elements[i].Usage);
			d3dve[i].UsageIndex = m_Elements[i].UsageIndex;
		}
	    
		d3dve[m_ElementCount].Stream = 0xFF;
		d3dve[m_ElementCount].Offset = 0;
		d3dve[m_ElementCount].Type   = D3DDECLTYPE_UNUSED;
		d3dve[m_ElementCount].Method = 0;
		d3dve[m_ElementCount].Usage  = 0;
		d3dve[m_ElementCount].UsageIndex = 0;

		HRESULT hr = mD3D9Device->CreateVertexDeclaration(d3dve, &mD3D9VertexDecl);
		ASSERT(hr == D3D_OK);
	}

	IDirect3DVertexDeclaration9 * D3D9VertexDeclaration::GetD3DVertexDeclaration()  
	{ 
		if (!mD3D9VertexDecl)
		{
			RT_StreamComplete();
		}
		return mD3D9VertexDecl; 
	}

}
