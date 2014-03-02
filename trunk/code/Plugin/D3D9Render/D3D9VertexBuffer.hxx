#include "D3D9VertexBuffer.h"

namespace ma
{

	D3D9VertexBuffer::D3D9VertexBuffer()
	{
		mD3D9Device = NULL;
		mD3D9VertexBuffer = NULL;
	}

	D3D9VertexBuffer::~D3D9VertexBuffer()
	{
		safe_release_com(mD3D9VertexBuffer);
	}

	// void D3D9VertexBuffer::DeleteSelf()
	// {
	//     VideoBufferManager::Instance()->DestroyVertexBuffer(this);
	// }

	void * D3D9VertexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		if (!mD3D9VertexBuffer)
		{
			Active();
		}

		void * pData = NULL;
		DWORD D3DLock = 0;

		if ((LockFlag & LOCK_DISCARD) && (m_Usage == USAGE_DYNAMIC))
			D3DLock |= D3DLOCK_DISCARD;

		if (LockFlag & LOCK_NOOVERWRITE)
			D3DLock |= D3DLOCK_NOOVERWRITE;

		if (LockFlag & LOCK_READONLY)
			D3DLock |= D3DLOCK_READONLY;

		HRESULT hr = mD3D9VertexBuffer->Lock((UINT)iOffsetBytes, (UINT)iLockSize, &pData, D3DLock);
		ASSERT(hr == D3D_OK && "Lock vertex buffer failed.");

		return pData;
	}

	void D3D9VertexBuffer::Unlock()
	{
		HRESULT hr = mD3D9VertexBuffer->Unlock();
		ASSERT(hr == D3D_OK);
	}

	void D3D9VertexBuffer::Active()
	{
		mD3D9Device = GetD3D9DxDevive();

		ASSERT(mD3D9VertexBuffer == NULL);

		ASSERT(m_Size);

		HRESULT hr;
		DWORD D3DUsage = D3D9Mapping::GetD3DUsage(m_Usage);
		D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(m_Usage);
		IDirect3DVertexBuffer9 * pD3DVB;

		hr = mD3D9Device->CreateVertexBuffer(m_Size, D3DUsage, 0, D3DPool, &pD3DVB, NULL);
		ASSERT(hr == D3D_OK && "D3D Error: CreateVertexBuffer failed");
		if (FAILED(hr))
		{
			return;
		}

		mD3D9VertexBuffer = pD3DVB;

		void* pLockData = NULL;
		DWORD D3DLock = D3DUsage == D3DUSAGE_DYNAMIC ? D3DLOCK_DISCARD : 0;
		hr = mD3D9VertexBuffer->Lock(0, m_Size, &pLockData, D3DLock);
		ASSERT(hr == D3D_OK && "Lock vertex buffer failed.");

		memcpy(pLockData,m_pData,m_Size);

		mD3D9VertexBuffer->Unlock();

	}


	IDirect3DVertexBuffer9 * D3D9VertexBuffer::GetD3DVertexBuffer()
	{
		if (!mD3D9VertexBuffer)
		{
			Active();
		}

		return mD3D9VertexBuffer;
	}

	void D3D9VertexBuffer::LostDevice()
	{
		if (m_Usage == USAGE_DYNAMIC)
		{
			safe_release_com(mD3D9VertexBuffer);
		}
	}

	void D3D9VertexBuffer::ResetDevice()
	{
		if (m_Usage == USAGE_DYNAMIC)
		{
			HRESULT hr = D3D_OK;
			DWORD D3DUsage = D3D9Mapping::GetD3DUsage(m_Usage);

			hr = mD3D9Device->CreateVertexBuffer(m_Size, D3DUsage, 0, D3DPOOL_DEFAULT, &mD3D9VertexBuffer, NULL);

			if (FAILED(hr))
			{
				//EXCEPTION("D3D Error: CreateVertexBuffer failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
			}
		}
	}


}
