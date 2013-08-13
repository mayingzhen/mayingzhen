# ifndef MATERIAL_H_
# define MATERIAL_H_

#include "MaterialManager.h"

namespace ma
{
	class MaterialParameter;
	class Camera;
	class Light;
	struct Renderable;
	class Technique;
	struct Uniform;


	class ENGINE_API Material
	{
	public:

		Material(const char* pMaterialFlag,const char* pShaderName = NULL);

		~Material();

		Technique*			CreateTechnique(const char* pTechName,const char* pShadrName, const char* pDefine = NULL);

		void				Bind();

		void				UnBind();

		MaterialParameter*	GetParameter(const char* name);

		void				ClearParameter(const char* name);

		UINT				GetParameterNumber() {return m_parameters.size();}

		MaterialParameter*	GetParameterByIndex(UINT index) {return m_parameters[index];}

		void				SetParameterAutoBinding(const char* name, AutoBinding autoBinding);

		void				SetMaterialFlage(const char* pszFlage) {m_strMaterialFlag = pszFlage;}

		const char*			GetMaterialFlage() {return m_strMaterialFlag.c_str();}

		Technique*			GetCurTechnqiue() {return m_pCurTechnque;}

		void				SetCurTechnqiue(const char* pShaderName,const char* pDefine);

	private:

		std::vector<Technique*>		m_arrTechnique;

		std::string					m_strMaterialFlag;
			
		std::vector<MaterialParameter*> m_parameters;

		Technique*					m_pCurTechnque;
		
	};

}

#endif
