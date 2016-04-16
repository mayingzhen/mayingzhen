#pragma once

namespace ma
{

	class D3D11IndexBuffer : public IndexBuffer , public D3D11Resource
	{
	public:
		D3D11IndexBuffer();

		~D3D11IndexBuffer();
	
		ID3D11Buffer * GetD3DIndexBuffer() ;

	private:
		virtual void*	Lock(int iOffsetBytes, int uiLockSize, LOCK LockFlags);
		
		virtual void	Unlock();

		virtual void	RT_StreamComplete();

		virtual void	NotifyOnDeviceDestroy();
		virtual void	NotifyOnDeviceLost();
		virtual void	NotifyOnDeviceReset();

	protected:
	
		ID3D11Buffer*    mD3D11IndexBuffer;

		void*			m_pLockedData;	
	};

}

