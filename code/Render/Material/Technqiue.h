#ifndef Technique_H_
#define Technique_H_

namespace ma
{
	class RENDER_API Technique : public Referenced
	{
	public:
		Technique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);

		~Technique();
		
		void				Bind();

		void				UnBind();

		const char*			GetTechName();

		ShaderProgram*		GetShaderProgram() {return m_pShaderProgram;}

		RenderState&		GetRenderState() {return m_renderState;}

		void				SetRenderState(const RenderState& renderState) {m_renderState = renderState;}

		void				SetParameter(const char* pszName,const Any& value);
		
		MaterialParameter*	GetParameter(const char* pszName);

	private:
		std::string						m_stName;

		ShaderProgram*					m_pShaderProgram;
	
		RenderState						m_renderState;

		std::vector<MaterialParameter*> m_arrParameters;
	};
	
	DeclareRefPtr(Technique);
}




#endif