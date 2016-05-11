#ifndef Technique_H_
#define Technique_H_

namespace ma
{

	class Technique : public RenderState
	{
	public:
		Technique();

		~Technique();

		DECL_OBJECT(Technique);

		static void			RegisterAttribute();

		void				Bind();

		void				UnBind();

		const char*			GetTechName() const;
		void				SetTechName(const char* pName);

		ShaderProgram*		GetShaderProgram() const {return m_pShaderProgram.get();}
		void				SetShaderProgram(ShaderProgram* pShader);	

		int					GetShaderMacroValue(const char* pszMacro);
		void				SetShaderMacroValue(const char* pszMacro, int nValue);
		void				SetShaderMacro(const char* pszMacro, bool b);
		bool				GetShaderMacro(const char* pszMacro);

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

	private:
		void				BindParametersUniform(Uniform* pUniform,const Any& anyValue);

	private:
		std::string						m_stName;

		RefPtr<ShaderProgram>			m_pShaderProgram;

		std::vector<Parameter*>			m_arrParameters;
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);
}




#endif