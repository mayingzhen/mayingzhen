#ifndef _Material_H_
#define _Material_H_

#include "MaterialManager.h"

namespace ma
{
	class MaterialParameter;
	class Effect;
	class SamplerState;


	class ENGINE_API Material : public Resource
	{
	public:

		Material();

		~Material();

		MaterialParameter*	GetParameter(const char* name);

		MaterialParameter*	AddParameter(const char* name);

		// Resource
		virtual void		SaveToFile(const char* pszPath);

		virtual bool		LoadFileToMemeory();

		virtual bool		CreateFromMemeory();

		virtual void		Serialize(Serializer& sl, const char* pszLable = "Material");

		//Effect*				CreateEffect(const char* pszEffectName);

		//Effect*				GetEffect() {return m_pEffect;}

		//void				SetEffect(Effect* pEffect) {m_pEffect = pEffect;}

		void				AddTechnique(Technique* pTechnique);

		Technique*			GetTechnqiue(const char* pTechName);

		Technique*			LoadTechnique(const std::string& sShaderName,const std::string& sMatFlag);

		void				SetDiffuse(const char* pDiffPath, Wrap eWrap = CLAMP, FilterOptions eFilter = TFO_BILINEAR);

		void				SetDiffuse(Texture* pTexture, Wrap eWrap = CLAMP, FilterOptions eFilter = TFO_BILINEAR);

	private:
		Technique*			AddTechnique(const char* pTechName,const char* pShadrName,const char* pDefine);

	private:

		std::vector<MaterialParameter*> m_parameters;

		std::string					m_strMaterialFlag;

		std::vector<Technique*>		m_arrTechnique;

		SamplerState*				m_pDiffuse;
		SamplerState*				m_pBumpmap;
		SamplerState*				m_pCustom;
		
	};

}

#endif // _Material_H_


