#ifndef _Sample_Lighting_H_
#define _Sample_Lighting_H_

namespace ma
{
	class SampleLighting : public SimpleSceneView
	{
	public:
		SampleLighting();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	private:
		Light* m_pLigt;
	};
}


#endif


