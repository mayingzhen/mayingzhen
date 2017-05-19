#pragma once

namespace ma
{
	class BlendState;
	class DepthStencilState;
	class RasterizerState;

	class Technique : public Serializable
	{
	public:
		Technique();

		~Technique();

		DECL_OBJECT(Technique);

		static void			RegisterAttribute();

		void				Bind();

		void				UnBind();

		void				CommitChanges();

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

		void				SetBlendState(BlendState* pBlendState);
		void				SetDepthStencilState(DepthStencilState* pDSSate);
		void				SetRasterizerState(RasterizerState* pRSState);

		void				SetVertexDeclaration(VertexDeclaration* pVertexDecl);

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

	private:
		void				BindParametersUniform(Uniform* pUniform,const Any& anyValue);

	private:
		std::string						m_stName;
		std::string						m_strDefine;

		RefPtr<ShaderProgram>			m_pShaderProgram;

		RefPtr<BlendState>				m_pBlendState;
		
		RefPtr<DepthStencilState>		m_pDSState;
		UINT							m_nStencilRef;

		RefPtr<RasterizerState>			m_pRSState;

		std::vector<Parameter*>			m_arrParameters;

		RefPtr<VertexDeclaration>		m_pDeclaration;
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine);
	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);
}
