#ifndef  _D3D9VertexDeclaration__H__
#define  _D3D9VertexDeclaration__H__

#include "D3D9Mapping.h"
#include "Engine/RenderSystem/VertexDeclaration.h"

namespace ma
{

	/* class: D3D9VertexDeclaration
	--------------------------------------------------------------------------------------
		@Remark:
			Vertex Declaration For Direct3D 9.0.
	--------------------------------------------------------------------------------------
	*/

	class D3D9RENDER_API D3D9VertexDeclaration : public VertexDeclaration
	{
		//DeclareRefObj();

	   friend class D3D9VideoBufferManager;
	   friend class D3D9RenderSystem;

	public: 
		D3D9VertexDeclaration(IDirect3DDevice9 * Device);
		~D3D9VertexDeclaration();

		void Init();

		IDirect3DVertexDeclaration9 * GetD3DVertexDeclaration() const { return mD3D9VertexDecl; }

	protected:
		IDirect3DDevice9 *                      mD3D9Device;
		IDirect3DVertexDeclaration9 *           mD3D9VertexDecl;
	};

}

#endif
