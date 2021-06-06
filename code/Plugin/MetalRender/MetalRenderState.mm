#include "MetalRenderState.h"
#include "MetalMapping.h"
#include "MetalRenderDevice.h"

namespace ma
{
	map<DepthStencilState, id<MTLDepthStencilState> > g_depthStatePool;
	//map<RasterizerState, IMetalRasterizerState* > g_rasterizerStatePool;
	//map<BlendState, IMetalBlendState > g_blendStatePool;

	MetalRasterizerStateObject::MetalRasterizerStateObject()
	{
	}

	void MetalRasterizerStateObject::RT_StreamComplete()
	{
	}

	void MetalRasterizerStateObject::Clear()
	{

	}

	MetalDepthStencilStateObject::MetalDepthStencilStateObject()
	{
		m_pMetalDSState = NULL;
	}

	void MetalDepthStencilStateObject::RT_StreamComplete()
	{
		//ASSERT(m_pMetalDSState == NULL);

        auto it = g_depthStatePool.find(*this);
        if (it != g_depthStatePool.end())
        {
            m_pMetalDSState = it->second;
        }
        else
        {
        
            MTLDepthStencilDescriptor* stateDesc = [[[MTLDepthStencilDescriptor alloc] init] autorelease];
            stateDesc.depthWriteEnabled = m_bDepthWrite;
            stateDesc.depthCompareFunction = MetalMapping::get(m_eDepthCheckMode);
        
            if(m_bStencil)
            {
                stateDesc.frontFaceStencil = [[[MTLStencilDescriptor alloc] init] autorelease];
            
                stateDesc.frontFaceStencil.readMask = m_nStencilMask;
                stateDesc.frontFaceStencil.writeMask = m_nStencilWriteMask;
                stateDesc.frontFaceStencil.stencilFailureOperation = MetalMapping::get(m_eStencilFail);
                stateDesc.frontFaceStencil.depthFailureOperation = MetalMapping::get(m_eDepthFailOp);
                stateDesc.frontFaceStencil.depthStencilPassOperation = MetalMapping::get(m_eStencilPass);
                stateDesc.frontFaceStencil.stencilCompareFunction = MetalMapping::get(m_eStencilfunc);
            }
        
            m_pMetalDSState = [GetMetalDevive() newDepthStencilStateWithDescriptor:stateDesc];
            
            if (!m_pMetalDSState)
            {
                LogError("Failed to create depth state");
                return;
            }
            
            g_depthStatePool.insert(std::make_pair(*this, m_pMetalDSState));
        }
	}

	void MetalDepthStencilStateObject::Clear()
	{
		for (auto it = g_depthStatePool.begin(); it != g_depthStatePool.end(); ++it)
		{
			//SAFE_RELEASE(it->second);
		}
		g_depthStatePool.clear();
	}


	MetalBlendStateObject::MetalBlendStateObject()
	{
	}

	void MetalBlendStateObject::RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
	}

	void MetalBlendStateObject::Clear()
	{
	}
}
