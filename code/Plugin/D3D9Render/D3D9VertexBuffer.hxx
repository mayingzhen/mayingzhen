#include "D3D9VertexBuffer.h"

namespace ma
{

	D3D9VertexBuffer::D3D9VertexBuffer()
	{
		mD3D9Device = NULL;
		mD3D9VertexBuffer = NULL;
		m_pLockedData = NULL;
	}

	D3D9VertexBuffer::~D3D9VertexBuffer()
	{
		SAFE_RELEASE(mD3D9VertexBuffer);
	}

	void * D3D9VertexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		if (m_pLockedData)
			return m_pLockedData;

		DWORD D3DLock = 0;

		if ((LockFlag & LOCK_DISCARD) && (m_Usage == USAGE_DYNAMIC))
			D3DLock |= D3DLOCK_DISCARD;

		if (LockFlag & LOCK_NOOVERWRITE)
			D3DLock |= D3DLOCK_NOOVERWRITE;

		if (LockFlag & LOCK_READONLY)
			D3DLock |= D3DLOCK_READONLY;

		HRESULT hr = mD3D9VertexBuffer->Lock((UINT)iOffsetBytes, (UINT)iLockSize, &m_pLockedData, D3DLock);
		ASSERT(hr == D3D_OK && "Lock vertex buffer failed.");

		return m_pLockedData;
	}

	void D3D9VertexBuffer::Unlock()
	{
		if (m_pLockedData)
		{
			HRESULT hr = mD3D9VertexBuffer->Unlock();
			ASSERT(hr == D3D_OK);

			m_pLockedData = NULL;
		}
	}

	void D3D9VertexBuffer::RT_StreamComplete()
	{
		mD3D9Device = GetD3D9DxDevive();

		ASSERT(mD3D9VertexBuffer == NULL);

		ASSERT(m_Size);

		HRESULT hr;
		DWORD D3DUsage = D3D9Mapping::GetD3DBufferUsage(m_Usage);
		D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(m_Usage);
		IDirect3DVertexBuffer9 * pD3DVB;

		hr = mD3D9Device->CreateVertexBuffer(m_Size, D3DUsage, 0, D3DPool, &pD3DVB, NULL);
		ASSERT(hr == D3D_OK && "D3D Error: CreateVertexBuffer failed");
		if (FAILED(hr))
		{
			return;
		}

		mD3D9VertexBuffer = pD3DVB;

		if (m_pData)
		{
			void* pLockData = NULL;
			DWORD D3DLock = D3DUsage == D3DUSAGE_DYNAMIC ? D3DLOCK_DISCARD : 0;
			hr = mD3D9VertexBuffer->Lock(0, m_Size, &pLockData, D3DLock);
			ASSERT(hr == D3D_OK && "Lock vertex buffer failed.");

			memcpy(pLockData,m_pData,m_Size);

			mD3D9VertexBuffer->Unlock();

			if (m_bNeedFreeData)
			{
				FreeData();
			}
		}
	}

	void D3D9VertexBuffer::NotifyOnDeviceDestroy()
	{
		SAFE_RELEASE(mD3D9VertexBuffer);
	}

	void D3D9VertexBuffer::NotifyOnDeviceLost()
	{
		if (m_descBuffer.Pool == D3DPOOL_DEFAULT)
		{
			SAFE_RELEASE(mD3D9VertexBuffer);
		}
	}

	void D3D9VertexBuffer::NotifyOnDeviceReset()
	{
		if (m_descBuffer.Pool == D3DPOOL_DEFAULT)
		{
			ASSERT(mD3D9VertexBuffer == NULL);
		}
	}



	IDirect3DVertexBuffer9 * D3D9VertexBuffer::GetD3DVertexBuffer()
	{
		return mD3D9VertexBuffer;
	}
}
