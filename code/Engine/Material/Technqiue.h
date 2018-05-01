#pragma once

#include "MaterialParameter.h"

namespace ma
{
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class Renderable;
	struct ShaderCreateInfo;

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

		virtual void		RT_SetSampler(Uniform* pUniform, SamplerState* pSampler) = 0;

		const char*			GetTechName() const;
		void				SetTechName(const char* pName);

		const char*			GetShaderDefine() const;
		void				SetShaderDefine(const char* pszDefine);

		int					GetShaderMacroInt(const char* pszMacro);
		void				SetShaderMacroInt(const char* pszMacro, int nValue);
		void				SetShaderMacroBool(const char* pszMacro, bool b);
		bool				GetShaderMacroBool(const char* pszMacro);

		ShaderProgram*		GetShaderProgram() const { return m_pShaderProgram.get(); }
		void				SetShaderProgram(ShaderProgram* pShader);

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);
		uint32_t			GetParameterCount();
		Parameter*			GetParameterByIndex(uint32_t nIndex);

		void				SetValue(Uniform* pUniform, int value);
		void				SetValue(Uniform* pUniform, float value);
		void				SetValue(Uniform* pUniform, const Vector2& value);
		void				SetValue(Uniform* pUniform, const Vector3& value);
		void				SetValue(Uniform* pUniform, const Vector4& value);
		void				SetValue(Uniform* pUniform, const Matrix4& value);
		void				SetValue(Uniform* pUniform, const Matrix4* values, uint32_t count);
		void				SetValue(Uniform* pUniform, const Vector4* values, uint32_t count);
		void				SetValue(Uniform* pUniform, const ColourValue& value);
		void				SetValue(Uniform* pUniform, Texture* pTexture);
		void				SetValue(Uniform* pUniform, SamplerState* sampler);

		Uniform*			GetUniform(const char* pszName);

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		uint32_t			GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType, uint32_t nIndex);

		void				AddSampler(Uniform* pUniform);
		uint32_t			GetSamplerCount();
		Uniform*			GetSamplerByIndex(uint32_t nIndex);
		
		SamplerState*		GetActiveSampler(uint32_t nIndex) { return m_arrSampler[nIndex]; }

		RefPtr<Technique>	CreateInstTech();

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		void				ReLoad();

		RefPtr<Technique>   Clone();

	protected:
		void				BindUniform(Renderable* pRenderable);

		void				BindParametersUniform(Uniform* pUniform,const Any& anyValue);

		void				SetValue(Uniform* uniform, const float* values, uint32_t nSize);

		void				ClearConstBuffer(ShaderType eType);

		void				ClearSampler();

	private:
		std::string						m_stName;
		std::string						m_strDefine;

		RefPtr<ShaderProgram>			m_pShaderProgram;

		std::vector<Parameter>			m_arrParameters; 

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER					m_vecConstBuffer[ShaderType_Number];

		std::vector< RefPtr<Uniform> >  m_vecPSSamplers;

		SamplerState*					m_arrSampler[MAX_TEXTURE_UNITS];
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine);
//    RefPtr<Technique> CreateTechnique(const char* pVSFile, const char* pPSFile, const char* pDefine,VertexDeclaration* pVertexDecl = NULL);
	RefPtr<Technique> CreateTechnique(const char* pTechName, const ShaderCreateInfo& info);
}
