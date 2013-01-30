#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class SampleFbxImport : public SceneView
	{
	public:
		virtual void Init(int windID);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);
	};
}



#endif
