#ifndef  _D3D9VertexDeclaration__H__
#define  _D3D9VertexDeclaration__H__

#include "D3D9Mapping.h"

namespace ma
{
	class D3D9VertexDeclaration : public VertexDeclaration
	{

	public: 
		D3D9VertexDeclaration(IDirect3DDevice9 * Device);
		
		~D3D9VertexDeclaration();

		IDirect3DVertexDeclaration9 * GetD3DVertexDeclaration()  ;

	private:

		virtual void Active();

	protected:
		IDirect3DDevice9 *                      mD3D9Device;
		IDirect3DVertexDeclaration9 *           mD3D9VertexDecl;
	};

}

#endif
