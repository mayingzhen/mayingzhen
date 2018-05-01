#pragma once

#include "MethodBinding.h"

namespace ma
{

	class Uniform;

	struct ShaderCreateInfo
	{
		std::string						m_strVSFile;
		std::string						m_strPSFile;
		std::string						m_shaderMacro;

		RefPtr<VertexDeclaration>		m_pVertexDecl;

		RefPtr<BlendState>				m_pBlendState;

		RefPtr<DepthStencilState>		m_pDSState;
		uint32_t						m_nStencilRef;

		RefPtr<RasterizerState>			m_pRSState;

		RenderPass*						m_pRenderPass = NULL;

		bool operator < (const ShaderCreateInfo& other) const
		{
#define CMPVAR(x) if (x != other.x) return x < other.x;

			CMPVAR(m_pVertexDecl->GetHash());
			CMPVAR(m_strVSFile)
			CMPVAR(m_strPSFile);
			CMPVAR(m_shaderMacro);
			CMPVAR(m_pBlendState);
			CMPVAR(m_pDSState);
			CMPVAR(m_pRSState);

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

		void				Reload();	

		const char*			GetVSFile() const;
		void				SetVSFile(const char* pszVSFile);

		const char*			GetPSFile() const;
		void				SetPSFile(const char* pszPSFile);

		const char*			GetShaderMacro() const;
		void				SetShaderMacro(const char* pszMacro);

		int					GetShaderMacroInt(const char* pszMacro);
		void				SetShaderMacroInt(const char* pszMacro, int nValue);
		void				SetShaderMacroBool(const char* pszMacro, bool b);
		bool				GetShaderMacroBool(const char* pszMacro);
        
        VertexDeclaration*  GetVertexDeclaration() {return m_createInfo.m_pVertexDecl.get();}

		//void				SetBlendState(BlendState* pBlendState) { m_pBlendState = pBlendState; }
		BlendState*			GetBlendState() { return m_createInfo.m_pBlendState.get(); }

		//void				SetDepthStencilState(DepthStencilState* pDSSate) { m_pDSState = pDSSate; }
		DepthStencilState*	GetDepthStencilState() { return m_createInfo.m_pDSState.get(); }

		//void				SetRasterizerState(RasterizerState* pRSState) { m_pRSState = pRSState; }
		RasterizerState*	GetRasterizerState() { return m_createInfo.m_pRSState.get(); }

		void				SetRenderPass(RenderPass* pRenderPass) { m_createInfo.m_pRenderPass = pRenderPass; }
		RenderPass*			GetRenderPass() { return m_createInfo.m_pRenderPass; }

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		uint32_t			GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType,uint32_t nIndex);

		void				AddSampler(Uniform* pUniform);
		uint32_t			GetSamplerCount();
		Uniform*			GetSamplerByIndex(uint32_t nIndex);

		virtual void		RT_StreamComplete() = 0;

	private:
		ShaderCreateInfo				m_createInfo;

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER					m_vecConstBuffer[ShaderType_Number];

		typedef std::vector< RefPtr<Uniform> > VEC_UNIFORM;
		VEC_UNIFORM						m_vecPSSamplers;
	};

	RefPtr<ShaderProgram> CreateShaderProgram(const ShaderCreateInfo& createInfo);
}

