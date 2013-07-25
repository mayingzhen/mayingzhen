#ifndef  _D3D9IndexBuffer__H__
#define  _D3D9IndexBuffer__H__

namespace ma
{

	class D3D9RENDER_API D3D9IndexBuffer : public IndexBuffer
	{
		DeclareD3DResource();

	public:
		D3D9IndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC);

		~D3D9IndexBuffer();
	
		IDirect3DIndexBuffer9 * GetD3DIndexBuffer() ;

	private:
		virtual void*	Lock(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	Unlock();

		virtual void	Active();

	protected:
		IDirect3DDevice9 *          mD3D9Device;
		IDirect3DIndexBuffer9 *     mD3D9IndexBuffer;
	};

}

#endif