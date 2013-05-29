#include "D3D9IndexBuffer.h"

namespace ma
{


D3D9IndexBuffer::D3D9IndexBuffer(void* Data, int size, INDEX_TYPE eIndexType, USAGE Usgae)
: IndexBuffer(Data,size,eIndexType,Usgae)
{
	mD3D9Device = NULL;
	mD3D9IndexBuffer = NULL;
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

void D3D9IndexBuffer::Active()
{
	mD3D9Device = GetD3D9DxDevive();

	ASSERT(m_nSize);

	HRESULT hr;
	DWORD D3DUsage = D3D9Mapping::GetD3DUsage(m_eUsage);
	D3DFORMAT D3DFormat = D3D9Mapping::GetD3DIndexType(m_eType);
	D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(m_eUsage);

	hr = mD3D9Device->CreateIndexBuffer(m_nSize, D3DUsage, D3DFormat, D3DPool, &mD3D9IndexBuffer, NULL);
	ASSERT(hr == D3D_OK && "D3D Error: CreateIndexBuffer failed");
	if (FAILED(hr))
	{
		false;
	}

	void* pLockData = NULL;
	DWORD D3DLock = LOCK_DISCARD;
	hr = mD3D9IndexBuffer->Lock(0, m_nSize, &pLockData, D3DLock);
	ASSERT(hr == D3D_OK && "Lock index buffer failed.");

	memcpy(pLockData,m_pData,m_nSize);

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
