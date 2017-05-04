#pragma once

namespace ma
{
	class D3D11SamplerStateObject : public SamplerState
	{
	public:
		D3D11SamplerStateObject() {}
		~D3D11SamplerStateObject() {}

	public:

		virtual void RT_StreamComplete();

	private:

	};

}