#include "MetalShaderProgram.h"
#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"
#include "MetalVertexDeclaration.h"
#include "MetalTexture.h"

#include "../../Engine/Material/PrePareShaderSource.h"

#include "glslang/glslang/Include/ResourceLimits.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"

#include "SPIRV-Cross/spirv_glsl.hpp"
#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "SPIRV-Cross/spirv_msl.hpp"

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

	void MetalShaderProgram::CreateFromSource(const char* vshSource, uint32_t vshSize, const char* fshSource, uint32_t fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
			return;

        const ShaderCreateInfo& info = this->GetShaderCreateInfo();
        
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
                LogError("Shader vs %s compile error: %s", info.m_strVSFile.c_str(), ns_error.localizedDescription.UTF8String);
            }
            
            m_pVertexShader = [library newFunctionWithName:@"main0"];
            ASSERT(m_pVertexShader);
        
            /*
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
             */
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
                LogError("Shader vs %s compile error: %s", info.m_strPSFile.c_str(), ns_error.localizedDescription.UTF8String);
            }
            
            m_pPiexelShader = [library newFunctionWithName:@"main0"];
            ASSERT(m_pPiexelShader);
		}

		return;
	}
    
	void MetalShaderProgram::ParseUniform(MTLRenderPipelineReflection* pReflection)
	{
        for (MTLArgument *arg in pReflection.vertexArguments)
        {
            if (arg.type == MTLArgumentTypeBuffer)
            {
                if (arg.bufferStructType == nil)
                    continue;
                
                RefPtr<MetalConstantBuffer> pConstantBuffer = new MetalConstantBuffer();
                pConstantBuffer->SetName(arg.name.UTF8String);
                pConstantBuffer->SetBound((uint32_t)arg.index);
                pConstantBuffer->SetSize((uint32_t)arg.bufferDataSize);
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
            else if (arg.type == MTLArgumentTypeTexture)
            {
                RefPtr<Uniform> pUniform = CreateUniform(arg.name.UTF8String);
                pUniform->SetIndex(arg.index);
                
                this->AddSampler(VS,pUniform.get());
            }
        }
        
        for (MTLArgument *arg in pReflection.fragmentArguments)
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
                
                this->AddSampler(PS,pUniform.get());
            }
        }
	}

	void MetalShaderProgram::ParseShaderUniform(ShaderType eType,const vector<uint8_t>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{

	}
    
    void init_resources(TBuiltInResource &Resources) {
        Resources.maxLights = 32;
        Resources.maxClipPlanes = 6;
        Resources.maxTextureUnits = 32;
        Resources.maxTextureCoords = 32;
        Resources.maxVertexAttribs = 64;
        Resources.maxVertexUniformComponents = 4096;
        Resources.maxVaryingFloats = 64;
        Resources.maxVertexTextureImageUnits = 32;
        Resources.maxCombinedTextureImageUnits = 80;
        Resources.maxTextureImageUnits = 32;
        Resources.maxFragmentUniformComponents = 4096;
        Resources.maxDrawBuffers = 32;
        Resources.maxVertexUniformVectors = 128;
        Resources.maxVaryingVectors = 8;
        Resources.maxFragmentUniformVectors = 16;
        Resources.maxVertexOutputVectors = 16;
        Resources.maxFragmentInputVectors = 15;
        Resources.minProgramTexelOffset = -8;
        Resources.maxProgramTexelOffset = 7;
        Resources.maxClipDistances = 8;
        Resources.maxComputeWorkGroupCountX = 65535;
        Resources.maxComputeWorkGroupCountY = 65535;
        Resources.maxComputeWorkGroupCountZ = 65535;
        Resources.maxComputeWorkGroupSizeX = 1024;
        Resources.maxComputeWorkGroupSizeY = 1024;
        Resources.maxComputeWorkGroupSizeZ = 64;
        Resources.maxComputeUniformComponents = 1024;
        Resources.maxComputeTextureImageUnits = 16;
        Resources.maxComputeImageUniforms = 8;
        Resources.maxComputeAtomicCounters = 8;
        Resources.maxComputeAtomicCounterBuffers = 1;
        Resources.maxVaryingComponents = 60;
        Resources.maxVertexOutputComponents = 64;
        Resources.maxGeometryInputComponents = 64;
        Resources.maxGeometryOutputComponents = 128;
        Resources.maxFragmentInputComponents = 128;
        Resources.maxImageUnits = 8;
        Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        Resources.maxCombinedShaderOutputResources = 8;
        Resources.maxImageSamples = 0;
        Resources.maxVertexImageUniforms = 0;
        Resources.maxTessControlImageUniforms = 0;
        Resources.maxTessEvaluationImageUniforms = 0;
        Resources.maxGeometryImageUniforms = 0;
        Resources.maxFragmentImageUniforms = 8;
        Resources.maxCombinedImageUniforms = 8;
        Resources.maxGeometryTextureImageUnits = 16;
        Resources.maxGeometryOutputVertices = 256;
        Resources.maxGeometryTotalOutputComponents = 1024;
        Resources.maxGeometryUniformComponents = 1024;
        Resources.maxGeometryVaryingComponents = 64;
        Resources.maxTessControlInputComponents = 128;
        Resources.maxTessControlOutputComponents = 128;
        Resources.maxTessControlTextureImageUnits = 16;
        Resources.maxTessControlUniformComponents = 1024;
        Resources.maxTessControlTotalOutputComponents = 4096;
        Resources.maxTessEvaluationInputComponents = 128;
        Resources.maxTessEvaluationOutputComponents = 128;
        Resources.maxTessEvaluationTextureImageUnits = 16;
        Resources.maxTessEvaluationUniformComponents = 1024;
        Resources.maxTessPatchComponents = 120;
        Resources.maxPatchVertices = 32;
        Resources.maxTessGenLevel = 64;
        Resources.maxViewports = 16;
        Resources.maxVertexAtomicCounters = 0;
        Resources.maxTessControlAtomicCounters = 0;
        Resources.maxTessEvaluationAtomicCounters = 0;
        Resources.maxGeometryAtomicCounters = 0;
        Resources.maxFragmentAtomicCounters = 8;
        Resources.maxCombinedAtomicCounters = 8;
        Resources.maxAtomicCounterBindings = 1;
        Resources.maxVertexAtomicCounterBuffers = 0;
        Resources.maxTessControlAtomicCounterBuffers = 0;
        Resources.maxTessEvaluationAtomicCounterBuffers = 0;
        Resources.maxGeometryAtomicCounterBuffers = 0;
        Resources.maxFragmentAtomicCounterBuffers = 1;
        Resources.maxCombinedAtomicCounterBuffers = 1;
        Resources.maxAtomicCounterBufferSize = 16384;
        Resources.maxTransformFeedbackBuffers = 4;
        Resources.maxTransformFeedbackInterleavedComponents = 64;
        Resources.maxCullDistances = 8;
        Resources.maxCombinedClipAndCullDistances = 8;
        Resources.maxSamples = 4;
        Resources.limits.nonInductiveForLoops = 1;
        Resources.limits.whileLoops = 1;
        Resources.limits.doWhileLoops = 1;
        Resources.limits.generalUniformIndexing = 1;
        Resources.limits.generalAttributeMatrixVectorIndexing = 1;
        Resources.limits.generalVaryingIndexing = 1;
        Resources.limits.generalSamplerIndexing = 1;
        Resources.limits.generalVariableIndexing = 1;
        Resources.limits.generalConstantMatrixVectorIndexing = 1;
    }
    
    EShLanguage FindLanguage(ShaderType eType)
    {
        switch (eType)
        {
            case VS:
            return EShLangVertex;
            
            //case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            //return EShLangTessControl;
            
            //case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            //return EShLangTessEvaluation;
            
            //case VK_SHADER_STAGE_GEOMETRY_BIT:
            //return EShLangGeometry;
            
            case PS:
            return EShLangFragment;
            
            //case VK_SHADER_STAGE_COMPUTE_BIT:
            //return EShLangCompute;
            
            default:
            return EShLangVertex;
        }
    }

    
    std::string HlslToMsl(const char* vshSource, uint32_t vshSize, ShaderType eType)
    {
        //VS
        EShLanguage stage = FindLanguage(eType);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char *shaderStrings[1];
        TBuiltInResource Resources;
        init_resources(Resources);
        
        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);
        
        shaderStrings[0] = vshSource;
        shader.setStrings(shaderStrings, 1);
        shader.setEntryPoint("main");
        
        if ( !shader.parse(&Resources, 100, false, messages) )
        {
            LogError(shader.getInfoLog());
            LogError(shader.getInfoDebugLog());
            return "";
        }
        
        program.addShader(&shader);
        
        if ( !program.link(messages) )
        {
            LogError(shader.getInfoLog());
            LogError(shader.getInfoDebugLog());
            return "";
        }
        
        std::vector<uint32_t> vtx_spv;
        glslang::GlslangToSpv(*program.getIntermediate(stage), vtx_spv);
        
        spirv_cross::CompilerMSL msl(vtx_spv.data(), vtx_spv.size());
        
        // The SPIR-V is now parsed, and we can perform reflection on it.
        spirv_cross::ShaderResources resources = msl.get_shader_resources();
        
		std::vector<spirv_cross::MSLResourceBinding> vecResBinding;
        for (auto &resource : resources.uniform_buffers)
        {
            unsigned set = eType == VS ? 0 : 1;
            msl.set_decoration(resource.id, spv::DecorationDescriptorSet, set);
            
            unsigned binding = msl.get_decoration(resource.id, spv::DecorationBinding);
            
			spirv_cross::MSLResourceBinding resBinding;
            resBinding.desc_set = set;
			resBinding.stage = eType == VS ? spv::ExecutionModelVertex : spv::ExecutionModelFragment;
			resBinding.binding = binding;
			resBinding.msl_buffer = binding + 1;
			vecResBinding.push_back(resBinding);
        }
        
        for (auto &resource : resources.separate_samplers)
        {
            msl.set_decoration(resource.id, spv::DecorationDescriptorSet, 2);
            
            unsigned binding = msl.get_decoration(resource.id, spv::DecorationBinding);
            
            spirv_cross::MSLResourceBinding resBinding;
            resBinding.desc_set = 2;
            resBinding.stage = eType == VS ? spv::ExecutionModelVertex : spv::ExecutionModelFragment;
            resBinding.binding = binding;
            resBinding.msl_sampler = binding;
            vecResBinding.push_back(resBinding);
        }

		//bool bShadow = false;
        for (auto &resource : resources.separate_images)
        {
            msl.set_decoration(resource.id, spv::DecorationDescriptorSet, 3);
            
            /*
            if (resource.name == "g_tShadowMap")
            {
                //const spirv_cross::SPIRType& spType = msl.get_type(resource.type_id);
                //const spirv_cross::SPIRType& spTypeSelf = msl.get_type(spType.self);
                //spirv_cross::SPIRType* pSpTypeSelf = (spirv_cross::SPIRType*)&spTypeSelf;
                //pSpTypeSelf->image.depth = true;
                
                //spirv_cross::SPIRType* pSpType = (spirv_cross::SPIRType*)&spType;
                //pSpType->image.depth = true;

				bShadow = true;
            }
             */
            
            unsigned binding = msl.get_decoration(resource.id, spv::DecorationBinding);
            
            spirv_cross::MSLResourceBinding resBinding;
            resBinding.desc_set = 3;
            resBinding.stage = eType == VS ? spv::ExecutionModelVertex : spv::ExecutionModelFragment;
            resBinding.binding = binding;
            resBinding.msl_texture = binding;
            vecResBinding.push_back(resBinding);
        }
        
        
        // Set some options.
        spirv_cross::CompilerMSL::Options options;
        options.platform = spirv_cross::CompilerMSL::Options::iOS;
        msl.set_options(options);
        
        // Compile to GLSL, ready to give to GL driver.
        std::string source = msl.compile(NULL, &vecResBinding);

        /*
		if (bShadow)
		{
			std::string strFrom = "sample_compare(g_sShadowMap,";
			std::string strTo = "sample_compare(sampler(coord::normalized, filter::linear, address::clamp_to_edge, compare_func::less),";
			source = StringUtil::replaceAll(source, strFrom, strTo);
            
            strFrom = "texture2d<float> g_tShadowMap";
            strTo = "depth2d<float> g_tShadowMap";
            source = StringUtil::replaceAll(source, strFrom, strTo);
		}
         */
        
        return source;
    }
    
    void MetalShaderProgram::RT_StreamComplete()
    {
        ASSERT(GetResState() == ResLoaded);
        
        std::string strPath = GetRenderSystem()->GetShaderPath();
        
        const ShaderCreateInfo& info = this->GetShaderCreateInfo();
        
        std::string strPathVS = strPath + info.m_strVSFile + ".vert";
        std::string strPathFS = strPath + info.m_strPSFile + ".frag";
        
        std::string strVshSource = PrePareShaderSource(strPathVS.c_str(), info.m_shaderMacro.c_str());
        std::string strFshSource = PrePareShaderSource(strPathFS.c_str(), info.m_shaderMacro.c_str());
        
        std::string strMslVSSource = HlslToMsl(strVshSource.c_str(), strVshSource.size(), VS);
        std::string strMslFSSource = HlslToMsl(strFshSource.c_str(), strFshSource.size(), PS);
        
        CreateFromSource(strMslVSSource.c_str(), strMslVSSource.length(),
                         strMslFSSource.c_str(), strMslFSSource.length());
        
        SetResState(ResInited);
    }
    
}

