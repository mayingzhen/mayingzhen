#ifndef  _D3D9VertexBuffer__H__
#define  _D3D9VertexBuffer__H__

#include "D3D9Resource.h"

namespace ma
{

	class D3D9VertexBuffer : public VertexBuffer, public D3D9Resource
	{
	public:
		D3D9VertexBuffer();

		~D3D9VertexBuffer();

		IDirect3DVertexBuffer9 * GetD3DVertexBuffer();

	private:

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	Unlock();

		virtual void	RT_StreamComplete();

		virtual void	NotifyOnDeviceDestroy();
		virtual void	NotifyOnDeviceLost();
		virtual void	NotifyOnDeviceReset();

	protected:
		IDirect3DDevice9*          mD3D9Device;

		IDirect3DVertexBuffer9*    mD3D9VertexBuffer;

		D3DINDEXBUFFER_DESC			m_descBuffer;
	};

}

#endif