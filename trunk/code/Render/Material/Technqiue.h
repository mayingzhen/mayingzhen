#ifndef Technique_H_
#define Technique_H_

namespace ma
{
	class RENDER_API Technique
	{
	public:
		Technique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine);

		~Technique();
		
		void			Bind();

		void			UnBind();

		//void			CreateShaderProgram(const char* pszName,const char* pDefine = NULL);

		const char*		GetTechName();

		ShaderProgram*	GetShaderProgram() {return m_pShaderProgram;}

		RenderState&	GetRenderState() {return m_renderState;}

		//Effect*			GetEffect() {return m_pEffect;}

		MaterialParameter*	GetParameter(const char* pszName);

		MaterialParameter*	AddParameter(const char* pszName);


		//virtual void		Serialize(Serializer& sl, const char* pszLable = "Technique");


	private:
		std::string						m_stName;

		ShaderProgram*					m_pShaderProgram;
	
		RenderState						m_renderState;

		std::vector<MaterialParameter*> m_parameters;
	};
		
}




#endif