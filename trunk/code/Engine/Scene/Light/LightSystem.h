#ifndef  _LightSystem__H__
#define  _LightSystem__H__


namespace ma
{
	class RENDER_API LightSystem
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


	RENDER_API	LightSystem*		GetLightSystem();

	RENDER_API	void				SetLightSystem(LightSystem* pLightSystem);
	
}


#endif