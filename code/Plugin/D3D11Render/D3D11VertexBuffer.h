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

		virtual void*	LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlags);
		
		virtual void	UnlockImpl();

		virtual void	RT_StreamComplete();

	protected:
		ID3D11Buffer*			mD3D11VertexBuffer;	
	};

}
