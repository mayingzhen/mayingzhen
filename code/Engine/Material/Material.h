#pragma once


namespace ma
{
	class Parameter;
	class SamplerState;
	class Material;
	

	class SubMaterial : public Referenced
	{
	public:
		SubMaterial();
		~SubMaterial();

		Technique*			GetShadingTechnqiue();
		void				SetShadingTechnqiue(Technique* pTech);

		Technique*			GetShadowDepthTechnqiue();

		Technique*			GetShadingInstTechnqiue();
		Technique*			GetShadowDepthInstTechnqiue();

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);

		virtual void		Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent);
		virtual void		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		void				ReLoad();

		RefPtr<SubMaterial>	Clone();
	
	private:
		RefPtr<Technique>	m_pShadingTech;
		RefPtr<Technique>	m_pShadingInstTech;

		RefPtr<Technique>	m_pShadowDepthTech;
		RefPtr<Technique>	m_pShadowDepthInstTech;

		typedef std::vector<Parameter> VEC_PARAM;
		VEC_PARAM			m_arrParameters;
	};

	RefPtr<SubMaterial> CreateSubMaterial();


	class Material : public XmlFile
	{
	
	public:
		Material();

		~Material();

		DECL_OBJECT(Material);
	
		uint32_t			GetLodNumber() const {return (uint32_t)m_arrLodSubMaterial.size();}
		uint32_t			GetLodSubNumber(uint32_t nLod) const;
		SubMaterial*		GetLodSubByIndex(uint32_t nLod,uint32_t index) const;

		void				AddSubMaterial(uint32_t nLod,SubMaterial* pSubMaterial);	

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		void				ReLoad();

		RefPtr<Material>	Clone();

	private:
		typedef std::vector< RefPtr<SubMaterial> >	VEC_SUBMATERIAL;
		typedef std::vector< VEC_SUBMATERIAL > VEC_LOD_SUBMATERIAL;
		VEC_LOD_SUBMATERIAL		m_arrLodSubMaterial;
	};

	RefPtr<Material> CreateMaterial();
	RefPtr<Material> CreateMaterial(const char* pszPath);

	extern ResourceSystem<Material>* g_pMaterialManager;
}	


