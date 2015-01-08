#ifndef _Material_H_
#define _Material_H_


namespace ma
{
	class MaterialParameter;
	class SamplerState;
	struct SubMaterialData;

	class RENDER_API SubMaterial : public Referenced
	{
	public:
		void				InitWithSubMatData(const SubMaterialData& subMatData);

		void				AddTechnique(Technique* pTechnique);

		Technique*			GetTechnqiueByName(const char* pTechName);

		Technique*			GetShadowDepthTechnqiue();

		Technique*			GetGbufferTechnqiue();

		Technique*			GetShadingTechnqiue();
		
		Technique*			GetTechnqiueByIndex(UINT index);

		Technique*			AddTechnique(const char* pTechName,const char* pShadrName,const char* pDefine);
		
		void				Serialize(Serializer& sl, const char* pszLable = "Material");

		RefPtr<SubMaterial>	Clone();

	private:

		Technique*			LoadTechnique(const char* sShaderName,const char* sMatFlag);
		
	private:
		typedef std::vector< RefPtr<Technique> > VEC_TECHNIQUE;
		VEC_TECHNIQUE		m_arrTechnique;

	};

	DeclareRefPtr(SubMaterial);

	RefPtr<SubMaterial> CreateSubMaterial();
}	

#endif // _Material_H_


