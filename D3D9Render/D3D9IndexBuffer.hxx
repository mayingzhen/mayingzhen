#include "D3D9IndexBuffer.h"

namespace ma
{


D3D9IndexBuffer::D3D9IndexBuffer(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

D3D9IndexBuffer::~D3D9IndexBuffer()
{
    safe_release_com(mD3D9IndexBuffer);
}

// void D3D9IndexBuffer::DeleteSelf()
// {
//     VideoBufferManager::Instance()->DestroyIndexBuffer(this);
// }

void * D3D9IndexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
{
    void * pData = NULL;
    DWORD D3DLock = 0;

    if ((LockFlag & LOCK_DISCARD) && (m_eUsage & USAGE_DYNAMIC))
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
    mD3D9IndexBuffer->Unlock();
}

void D3D9IndexBuffer::Bind(void* Data, int iSize,INDEX_TYPE eIndexType, USAGE Usage)
{
	ASSERT(iSize);

	HRESULT hr;
	DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
	D3DFORMAT D3DFormat = D3D9Mapping::GetD3DIndexType(eIndexType);
	D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
	IDirect3DIndexBuffer9 * pD3DIB;

	hr = mD3D9Device->CreateIndexBuffer(iSize, D3DUsage, D3DFormat, D3DPool, &pD3DIB, NULL);
	ASSERT(hr == D3D_OK && "D3D Error: CreateIndexBuffer failed");
	if (FAILED(hr))
	{
		false;
	}

	mD3D9IndexBuffer = pD3DIB;
	m_nSize = iSize;
	m_eUsage = Usage;
	m_eType = eIndexType;

	void * pData = NULL;
	DWORD D3DLock = LOCK_DISCARD;
	hr = mD3D9IndexBuffer->Lock(0, iSize, &pData, D3DLock);
	ASSERT(hr == D3D_OK && "Lock index buffer failed.");

	memcpy(pData,Data,iSize);

	mD3D9IndexBuffer->Unlock();

}

void D3D9IndexBuffer::LostDevice()
{
    if (m_eUsage == USAGE_DYNAMIC)
        safe_release_com(mD3D9IndexBuffer);
}

void D3D9IndexBuffer::ResetDevice()
{
    if (m_eUsage == USAGE_DYNAMIC)
    {
        ASSERT(mD3D9IndexBuffer == NULL);

        HRESULT hr;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(m_eUsage);
        D3DFORMAT D3DFormat = D3DFMT_INDEX16;

        hr = mD3D9Device->CreateIndexBuffer(m_nSize,
                                              D3DUsage,
                                              D3DFormat,
                                              D3DPOOL_DEFAULT,
                                              &mD3D9IndexBuffer,
                                              NULL);
        if (FAILED(hr))
        {
            //EXCEPTION("D3D Error: CreateIndexBuffer failed. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
        }
    }
}

IDirect3DIndexBuffer9 * D3D9IndexBuffer::GetD3DIndexBuffer() const
{
    return mD3D9IndexBuffer;
}

}
