#ifndef  _LightSystem__H__
#define  _LightSystem__H__


namespace ma
{
	class LightSystem
	{
	public:
		LightSystem();

		//void				Update();

		void				AddLight(Light* pLight);

		void				RemoveLight(Light* pLight);

		UINT				GetLightNumber() {return m_arrLight.size();}

		Light*				GetLightByIndex(UINT i) {return m_arrLight[i];}

		const ColourValue&	GetAmbientColor() {return m_cAmbientColor;}

		void				SetAmbientColor(const ColourValue& cAmbientColor) {m_cAmbientColor = cAmbientColor;}

	private:
		std::vector<Light*>			m_arrLight;

		ColourValue					m_cAmbientColor;
	};

	extern LightSystem*	g_pLightSystem;	
	LightSystem* GetLightSystem();
	
}


#endif