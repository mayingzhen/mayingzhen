#ifndef  _D3D9IndexBuffer__H__
#define  _D3D9IndexBuffer__H__

namespace ma
{

	class D3D9IndexBuffer : public IndexBuffer , public D3D9Resource
	{
	public:
		D3D9IndexBuffer();

		~D3D9IndexBuffer();
	
		IDirect3DIndexBuffer9 * GetD3DIndexBuffer() ;

	private:
		virtual void*	Lock(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	Unlock();

		virtual void	RT_StreamComplete();

		virtual void	NotifyOnDeviceDestroy();
		virtual void	NotifyOnDeviceLost();
		virtual void	NotifyOnDeviceReset();

	protected:
		IDirect3DDevice9 *          mD3D9Device;

		IDirect3DIndexBuffer9 *     mD3D9IndexBuffer;

		D3DINDEXBUFFER_DESC			m_descBuffer;

		void*						m_pLockedData;	
	};

}

#endif