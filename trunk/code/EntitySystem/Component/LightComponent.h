#ifndef  _LightComponent__H__
#define  _LightComponent__H__

#include "Component.h"


namespace ma
{
	class Light;
	class PointLight;
	
// 	class ENTITYSYSTEM_API LightComponent : public Component
// 	{
// 		DECL_OBJECT(LightComponent)
// 
// 	public:
// 		LightComponent(GameObject* pGameObj); 
// 
// 		~LightComponent();
// 
// 		virtual void	Serialize(Serializer& sl, const char* pszLable = "LightComponent");
// 
// 	private:
// 		Light*			m_Light;
// 
// 	};

	class ENTITYSYSTEM_API PointLightComponent : public Component
	{
		DECL_OBJECT(PointLightComponent)
	
	public:
		
 		PointLightComponent(GameObject* pGameObj); 

		~PointLightComponent();

		PointLight*		GetLight() {return m_pLight;}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "PointLightComponent");

		virtual void	BuildRenderItem();

	private:
		PointLight*			m_pLight;

	};
	
}

#endif // _LightComponent__H__
