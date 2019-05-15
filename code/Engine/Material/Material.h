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

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);

		virtual void		Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent);
		virtual void		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

		void				ReLoad();

		RefPtr<SubMaterial>	Clone();
	
	private:
		RefPtr<Technique>	m_pShadingTech;

		RefPtr<Technique>	m_pShadowDepthTech;

		typedef std::vector<Parameter> VEC_PARAM;
		VEC_PARAM			m_arrParameters;
	};

	RefPtr<SubMaterial>	CreateSubMaterial();


	class Material : public XmlFile
	{
	
	public:
		Material();

		~Material();

		DECL_OBJECT(Material);
	
		uint32_t			GetSubNumber() const;
		SubMaterial*		GetSubByIndex(uint32_t index) const;

		void				AddSubMaterial(SubMaterial* pSubMaterial);	

		void				ReLoad();

		RefPtr<Material>	Clone();

	private:
		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc);

	private:
		typedef std::vector< RefPtr<SubMaterial> >	VEC_SUBMATERIAL;
		VEC_SUBMATERIAL		m_arrSubMaterial;
	};

	RefPtr<Material> CreateMaterial();
	RefPtr<Material> CreateMaterial(const char* pszPath);

	extern ResourceSystem<Material>* g_pMaterialManager;

}	



