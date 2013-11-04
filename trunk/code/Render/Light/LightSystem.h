#ifndef  _LightSystem__H__
#define  _LightSystem__H__


namespace ma
{
	class RENDER_API LightSystem
	{
	public:
		LightSystem();

		void				Update();

		void				AddLight(Light* pLight) {m_arrLight.push_back(pLight);}

		UINT				GetLightNumber() {return m_arrLight.size();}

		Light*				GetLightByIndex(UINT i) {return m_arrLight[i];}

		const Vector4&		GetAmbientColor() {return m_cAmbientColor;}

		void				SetAmbientColor(const Vector4& cAmbientColor) {m_cAmbientColor = cAmbientColor;}

	private:
		std::vector<Light*>			m_arrLight;

		Vector4						m_cAmbientColor;
	};


	RENDER_API	LightSystem*		GetLightSystem();

	RENDER_API	void				SetLightSystem(LightSystem* pLightSystem);
	
}


#endif