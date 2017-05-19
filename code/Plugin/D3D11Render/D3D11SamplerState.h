#pragma once

namespace ma
{
	class D3D11SamplerStateObject : public SamplerState
	{
	public:
		D3D11SamplerStateObject();

	public:

		virtual void RT_StreamComplete();

		static void Clear();

	public:
		ID3D11SamplerState* m_pImpl;
	};

}