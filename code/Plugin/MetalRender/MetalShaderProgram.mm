#include "MetalShaderProgram.h"
#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"
#include "MetalVertexDeclaration.h"

namespace ma
{
	id<MTLBuffer> g_vecMetalConstantBuffers[2][MAX_SHADER_PARAMETER_GROUPS];

	MetalShaderProgram::MetalShaderProgram()
	{
		m_pVertexShader = NULL;
		m_pPiexelShader = NULL;
        //m_pVertexDec = NULL;
	}

	MetalShaderProgram::~MetalShaderProgram()
	{
		Destory();
	}

	void MetalShaderProgram::Destory()
	{
        m_vecVSConstantBuffers.clear();
        m_vecPSConstantBuffers.clear();
	}

	void MetalShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
			return;

		if (vshSize > 0)
		{
            MTLCompileOptions* options = [[[MTLCompileOptions alloc] init] autorelease];
            
            NSString* ns_src_content = [NSString stringWithUTF8String:vshSource];
            //NSString* ns_src_content = [[NSString alloc] initWithBytes:vshSource length:vshSize encoding:NSUTF8StringEncoding];
            
            NSError* ns_error = nil;
            
            id<MTLLibrary> library = [[GetMetalDevive() newLibraryWithSource:ns_src_content options:options error:&ns_error] autorelease];
            //ASSERT(library != nil);
            if (library == nil && ns_error != nil)
            {
                LogError("Shader vs %s compile error: %s", this->GetVSFile(), ns_error.localizedDescription.UTF8String);
            }
            
            m_pVertexShader= [library newFunctionWithName:@"vs_main"];
            ASSERT(m_pVertexShader);
        }


		if (fshSize > 0)
		{
            MTLCompileOptions* options = [[[MTLCompileOptions alloc] init] autorelease];
            
            NSString* ns_src_content = [NSString stringWithUTF8String:fshSource];
            
            NSError* ns_error = nil;
            
            id<MTLLibrary> library = [[GetMetalDevive() newLibraryWithSource:ns_src_content options:options error:&ns_error] autorelease];
            //ASSERT(library != nil);
            if (library == nil && ns_error != nil)
            {
                LogError("Shader vs %s compile error: %s", this->GetPSFile(), ns_error.localizedDescription.UTF8String);
            }
            
            m_pPiexelShader = [library newFunctionWithName:@"ps_main"];
            ASSERT(m_pPiexelShader);
		}
        
        MetalVertexDeclaration* pMetalVertexDec = (MetalVertexDeclaration*)( this->GetVertexDeclaration() );
        ASSERT(pMetalVertexDec);
        MTLVertexDescriptor* descriptor = NULL;
        if (pMetalVertexDec)
        {
            if (pMetalVertexDec->m_descriptor == nil)
            {
                pMetalVertexDec->RT_StreamComplete();
            }
           descriptor = pMetalVertexDec->m_descriptor;
        }
        
        NSUInteger sampleCount = 1;
        MTLPixelFormat depthPixelFormat = MTLPixelFormatDepth32Float;
        
        //  create a reusable pipeline state
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"MyPipeline";
        [pipelineStateDescriptor setSampleCount: sampleCount];
        [pipelineStateDescriptor setVertexFunction:m_pVertexShader];
        [pipelineStateDescriptor setFragmentFunction:m_pPiexelShader];
        [pipelineStateDescriptor setVertexDescriptor:descriptor];
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        pipelineStateDescriptor.depthAttachmentPixelFormat = depthPixelFormat;
        pipelineStateDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatStencil8;
        
        bool blending = false;
        if (blending)
        {
            //Enable Blending
            pipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
            pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
            pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
            pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorOne;
            pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
            pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        }
        
        NSError* ns_error = nil;
        MTLRenderPipelineReflection *reflectionObj;
        m_pipelineState = [GetMetalDevive() newRenderPipelineStateWithDescriptor:pipelineStateDescriptor options:MTLPipelineOptionArgumentInfo reflection:&reflectionObj error:&ns_error];
        //ASSERT(_pipelineState);
        if (m_pipelineState == nil && ns_error != nil)
        {
            LogError("newRenderPipeline error: %s", ns_error.localizedDescription.UTF8String);
        }
        m_pReflection = reflectionObj;

		return;
	}


	void MetalShaderProgram::ParseUniform()
	{
        ASSERT(m_vecVSConstantBuffers.empty());
        ASSERT(m_vecPSConstantBuffers.empty());
        
        for (MTLArgument *arg in m_pReflection.vertexArguments)
        {
            if (arg.type == MTLArgumentTypeBuffer)
            {
                if (arg.bufferStructType == nil)
                    continue;
                
                RefPtr<ConstantBuffer> pConBuffer =  CreateConstantBuffer(arg.name.UTF8String, arg.index, arg.bufferDataSize);
                m_vecVSConstantBuffers.push_back(pConBuffer);
                for (MTLStructMember* mem in arg.bufferStructType.members)
                {
                    ASSERT(mem.name);
                    Uniform* pUniform = this->AddUniform(mem.name.UTF8String);
                    //pUniform->m_location = 0;
                    pUniform->m_type = mem.dataType;
                    pUniform->m_vshShder = true;
                    //pUniform->m_index = 0;
                    pUniform->m_nCount = 0;
                    pUniform->m_pD3D11CBPtr = pConBuffer.get();
                    pUniform->m_nCBOffset = mem.offset;
                    pUniform->m_nCBSize = arg.bufferDataSize;
                }
            }
        }
        
        for (MTLArgument *arg in m_pReflection.fragmentArguments)
        {
            if (arg.type == MTLArgumentTypeBuffer)
            {
                RefPtr<ConstantBuffer> pConBuffer =  CreateConstantBuffer(arg.name.UTF8String, arg.index, arg.bufferDataSize);
                m_vecPSConstantBuffers.push_back(pConBuffer);
                for (MTLStructMember* mem in arg.bufferStructType.members)
                {
                    Uniform* pUniform = this->AddUniform(mem.name.UTF8String);
                    //pUniform->m_location = 0;
                    pUniform->m_type = mem.dataType;
                    pUniform->m_vshShder = false;
                    //pUniform->m_index = 0;
                    pUniform->m_nCount = 0;
                    pUniform->m_pD3D11CBPtr = pConBuffer.get();
                    pUniform->m_nCBOffset = mem.offset;
                    pUniform->m_nCBSize = arg.bufferDataSize;
                }
            }
            else if (arg.type == MTLArgumentTypeTexture)
            {
                Uniform* pUniform = this->AddUniform(arg.name.UTF8String);
                
                pUniform->m_type = arg.textureDataType;
                pUniform->m_index = arg.index;
            }
        }
	}

	void MetalShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{


	}

	void MetalShaderProgram::RT_SetShader()
	{

	}

	void MetalShaderProgram::CommitChanges(/*id<MTLRenderCommandEncoder> renderEncoder*/)
	{
        MetalRenderDevice* pMetalRender = (MetalRenderDevice*)GetRenderDevice();
        id<MTLRenderCommandEncoder> renderEncoder = pMetalRender->GetRenderCommandEncoder();
        [renderEncoder setRenderPipelineState:m_pipelineState];

		for (UINT i = 0; i < m_vecVSConstantBuffers.size(); ++i)
		{
			if (m_vecVSConstantBuffers[i])
			{
				m_vecVSConstantBuffers[i]->Apply(renderEncoder,false);
			}
		}
        
        for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
        {
            if (m_vecPSConstantBuffers[i])
            {
                m_vecPSConstantBuffers[i]->Apply(renderEncoder,true);
            }
        }
	}
}

