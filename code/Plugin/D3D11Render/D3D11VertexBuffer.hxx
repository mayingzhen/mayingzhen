#include "D3D11VertexBuffer.h"

namespace ma
{

	D3D11VertexBuffer::D3D11VertexBuffer()
	{
		mD3D11VertexBuffer = NULL;
		m_pLockedData = NULL;
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		SAFE_RELEASE(mD3D11VertexBuffer);
	}

	void* D3D11VertexBuffer::Lock(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		if (m_pLockedData)
			return m_pLockedData;

		D3D11_MAP D3DLock;

		if ((LockFlag == LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
			D3DLock = D3D11_MAP_WRITE_DISCARD;

		if (LockFlag == LOCK_NOOVERWRITE)
			D3DLock = D3D11_MAP_WRITE_NO_OVERWRITE;

		if (LockFlag == LOCK_READONLY)
			D3DLock = D3D11_MAP_READ;

		if (LockFlag == LOCK_WRITE)
			D3DLock = D3D11_MAP_WRITE;

		D3D11_MAPPED_SUBRESOURCE mappedData;
		mappedData.pData = 0;

		GetD3D11DxDeviveContext()->Map(mD3D11VertexBuffer, 0, D3DLock, 0,&mappedData);
		m_pLockedData = mappedData.pData;
		ASSERT(m_pLockedData);
		if (m_pLockedData)
		{
			LogError("Failed to map vertex buffer");
		}

		return m_pLockedData;
	}

	void D3D11VertexBuffer::Unlock()
	{
		if (m_pLockedData)
		{
			GetD3D11DxDeviveContext()->Unmap(mD3D11VertexBuffer, 0);

			m_pLockedData = NULL;
		}
	}

	void D3D11VertexBuffer::RT_StreamComplete()
	{
		ASSERT(mD3D11VertexBuffer == NULL);

		ASSERT(m_Size);

		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof bufferDesc);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = m_Usage == HBU_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		bufferDesc.Usage = m_Usage == HBU_DYNAMIC ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = (UINT)(m_Size);

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_pData;

		GetD3D11DxDevive()->CreateBuffer(&bufferDesc, &InitData, &mD3D11VertexBuffer);
		ASSERT(mD3D11VertexBuffer);
		if (mD3D11VertexBuffer == NULL)
		{
			LogError("CreateVertexBuffer failed");
			return;
		}

		if (0/*m_pData*/)
		{
			D3D11_BOX destBox;
			destBox.left = 0;
			destBox.right = m_Size;
			destBox.top = 0;
			destBox.bottom = 1;
			destBox.front = 0;
			destBox.back = 1;

			GetD3D11DxDeviveContext()->UpdateSubresource(mD3D11VertexBuffer, 0, &destBox, m_pData, 0, 0);

			if (m_bNeedFreeData)
			{
				FreeData();
			}
		}
	}

	void D3D11VertexBuffer::NotifyOnDeviceDestroy()
	{
		
	}

	void D3D11VertexBuffer::NotifyOnDeviceLost()
	{
	}

	void D3D11VertexBuffer::NotifyOnDeviceReset()
	{
	}

	ID3D11Buffer * D3D11VertexBuffer::GetD3DVertexBuffer()
	{
		return mD3D11VertexBuffer;
	}
}
