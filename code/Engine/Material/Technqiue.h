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

		RenderState&		GetRenderState() {return m_renderState;}
		void				SetRenderState(const RenderState& renderState) {m_renderState = renderState;}

		void				SetParameter(const char* pszName,const Any& value);	
		MaterialParameter*	GetParameter(const char* pszName);

		RefPtr<Technique>	Clone();

	private:
		std::string						m_stName;

		RefPtr<ShaderProgram>			m_pShaderProgram;
	
		RenderState						m_renderState;

		std::vector<MaterialParameter*> m_arrParameters;
	};
	
	DeclareRefPtr(Technique);

	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);
}




#endif