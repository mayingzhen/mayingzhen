#ifndef _Effect_H_
#define _Effect_H_


namespace ma
{
	class MaterialParameter;
	class Camera;
	class Light;
	struct Renderable;
	class Technique;
	struct Uniform;

	class ENGINE_API Effect
	{
	public:
		Effect(const char* pEffectName);
		
// 		void				Begin(const char* pTechName);
// 
// 		void				End();

		//void				CommitParam();
		void				Bind();

		void				UnBind();

		const char*			GetEffectName() {return m_strEffectName.c_str();}

		void				SetCurCurTechnqiue(const char* pTechName);

		Technique*			GetCurTechnqiue() {return m_pCurTechnque;}

		Technique*			GetTechnqiue(const char* pTechName);

		Technique*			AddTechnique(const char* pTechName,const char* pShadrName,const char* pDefine);

		MaterialParameter*	GetParameter(const char* name);

		void				AddParameter(MaterialParameter* pParam);

	private:
		std::vector<Technique*>		m_arrTechnique;

		Technique*					m_pCurTechnque;

		std::string					m_strEffectName;

		std::vector<MaterialParameter*> m_parameters;
	};

}

#endif


