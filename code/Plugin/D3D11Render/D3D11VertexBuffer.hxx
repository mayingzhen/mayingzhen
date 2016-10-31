#include "D3D11VertexBuffer.h"

namespace ma
{

	D3D11VertexBuffer::D3D11VertexBuffer()
	{
		mD3D11VertexBuffer = NULL;
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		SAFE_RELEASE(mD3D11VertexBuffer);
	}

	void* D3D11VertexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		ASSERT(mD3D11VertexBuffer);
		if (mD3D11VertexBuffer == NULL)
			return NULL;

		D3D11_MAP D3DLock;

		if (LockFlag & LOCK_WRITE)
			D3DLock = D3D11_MAP_WRITE;

		if (LockFlag & LOCK_READONLY)
			D3DLock = D3D11_MAP_READ;

		if ((LockFlag & LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
			D3DLock = D3D11_MAP_WRITE_DISCARD;

		if (LockFlag & LOCK_NOOVERWRITE)
			D3DLock = D3D11_MAP_WRITE_NO_OVERWRITE;

		D3D11_MAPPED_SUBRESOURCE mappedData;
		mappedData.pData = 0;

		GetD3D11DxDeviveContext()->Map(mD3D11VertexBuffer, 0, D3DLock, 0,&mappedData);
		ASSERT(mappedData.pData);
		if (mappedData.pData == NULL)
		{
			LogError("Failed to map vertex buffer");
		}

		return mappedData.pData;
	}

	void D3D11VertexBuffer::UnlockImpl()
	{
		ASSERT(mD3D11VertexBuffer);
		if (mD3D11VertexBuffer == NULL)
			return;

		GetD3D11DxDeviveContext()->Unmap(mD3D11VertexBuffer, 0);
	}

	void D3D11VertexBuffer::RT_StreamComplete()
	{
		ASSERT(mD3D11VertexBuffer == NULL);

		ASSERT(m_Size);

		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof bufferDesc);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = m_Usage & HBU_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		if (m_Usage & HBU_DYNAMIC)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else if (m_Usage == HBU_STATIC)
		{
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else
		{
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		}
		bufferDesc.ByteWidth = (UINT)(m_Size);

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_pData;

		GetD3D11DxDevive()->CreateBuffer(&bufferDesc, m_pData ? &InitData : NULL, &mD3D11VertexBuffer);
		ASSERT(mD3D11VertexBuffer);
		if (mD3D11VertexBuffer == NULL)
		{
			LogError("CreateVertexBuffer failed");
			return;
		}

		if (!m_bShadowData)
		{
			FreeData();
		}
	}

	ID3D11Buffer * D3D11VertexBuffer::GetD3DVertexBuffer()
	{
		return mD3D11VertexBuffer;
	}
}
