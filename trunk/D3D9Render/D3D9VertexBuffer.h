#ifndef  _D3D9VertexBuffer__H__
#define  _D3D9VertexBuffer__H__

#include "D3D9Mapping.h"
#include "Engine/RenderSystem/VertexBuffer.h"

namespace ma
{

	/* class: D3D9VertexBuffer
	--------------------------------------------------------------------------------------
		@Remark:
			Vertex Buffer For Direct3D 9.0.
	--------------------------------------------------------------------------------------
	*/
	class D3D9RENDER_API D3D9VertexBuffer : public VertexBuffer
	{
		DeclareD3DResource();
		//DeclareRefObj();

	public:
		D3D9VertexBuffer(IDirect3DDevice9 * Device);
		~D3D9VertexBuffer();

		void * Lock(int iOffsetBytes, int iLockSize, int LockFlags);
		void Unlock();

		virtual void	Bind(void* Data, int size, UINT nVertexType, USAGE Usgae = USAGE_STATIC);

		IDirect3DVertexBuffer9 * GetD3DVertexBuffer() const;

		friend class D3D9VideoBufferManager;
		friend class D3D9RenderSystem;

	protected:
		IDirect3DDevice9 *          mD3D9Device;
		IDirect3DVertexBuffer9 *    mD3D9VertexBuffer;
	};

}

#endif