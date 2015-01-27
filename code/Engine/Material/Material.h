#ifndef _Material_H_
#define _Material_H_


namespace ma
{
	class MaterialParameter;
	class SamplerState;
	struct SubMaterialData;

	class SubMaterial : public Referenced
	{
	public:
		void				InitWithSubMatData(const SubMaterialData& subMatData);

		Technique*			GetShadowDepthTechnqiue();

		Technique*			GetGbufferTechnqiue();

		Technique*			GetShadingTechnqiue();

		void				SetShadingTechnqiue(Technique* pTech);

		void				SetShadingTechnqiue(const char* pShaderName,const char* pDefine);
		
		RefPtr<SubMaterial>	Clone();
	
	private:
		RefPtr<Technique>	m_pShadingTech;
		
		RefPtr<Technique>	m_pGBufferTech;

		RefPtr<Technique>	m_pShadowDepthTech;

		std::string			m_pShaderMarco;
	};

	DeclareRefPtr(SubMaterial);

	RefPtr<SubMaterial> CreateSubMaterial();
}	

#endif // _Material_H_


