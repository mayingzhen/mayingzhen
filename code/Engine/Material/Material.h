#ifndef _Material_H_
#define _Material_H_


namespace ma
{
	class Parameter;
	class SamplerState;
	

	class SubMaterial : public Referenced
	{
	public:
		SubMaterial();
		~SubMaterial();

		Technique*			GetShadingTechnqiue();
		void				SetShadingTechnqiue(Technique* pTech);
		void				SetShadingTechnqiue(const char* pShaderName,const char* pDefine);

		Technique*			GetShadowDepthTechnqiue();
		void				SetShadowDepthTechnqiue(Technique* pTech);
		void				SetShadowDepthTechnqiue(const char* pShaderName,const char* pDefine);

		void				SetShaderName(const char* pszSharName);
		void				SetShderMacro(const char* pszShaderMacro);
		void				AddParameter(const char* pName,Any value);

		void				Serialize(Serializer& sl, const char* pszLable = "SubMaterial");
		
		RefPtr<SubMaterial>	Clone();
	
	private:
		RefPtr<Technique>	m_pShadingTech;

		RefPtr<Technique>	m_pShadowDepthTech;

		std::string			m_strShaderName;
		std::string			m_strShaderMacro;
		RenderState			m_renderState;

		typedef std::vector<Parameter> VEC_PARAM;
		VEC_PARAM			m_arrParameters;
	};

	RefPtr<SubMaterial> CreateSubMaterial();


	class Material : public Resource
	{
	
	public:
		Material();

		~Material();

		DECL_OBJECT(Material)

		UINT				GetSubMaterialNumber() {return m_arrSubMaterial.size();}
		SubMaterial*		GetSubMaterialByIndex(uint32 index) {return m_arrSubMaterial[index].get();}

		void				AddSubMaterial(SubMaterial* pSubMaterial);	

		virtual void		Serialize(Serializer& sl, const char* pszLable = "Material");

	private:
		typedef std::vector< RefPtr<SubMaterial> >	VEC_SUBMATERIAL;
		VEC_SUBMATERIAL		m_arrSubMaterial;
	};

	RefPtr<Material> CreateMaterial();
	RefPtr<Material> CreateMaterial(const char* pszPath);
}	

#endif // _Material_H_


