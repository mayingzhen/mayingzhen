#ifndef _Sample_ShadowMap_H_
#define _Sample_ShadowMap_H_

namespace ma
{
	class SampleShadowMap : public SimpleSceneView
	{
	public:
		SampleShadowMap();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	private:
		Light* m_pLigt;
	};
}


#endif


