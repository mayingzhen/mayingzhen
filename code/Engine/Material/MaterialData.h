#ifndef _MaterialData_H_
#define _MaterialData_H_

#include "MaterialParameter.h"
#include "RenderState.h"

namespace ma
{
	struct  SubMaterialData 
	{
	public:
		void						SetShaderName(const char* pName);
		const char*					GetShaderName() const;

		void						SetShderMacro(const char* pMacro);
		const char*					GetShaderMacro() const;

		void						AddParameter(const char* pName,Any value);
		UINT						GetParameterCount() const {return m_arrParameters.size();}
		const MaterialParameter&	GetParameterByIndex(UINT index) const {return m_arrParameters[index];}

		const RenderState&			GetRenderState() const {return m_renderState;}
		RenderState&				GetRenderState()  {return m_renderState;}

		void						Serialize(Serializer& sl, const char* pszLable = "SubMaterial");

	private:
		std::string					m_strShaderMacro;

		std::string					m_strShaderName;

		RenderState					m_renderState;

		typedef std::vector<MaterialParameter>	VEC_PARAMTER;
		VEC_PARAMTER				m_arrParameters;
	};


	class MaterialData : public Resource
	{
		DECL_OBJECT(MaterialData)

	public:
		MaterialData(const char* pszPath = NULL);

		~MaterialData();

		UINT							GetSubMatDataNumber() {return m_arrSubMatData.size();}

		const SubMaterialData&			GetSubMatDataByIndex(int index) {return m_arrSubMatData[index];}

		void							AddSubMatData(const SubMaterialData& subMatData);
		SubMaterialData&				AddSubMatData();		

		virtual void					Serialize(Serializer& sl, const char* pszLable = "Material");

	private:
		std::vector<SubMaterialData>	m_arrSubMatData;
	};

	DeclareRefPtr(MaterialData);

}

#endif // _Material_H_


