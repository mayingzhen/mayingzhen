#ifndef _Material_H_
#define _Material_H_


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
		void				SetShadingTechnqiue(const char* pShaderName,const char* pDefine);

		Technique*			GetShadowDepthTechnqiue();
		void				SetShadowDepthTechnqiue(Technique* pTech);
		void				SetShadowDepthTechnqiue(const char* pShaderName,const char* pDefine);

		void				SetParameter(const char* pszName,const Any& value);	
		Parameter*			GetParameter(const char* pszName);

		virtual void		Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent);
		virtual void		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);		

		RefPtr<SubMaterial>	Clone();
	
	private:
		RefPtr<Technique>	m_pShadingTech;

		RefPtr<Technique>	m_pShadowDepthTech;

		std::string			m_strShaderName;
		std::string			m_strShaderMacro;

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
	
		UINT				GetLodNumber() const {return m_arrLodSubMaterial.size();}
		UINT				GetLodSubNumber(UINT nLod) const;
		SubMaterial*		GetLodSubByIndex(UINT nLod,uint32 index) const;

		void				AddSubMaterial(UINT nLod,SubMaterial* pSubMaterial);	

		virtual bool		Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

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

#endif // _Material_H_


