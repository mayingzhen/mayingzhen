#include "MetalShaderProgram.h"
#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"
#include "MetalVertexDeclaration.h"
#include "../../Engine/Material/PrePareShaderSource.h"

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
            
            m_pVertexShader = [library newFunctionWithName:@"vs_main"];
            ASSERT(m_pVertexShader);
        
            if (m_pVertexShader.vertexAttributes != nil)
            {
                for(MTLVertexAttribute * mtlAttr in m_pVertexShader.vertexAttributes)
                {
                    if (mtlAttr.active)
                    {
                        int i = 5;
                    }
                    else
                    {
                        int i = 5;
                    }
                }
            }
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
        for (MTLArgument *arg in m_pReflection.vertexArguments)
        {
            if (arg.type == MTLArgumentTypeBuffer)
            {
                if (arg.bufferStructType == nil)
                    continue;
                
                RefPtr<MetalConstantBuffer> pConstantBuffer = new MetalConstantBuffer();
                pConstantBuffer->SetName(arg.name.UTF8String);
                pConstantBuffer->SetBound(arg.index);
                pConstantBuffer->SetSize(arg.bufferDataSize);
                this->AddConstBuffer(VS, pConstantBuffer.get());
                Uniform* pPreUniform = NULL;
                for (MTLStructMember* mem in arg.bufferStructType.members)
                {
                    Uniform* pUniform = pConstantBuffer->AddUniform(mem.name.UTF8String);
    
                    pUniform->SetOffset(mem.offset);

                    // ToDo :getSize mem.dataType
                    if (pPreUniform)
                    {
                        pPreUniform->SetSize(pUniform->GetOffset() - pPreUniform->GetOffset());
                    }
                    else
                    {
                        pUniform->SetSize(pUniform->GetOffset());
                    }
                    
                    pPreUniform = pUniform;
                }
                
                if (pPreUniform)
                {
                    pPreUniform->SetSize(arg.bufferDataSize - pPreUniform->GetOffset());
                }
            }
        }
        
        for (MTLArgument *arg in m_pReflection.fragmentArguments)
        {
            if (arg.type == MTLArgumentTypeBuffer)
            {
                RefPtr<MetalConstantBuffer> pConstantBuffer = new MetalConstantBuffer();
                pConstantBuffer->SetName(arg.name.UTF8String);
                pConstantBuffer->SetBound(arg.index);
                pConstantBuffer->SetSize(arg.bufferDataSize);
                this->AddConstBuffer(PS, pConstantBuffer.get());
                
                Uniform* pPreUniform = NULL;
                for (MTLStructMember* mem in arg.bufferStructType.members)
                {
                    Uniform* pUniform = pConstantBuffer->AddUniform(mem.name.UTF8String);
                    
                    pUniform->SetOffset(mem.offset);
                    
                    // ToDo :getSize mem.dataType
                    if (pPreUniform)
                    {
                        pPreUniform->SetSize(pUniform->GetOffset() - pPreUniform->GetOffset());
                    }
                    else
                    {
                        pUniform->SetSize(pUniform->GetOffset());
                    }
                    
                    pPreUniform = pUniform;
                }
                
                if (pPreUniform)
                {
                    pPreUniform->SetSize(arg.bufferDataSize - pPreUniform->GetOffset());
                }
            }
            else if (arg.type == MTLArgumentTypeTexture)
            {
                RefPtr<Uniform> pUniform = CreateUniform(arg.name.UTF8String);
                pUniform->SetIndex(arg.index);
                
                this->AddSampler(pUniform.get());
            }
        }
	}

	void MetalShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{


	}
    
    void MetalShaderProgram::RT_StreamComplete()
    {
        ASSERT(GetResState() == ResLoaded);
        
        std::string strPath = GetRenderSystem()->GetShaderPath();
        
        std::string strPathVS = strPath + GetVSFile() + ".vert";
        std::string strPathFS = strPath + GetPSFile() + ".frag";
        
        std::string strVshSource = PrePareShaderSource(strPathVS.c_str(), GetShaderMacro());
        std::string strFshSource = PrePareShaderSource(strPathFS.c_str(), GetShaderMacro());
        
        CreateFromSource(strVshSource.c_str(), strVshSource.length(),
                         strFshSource.c_str(), strFshSource.length());
        
        ParseUniform();
        
        SetResState(ResInited);
    }
    
}

