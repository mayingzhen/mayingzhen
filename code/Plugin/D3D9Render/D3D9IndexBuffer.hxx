#include "D3D9IndexBuffer.h"

namespace ma
{


D3D9IndexBuffer::D3D9IndexBuffer()
{
	mD3D9Device = NULL;
	mD3D9IndexBuffer = NULL;
}

D3D9IndexBuffer::~D3D9IndexBuffer()
{
    SAFE_RELEASE(mD3D9IndexBuffer);
}


void * D3D9IndexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
{
    void * pData = NULL;
    DWORD D3DLock = 0;

    if ((LockFlag & LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
        D3DLock |= D3DLOCK_DISCARD;

    if (LockFlag & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;

    if (LockFlag & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    HRESULT hr = mD3D9IndexBuffer->Lock(iOffsetBytes, iLockSize, &pData, D3DLock);
    ASSERT(hr == D3D_OK && "Lock index buffer failed.");

    return pData;
}

void D3D9IndexBuffer::Unlock()
{
	HRESULT hr = D3D_OK;
    hr = mD3D9IndexBuffer->Unlock();
	ASSERT(hr == D3D_OK);
}

void D3D9IndexBuffer::RT_StreamComplete()
{
	mD3D9Device = GetD3D9DxDevive();

	ASSERT(m_Size);

	HRESULT hr;
	DWORD D3DUsage = D3D9Mapping::GetD3DUsage(m_Usage);
	D3DFORMAT D3DFormat = D3D9Mapping::GetD3DIndexType(GetIndexType());
	D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(m_Usage);

	hr = mD3D9Device->CreateIndexBuffer(m_Size, D3DUsage, D3DFormat, D3DPool, &mD3D9IndexBuffer, NULL);
	ASSERT(hr == D3D_OK && "D3D Error: CreateIndexBuffer failed");
	if (FAILED(hr))
	{
		false;
	}

	void* pLockData = NULL;
	DWORD D3DLock = D3DUsage == USAGE_DYNAMIC ? LOCK_DISCARD : 0;
	hr = mD3D9IndexBuffer->Lock(0, m_Size, &pLockData, D3DLock);
	ASSERT(hr == D3D_OK && "Lock index buffer failed.");

	memcpy(pLockData,m_pData,m_Size);

	mD3D9IndexBuffer->Unlock();
}


void D3D9IndexBuffer::NotifyOnDeviceDestroy()
{
	SAFE_RELEASE(mD3D9IndexBuffer);
}

void D3D9IndexBuffer::NotifyOnDeviceLost()
{
	if (m_descBuffer.Pool == D3DPOOL_DEFAULT)
	{
		SAFE_RELEASE(mD3D9IndexBuffer);
	}
}

void D3D9IndexBuffer::NotifyOnDeviceReset()
{
	if (m_descBuffer.Pool == D3DPOOL_DEFAULT)
	{
		ASSERT(mD3D9IndexBuffer == NULL);
	}
}

IDirect3DIndexBuffer9 * D3D9IndexBuffer::GetD3DIndexBuffer() 
{
    return mD3D9IndexBuffer;
}

}
