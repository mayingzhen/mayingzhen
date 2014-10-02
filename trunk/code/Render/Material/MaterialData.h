#ifndef _MaterialData_H_
#define _MaterialData_H_

#include "MaterialParameter.h"

namespace ma
{
	struct RENDER_API SubMaterialData 
	{
		std::string							m_strShaderMacro;

		std::string							m_strShaderName;

		RenderState							m_renderState;

		std::vector<MaterialParameter>		m_arrParameters;

		virtual void						Serialize(Serializer& sl, const char* pszLable = "SubMaterial");
	};


	class RENDER_API MaterialData : public Resource
	{
		DECL_OBJECT(MaterialData)

	public:
		MaterialData(const char* pszPath = NULL);

		~MaterialData();

		UINT							GetSubMatDataNumber() {return m_arrSubMatData.size();}

		const SubMaterialData&			GetSubMatDataByIndex(int index) {return m_arrSubMatData[index];}

		void							AddSubMatData(const SubMaterialData& subMatData);

		virtual void					SaveToFile(const char* pszPath);

		virtual bool					CreateFromMemeory();

		virtual void					Serialize(Serializer& sl, const char* pszLable = "Material");

	private:
		std::vector<SubMaterialData>	m_arrSubMatData;
	};

	DeclareRefPtr(MaterialData);
}

#endif // _Material_H_


