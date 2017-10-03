#include "MetalSamplerState.h"
#include "MetalMapping.h"
#include "MetalRenderDevice.h"


namespace ma
{
	std::map<SamplerState, id<MTLSamplerState> > g_samplerStatesPool;

	MetalSamplerStateObject::MetalSamplerStateObject()
	{
		m_pImpl = nil;
	}

	void MetalSamplerStateObject::RT_StreamComplete()
	{
		//ASSERT(m_pImpl == nil);
        if (m_pImpl != nil)
            return;

		auto it = g_samplerStatesPool.find(*this);
		if (it != g_samplerStatesPool.end())
		{
			m_pImpl = it->second;
		}
		else
		{
			MTLSamplerDescriptor* samplerDesc = [[[MTLSamplerDescriptor alloc] init] autorelease];

            MTLSamplerMinMagFilter minmag;
            MTLSamplerMipFilter mipmap;
			MetalMapping::GetFilter(this->GetFilterMode(), minmag, mipmap);
			samplerDesc.magFilter = minmag;
            samplerDesc.mipFilter = mipmap;
			//samplerDesc.Filter = MetalMapping::GetFilter(this->GetFilterMode()); 
			samplerDesc.sAddressMode =  MetalMapping::GetWrap(this->GetWrapMode());
			samplerDesc.tAddressMode = MetalMapping::GetWrap(this->GetWrapMode());
			samplerDesc.rAddressMode = MetalMapping::GetWrap(this->GetWrapModeW());
			samplerDesc.maxAnisotropy = 1;//graphics_->GetTextureAnisotropy();
			//samplerDesc.ComparisonFunc = Metal_COMPARISON_ALWAYS;
			//samplerDesc.MinLOD = 0;
			//samplerDesc.MaxLOD = Metal_FLOAT32_MAX;

			//if (this->GetFilterMode() == TFO_SHADOWCOMPARE)
			//{
			//	samplerDesc.ComparisonFunc = Metal_COMPARISON_LESS;	 
			//}

			m_pImpl = [GetMetalDevive() newSamplerStateWithDescriptor:samplerDesc];

			g_samplerStatesPool.insert(std::make_pair(*this, m_pImpl));
		}
	}

	void MetalSamplerStateObject::Clear()
	{
		for (auto it = g_samplerStatesPool.begin(); it != g_samplerStatesPool.end(); ++it)
		{
			//SAFE_RELEASE(it->second);
		}
		g_samplerStatesPool.clear();
	}

}
