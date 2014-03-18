#ifndef _MaterialData_H_
#define _MaterialData_H_


namespace ma
{
	class MaterialParameter;

	class RENDER_API MaterialData : public Resource
	{
		DECL_OBJECT(MaterialData)

	public:
		MaterialData(const char* pszPath = NULL);

		~MaterialData();

 		UINT				GetParameterNumber() {return m_arrParameters.size();}
 		
 		MaterialParameter*	GetParameterByIndex(UINT index) {return m_arrParameters[index];}
 
		const char*			GetShaderName() {return m_strShaderName.c_str();}

		const char*			GetShaderMacro() {return m_strShaderMacro.c_str();}

		void				SetShader(const char* pShaderName,const char* pShaderMacro);

		void				SetParameter(const char* pParmName,Vector4 value);

		void				SetParameter(const char* pParmName,SamplerState& sampler);
 
 		RenderState&		GetRenderState() {return m_renderState;}
			
		virtual void		SaveToFile(const char* pszPath);

		virtual bool		CreateFromMemeory();

		virtual void		Serialize(Serializer& sl, const char* pszLable = "Material");

	private:

		MaterialParameter*	GetParameter(const char* pszName);


	private:

		std::string							m_strShaderMacro;

		std::string							m_strShaderName;

		Technique*							m_pTechnique;

		RenderState							m_renderState;
		
		std::vector<MaterialParameter*>		m_arrParameters;
	};

	DeclareRefPtr(MaterialData);
}

#endif // _Material_H_


