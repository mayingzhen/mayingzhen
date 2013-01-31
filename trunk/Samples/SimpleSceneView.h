#ifndef _SimpleSceneView_H_
#define _SimpleSceneView_H_

namespace ma
{
	class SimpleSceneView : public SceneView
	{
	public:
		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	protected:
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProj;
	
		Application* m_pApplication;
	};
}

#endif
