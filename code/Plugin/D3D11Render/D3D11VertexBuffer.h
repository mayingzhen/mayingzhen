#pragma once

#include "D3D11Resource.h"

namespace ma
{

	class D3D11VertexBuffer : public VertexBuffer, public D3D11Resource
	{
	public:
		D3D11VertexBuffer();

		~D3D11VertexBuffer();

		ID3D11Buffer* GetD3DVertexBuffer();

	private:

		virtual void*	Lock(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	Unlock();

		virtual void	RT_StreamComplete();

		virtual void	NotifyOnDeviceDestroy();
		virtual void	NotifyOnDeviceLost();
		virtual void	NotifyOnDeviceReset();

	protected:
		ID3D11Buffer*			mD3D11VertexBuffer;

		void*					m_pLockedData;		
	};

}
