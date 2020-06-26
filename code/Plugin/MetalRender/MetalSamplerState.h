#pragma once

#import <Metal/Metal.h>

namespace ma
{
	class MetalSamplerStateObject : public SamplerState
	{
	public:
		MetalSamplerStateObject();

	public:

		virtual void RT_StreamComplete();

		static void Clear();

	public:
		id<MTLSamplerState> m_pImpl;
	};

}
