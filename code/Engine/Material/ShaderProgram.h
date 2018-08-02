#pragma once

#include "MethodBinding.h"

namespace ma
{

	class Uniform;

	struct ShaderCreateInfo
	{
		RefPtr<VertexDeclaration>		m_pVertexDecl;

		RefPtr<BlendState>				m_pBlendState;

		RefPtr<DepthStencilState>		m_pDSState;
		uint32_t						m_nStencilRef;

		RefPtr<RasterizerState>			m_pRSState;

		RefPtr<RenderPass>				m_pRenderPass;

		std::string						m_strVSFile;
		std::string						m_strPSFile;
		std::string						m_shaderMacro;

		bool operator < (const ShaderCreateInfo& other) const
		{
#define CMPVAR(x) if (x != other.x) return x < other.x;

			CMPVAR(m_pVertexDecl->GetHash());
			CMPVAR(m_strVSFile)
			CMPVAR(m_strPSFile);
			CMPVAR(m_shaderMacro);
			CMPVAR(m_pBlendState.get());
			CMPVAR(m_pDSState.get());
			CMPVAR(m_pRSState.get());
			CMPVAR(m_pRenderPass.get());

			return false;
		}
	};

	class ShaderProgram : public Resource
	{
		
	public:
		ShaderProgram();

		virtual ~ShaderProgram();
	
		DECL_OBJECT(ShaderProgram)

		void				CreateFromFile(const ShaderCreateInfo& createInfo);

		const ShaderCreateInfo& GetShaderCreateInfo() { return m_createInfo; }

		void				SetRenderPass(RenderPass* pRenderPass) { m_createInfo.m_pRenderPass = pRenderPass; }
		RenderPass*			GetRenderPass() { return m_createInfo.m_pRenderPass.get(); }

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		uint32_t			GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType,uint32_t nIndex);

		void				AddSampler(ShaderType eType, Uniform* pUniform);
		uint32_t			GetSamplerCount(ShaderType eType);
		Uniform*			GetSamplerByIndex(ShaderType eType, uint32_t nIndex);

		virtual void		RT_StreamComplete() = 0;

	private:
		ShaderCreateInfo	m_createInfo;

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER		m_vecConstBuffer[ShaderType_Number];

		typedef std::vector< RefPtr<Uniform> > VEC_UNIFORM;
		VEC_UNIFORM			m_vecSamplers[ShaderType_Number];
	};

	RefPtr<ShaderProgram> CreateShaderProgram(const ShaderCreateInfo& createInfo);
}

