#ifndef _Material_H_
#define _Material_H_


namespace ma
{
	class MaterialParameter;
	class SamplerState;

	class RENDER_API Material 
	{
	public:
		Material();

		Material(ref_ptr<MaterialData> pMatData);

		MaterialParameter*	GetParameter(const char* name);

		MaterialParameter*	AddParameter(const char* name);

		void				AddTechnique(Technique* pTechnique);

		Technique*			GetTechnqiue(const char* pTechName);

		Technique*			LoadTechnique(const std::string& sShaderName,const std::string& sMatFlag);

	private:
		Technique*			AddTechnique(const char* pTechName,const char* pShadrName,const char* pDefine);

	private:

		//std::vector<MaterialParameter*> m_parameters;

		std::vector<Technique*>			m_arrTechnique;

		ref_ptr<MaterialData>			m_pMatData;
	};

	DeclareRefPtr(Material);
}

#endif // _Material_H_


