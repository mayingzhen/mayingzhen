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

		virtual ~Technique();

		DECL_OBJECT(Technique);

		static void			RegisterAttribute();

		virtual void		Bind(Renderable* pRenderable);

		virtual void		BindCompute(Renderable* pRenderable);

		virtual void        RT_StreamComplete() = 0;

		virtual void		RT_SetSampler(Uniform* pUniform, SamplerState* pSampler) = 0;

		virtual void		RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer) = 0;

		const char*			GetShaderDefine() const;
		void				SetShaderDefine(const char* pszDefine);

		RenderPass*			GetRenderPass();
		void				SetRenderPass(RenderPass* pPass);

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
		void				SetValue(Uniform* pUniform, SamplerState* sampler);
		void				SetValue(Uniform* uniform, const uint8_t* values, uint32_t nSize);

		Uniform*			GetUniform(ShaderType eType, const char* pszName);

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		uint32_t			GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType, uint32_t nIndex);

		void				AddSampler(ShaderType eType, Uniform* pUniform);
		uint32_t			GetSamplerCount(ShaderType eType);
		Uniform*			GetSamplerByIndex(ShaderType eType, uint32_t nIndex);

		void				AddStorgeBuffer(Uniform* pUniform);
		uint32_t			GetStorgeBufferCount();
		Uniform*			GetStorgeBufferByIndex(uint32_t nIndex);
		
		void				SetActiveSampler(Uniform* pUniform, SamplerState* pSampler);

		void				SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

		RefPtr<Technique>	CreateInstTech();

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		void				ReLoad();

		bool				GetTransluce();

		RefPtr<Technique>   Clone();

	protected:
		void				BindUniform(Renderable* pRenderable, ShaderType eType);

		void				BindParametersUniform(Renderable* pRenderable,Uniform* pUniform,const Any& anyValue);

		void				ClearConstBuffer(ShaderType eType);

		void				ClearSampler(ShaderType eType);

		void				ClearStorgeBuffer();

	private:
		std::string						m_strDefine;

		RefPtr<RenderPass>				m_pRenderPass;

		RefPtr<ShaderProgram>			m_pShaderProgram;

		std::vector<Parameter>			m_arrParameters; 

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER					m_vecConstBuffer[ShaderType_Number];

		typedef std::vector< RefPtr<Uniform> > VEC_SAMPLER;
		VEC_SAMPLER						m_vecSamplers[ShaderType_Number];

		typedef std::vector< RefPtr<Uniform> > VEC_STOREGEBUFFER;
		VEC_STOREGEBUFFER				m_vecStorgeBuffer;
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine, RenderPass* pPass);
	RefPtr<Technique> CreateTechnique(const ShaderCreateInfo& info);
}
