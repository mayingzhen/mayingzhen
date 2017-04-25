#ifndef _Sample_ShadowMap_H_
#define _Sample_ShadowMap_H_

namespace ma
{
	class SampleShadowMap : public Sample
	{
	public:
		SampleShadowMap();

		virtual void		Load();

		virtual void		UnLoad();

		virtual void		Update();

		virtual void		Render();

		virtual void		keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key);

		void				OnTerrainLoadOver();

	private:
		RefPtr<DirectonalLight>	m_pDirectLight;

		RefPtr<Terrain> m_pTerrain;
	};
}


#endif


