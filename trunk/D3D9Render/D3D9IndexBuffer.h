#ifndef  _D3D9IndexBuffer__H__
#define  _D3D9IndexBuffer__H__


#include "D3D9Mapping.h"
#include "Engine/RenderSystem/IndexBuffer.h"

namespace ma
{

	/* class: D3D9IndexBuffer
	--------------------------------------------------------------------------------------
		@Remark:
			Index Buffer For Direct3D 9.0.
	--------------------------------------------------------------------------------------
	*/
	class D3D9RENDER_API D3D9IndexBuffer : public IndexBuffer
	{
		//DeclareRefObj();
		DeclareD3DResource();

		friend class D3D9VideoBufferManager;
		friend class D3D9RenderSystem;

	public:
		D3D9IndexBuffer(IDirect3DDevice9 * Device);
		~D3D9IndexBuffer();

		void * Lock(int iOffsetBytes, int uiLockSize, int LockFlags);
		void Unlock();

		virtual void	Bind(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae = USAGE_STATIC);

		IDirect3DIndexBuffer9 * GetD3DIndexBuffer() const;

	protected:
		IDirect3DDevice9 *          mD3D9Device;
		IDirect3DIndexBuffer9 *     mD3D9IndexBuffer;
	};

}

#endif