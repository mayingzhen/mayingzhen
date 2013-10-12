#ifndef Technique_H_
#define Technique_H_

namespace ma
{
	struct RenderState 
	{
		bool				m_bDepthWrite;
		//bool				m_bDepthTest;
		DEPTH_CHECK_MODE	m_eDepthCheckMode;
		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;

		RenderState()
		{
			m_bDepthWrite = true;
			m_eDepthCheckMode = DCM_LESS_EQUAL;
			m_eBlendMode = BM_OPATICY;
			m_eCullMode = CULL_FACE_SIDE_BACK;
		}
	};

	class Technique
	{
	public:
		Technique(Effect* pEffect,const char* pName);

		~Technique();
		
		void			Bind();

		void			UnBind();

		void			CreateShaderProgram(const char* pszName,const char* pDefine = NULL);

		const char*		GetTechName();

		ShaderProgram*	GetShaderProgram() {return m_pShaderProgram;}

		RenderState&	GetRenderState() {return m_renderState;}

	private:
		Effect*			m_pEffect;
		
		std::string		m_stName;

		ShaderProgram*	m_pShaderProgram;
	
		RenderState		m_renderState;
	};
		
}




#endif