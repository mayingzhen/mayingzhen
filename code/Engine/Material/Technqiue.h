#pragma once

namespace ma
{
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class Renderable;

	class Technique : public Serializable
	{
	public:
		Technique();

		~Technique();

		DECL_OBJECT(Technique);

		static void			RegisterAttribute();

		virtual void		Bind(Renderable* pRenderable);

		virtual void		UnBind();

		virtual void        RT_StreamComplete() = 0;

		const char*			GetTechName() const;
		void				SetTechName(const char* pName);

		const char*			GetShaderDefine() const;
		void				SetShaderDefine(const char* pszDefine);

		ShaderProgram*		GetShaderProgram() const { return m_pShaderProgram.get(); }
		void				SetShaderProgram(ShaderProgram* pShader);

		int					GetShaderMacroInt(const char* pszMacro);
		void				SetShaderMacroInt(const char* pszMacro, int nValue);
		void				SetShaderMacroBool(const char* pszMacro, bool b);
		bool				GetShaderMacroBool(const char* pszMacro);

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);
		UINT				GetParameterCount();
		Parameter*			GetParameterByIndex(UINT nIndex);

		void				SetBlendState(BlendState* pBlendState);
		const BlendState*	GetBlendState() { return m_pBlendState.get(); }

		void				SetDepthStencilState(DepthStencilState* pDSSate);
		const DepthStencilState* GetDepthStencilState() { return m_pDSState.get(); }

		void				SetRasterizerState(RasterizerState* pRSState);
		const RasterizerState* GetRasterizerState() { return m_pRSState.get(); }

		void				SetValue(Uniform* pUniform, int value);
		void				SetValue(Uniform* pUniform, float value);
		void				SetValue(Uniform* pUniform, const Vector2& value);
		void				SetValue(Uniform* pUniform, const Vector3& value);
		void				SetValue(Uniform* pUniform, const Vector4& value);
		void				SetValue(Uniform* pUniform, const Matrix4& value);
		void				SetValue(Uniform* pUniform, const Matrix4* values, UINT count);
		void				SetValue(Uniform* pUniform, const Vector4* values, UINT count);
		void				SetValue(Uniform* pUniform, const ColourValue& value);
		void				SetValue(Uniform* pUniform, Texture* pTexture);
		void				SetValue(Uniform* pUniform, SamplerState* sampler);

		Uniform*			GetUniform(const char* pszName);

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		UINT				GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType, UINT nIndex);
		void				ClearConstBuffer(ShaderType eType);

		void				AddSampler(Uniform* pUniform);
		UINT				GetSamplerCount();
		Uniform*			GetSamplerByIndex(UINT nIndex);

		SamplerState*		GetActiveSampler(UINT nIndex) { return m_arrSampler[nIndex]; }

		void				SetInstTech(Technique* pTech) { m_pInstTech = pTech; }
		Technique*			GetInstTech() { return m_pInstTech.get(); }

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		RefPtr<Technique>   Clone();

		void				SetRenderPass(RenderPass* pRenderPass) { m_pRenderPass = pRenderPass; }
		RenderPass*			GetRenderPass() { return m_pRenderPass; }

	protected:
		void				BindUniform(Renderable* pRenderable);

		void				BindParametersUniform(Uniform* pUniform,const Any& anyValue);

		void				SetValue(Uniform* uniform, const float* values, UINT nSize);

	private:
		std::string						m_stName;
		std::string						m_strDefine;

		RefPtr<ShaderProgram>			m_pShaderProgram;

		RefPtr<BlendState>				m_pBlendState;
		
		RefPtr<DepthStencilState>		m_pDSState;
		UINT							m_nStencilRef;

		RefPtr<RasterizerState>			m_pRSState;

		std::vector<Parameter*>			m_arrParameters; 

		RenderPass*						m_pRenderPass = NULL;

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER					m_vecConstBuffer[ShaderType_Number];

		std::vector< RefPtr<Uniform> >  m_vecPSSamplers;

		SamplerState*					m_arrSampler[MAX_TEXTURE_UNITS];

		RefPtr<Technique>				m_pInstTech;
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine);
    RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine,VertexDeclaration* pVertexDecl = NULL);
}
