#ifndef Technique_H_
#define Technique_H_

namespace ma
{
	struct RENDER_API RenderState 
	{
		bool				m_bDepthWrite;
		bool				m_bColorWrite;
		float				m_fDepthBias;
		//bool				m_bDepthTest;
		DEPTH_CHECK_MODE	m_eDepthCheckMode;
		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;

		RenderState()
		{
			m_bDepthWrite = true;
			m_bColorWrite = true;
			m_fDepthBias = 0;
			m_eDepthCheckMode = DCM_LESS_EQUAL;
			m_eBlendMode = BM_OPATICY;
			m_eCullMode = CULL_FACE_SIDE_BACK;
		}
	};

	class RENDER_API Technique
	{
	public:
		Technique(/*Effect* pEffect,*/const char* pTechName,const char* pShadrName, const char* pDefine);

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


	private:
		//Effect*			m_pEffect;
		
		std::string		m_stName;

		ShaderProgram*	m_pShaderProgram;
	
		RenderState		m_renderState;

		std::vector<MaterialParameter*> m_parameters;
	};
		
}




#endif