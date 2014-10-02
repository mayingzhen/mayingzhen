#ifndef _Material_H_
#define _Material_H_


namespace ma
{
	class MaterialParameter;
	class SamplerState;
	struct SubMaterialData;

	class RENDER_API Material : public Referenced
	{
	public:
		void				InitWithSubMatData(const SubMaterialData& subMatData);

		MaterialParameter*	GetParameter(const char* name);

		MaterialParameter*	AddParameter(const char* name);

		void				AddTechnique(Technique* pTechnique);

		Technique*			GetTechnqiue(const char* pTechName);

		Technique*			LoadTechnique(const char* sShaderName,const char* sMatFlag);

		Technique*			AddTechnique(const char* pTechName,const char* pShadrName,const char* pDefine);

	private:
		std::vector<Technique*>			m_arrTechnique;
	};

	DeclareRefPtr(Material);
}

#endif // _Material_H_


