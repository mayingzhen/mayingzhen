#ifndef  _ShadowDepthPass__H__
#define  _ShadowDepthPass__H__

namespace ma
{
	class ShadowDepthPass : public RenderPass
	{
		DECL_OBJECT(ShadowDepthPass)

	public:
		void Init();

		void Render();

		void ShoutDown();
	};

}


#endif