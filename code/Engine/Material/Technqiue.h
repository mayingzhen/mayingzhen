#ifndef Technique_H_
#define Technique_H_

namespace ma
{

	class Technique : public Referenced
	{
	public:
		Technique();

		~Technique();

		void				Bind();

		void				UnBind();

		const char*			GetTechName() const;
		void				SetTechName(const char* pName);

		ShaderProgram*		GetShaderProgram() const {return m_pShaderProgram.get();}
		void				SetShaderProgram(ShaderProgram* pShader);	

		void				AddShaderMarco(const char* pszMarco);

		RenderState&		GetRenderState() {return m_renderState;}
		void				SetRenderState(const RenderState& renderState) {m_renderState = renderState;}

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);

		void				Serialize(Serializer& sl, const char* pszLable = "Technique");

		RefPtr<Technique>	Clone();

	private:
		std::string						m_stName;

		RefPtr<ShaderProgram>			m_pShaderProgram;
	
		RenderState						m_renderState;

		std::vector<Parameter*>			m_arrParameters;
	};
	
	RefPtr<Technique> CreateTechnique();
	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);
}




#endif